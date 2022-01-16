/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/
#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif
#include <map>
#include <utility>

#include <QDialogButtonBox>

#include <obs-module.h>
#include <QObject>
#include "ui_midi_mapping_wizard.h"
#include "../device-manager.h"
#include "../config.h"
#include "Macros.h"
#include <QListWidget>
#include <qwizard.h>
PluginWindow::PluginWindow(QWidget *parent) : QDialog(parent, Qt::Dialog), ui(new Ui::PluginWindow)
{
	ui->setupUi(this);
	auto *macros = new Macros(ui);
	macros->setParent(this);
	// Set Window Title
	setup_actions();
	set_title_window();
	// configure_table();
	connect_ui_signals();
	ui->box_action->setAlignment((int)Alignment::Top_Center);
	ui->box_action->setFlat(true);
	ui->box_action->setPalette(ui->list_mapping->palette());

	// connect(ui->search_mapping, &QLineEdit::textChanged, devmodel, &QSortFilterProxyModel::setFilterWildcard);
	starting = false;
}
WizardWindow::WizardWindow(QWidget *parent) : QWizard(parent), wiz(new Ui::Wizard)
{
	wiz->setupUi(this);
}
WizardWindow::~WizardWindow() {}

void PluginWindow::set_title_window()
{
	QString title;
	title.append(GIT_TAG);
	blog(LOG_DEBUG, "OBS-MIDI Version: %s", title.qtocs());
	title.prepend("OBS MIDI Settings:  ");
	this->setWindowTitle(title);
}
void PluginWindow::connect_ui_signals() const
{
	connect(ui->list_midi_dev, SIGNAL(currentTextChanged(QString)), this, SLOT(on_device_select(QString)));
	connect(ui->check_enabled, SIGNAL(stateChanged(int)), this, SLOT(on_check_enabled_state_changed(int)));
	// Connections for Configure Tab
	connect(ui->cb_obs_output_action, SIGNAL(currentTextChanged(QString)), this, SLOT(obs_actions_select(QString)));
	// connect(ui->table_mapping, SIGNAL(cellClicked(int, int)), this, SLOT(edit_mapping()));
	/**************Connections to mappper****************/
	connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(add_new_mapping()));
	connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(reset_to_defaults()));
	connect(ui->btn_delete, SIGNAL(clicked()), this, SLOT(delete_mapping()));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
	connect(ui->list_mapping, &QListWidget::currentRowChanged, this, &PluginWindow::table_select);
}
void PluginWindow::setup_actions() const
{
	ui->cb_obs_output_action->clear();
	ui->cb_obs_output_action->addItems(Utils::TranslateActions());
	ui->cb_obs_output_action->setCurrentIndex(1);
	ui->cb_obs_output_action->setCurrentIndex(0);
}
void PluginWindow::ToggleShowHide()
{
	if (!isVisible()) {
		load_devices();
		ui->tabWidget->setCurrentIndex(0);
		ui->list_midi_dev->setCurrentRow(0);
		setVisible(true);
	} else {
		setVisible(false);
		ui->btn_Listen_many->setChecked(false);
		ui->btn_Listen_one->setChecked(false);
		reset_to_defaults();
	}
}

void PluginWindow::load_devices()
{
	loadingdevices = true;
	const auto midiOutDevices = GetDeviceManager()->get_output_ports_list();
	const auto midiDevices = GetDeviceManager()->get_input_ports_list();
	this->ui->list_midi_dev->clear();
	if (midiDevices.size() == 0) {
		this->ui->list_midi_dev->addItem("No Devices Available");
		ui->tab_configure->setEnabled(false);
		ui->check_enabled->setEnabled(false);
		ui->tabWidget->setEnabled(false);
	} else if (midiDevices.size() > 0) {
		for (int i = 0; i < midiDevices.size(); i++) {
			this->ui->list_midi_dev->addItem(midiDevices.at(i));
		}
		ui->tab_configure->setEnabled(true);
		ui->check_enabled->setEnabled(true);
		ui->tabWidget->setEnabled(true);
	}
	loadingdevices = false;
}
void PluginWindow::on_check_enabled_state_changed(int state) const
{
	if (state == Qt::CheckState::Checked) {
		const auto selectedDeviceName = ui->list_midi_dev->currentItem()->text().toStdString();
		auto device = GetDeviceManager()->get_midi_device(selectedDeviceName.c_str());
		blog(LOG_INFO, "Item enabled: %s", selectedDeviceName.c_str());
		const int devicePort = GetDeviceManager()->get_input_port_number(selectedDeviceName.c_str());
		if (device == NULL) {
			device = GetDeviceManager()->register_midi_device(devicePort);
		}
		device->set_enabled(true);
		device->open_midi_input_port();
		
		set_configure_title(QString::fromStdString(selectedDeviceName));
		connect_midi_message_handler();
	}
	GetConfig()->Save();
}
void PluginWindow::disconnect_midi_message_handler() const
{
	auto devicemanager = GetDeviceManager();
	auto devices = devicemanager->get_active_midi_devices();
	for (auto device : devices) {
		disconnect(device, SIGNAL(broadcast_midi_message(MidiMessage)), this, SLOT(handle_midi_message(MidiMessage)));
	}
}
void PluginWindow::connect_midi_message_handler() const
{
	/**
	 * Disconnects all midi agents from the ui message handler, to ensure only one device is connected to the UI at a time
	 */
	disconnect_midi_message_handler();

	auto devicemanager = GetDeviceManager();

	const auto MAdevice = devicemanager->get_midi_device(ui->list_midi_dev->currentItem()->text());
	connect(MAdevice, SIGNAL(broadcast_midi_message(MidiMessage)), this,
		SLOT(handle_midi_message(MidiMessage))); /// name, mtype, norc, channel
}
void PluginWindow::on_device_select(const QString &curitem) const
{
	if (!starting) {
		blog(LOG_DEBUG, "on_device_select %s", curitem.qtocs());
		auto devicemanager = GetDeviceManager();
		auto config = GetConfig();
		MidiAgent *MAdevice = devicemanager->get_midi_device(curitem);
		set_configure_title(curitem);
		// Pull info on if device is enabled, if so set true if not set false
		try {
			if (MAdevice != NULL && MAdevice->isEnabled()) {
				ui->check_enabled->setChecked(true);
				connect_midi_message_handler();
			} else {
				ui->check_enabled->setChecked(false);
			}
			/// HOOK up the Message Handler
			ui->mapping_lbl_device_name->setText(curitem);
		} catch (...) {
		}
	}
}
void PluginWindow::set_configure_title(const QString &title) const
{
	ui->tabWidget->setTabText(1, QString("Configure - ").append(title));
}
void PluginWindow::handle_midi_message(const MidiMessage &mess) const
{
	if (ui->tabWidget->currentIndex() != 1)
		return;

	if (ui->btn_Listen_one->isChecked() || ui->btn_Listen_many->isChecked()) {
		blog(1, "got midi message via gui, \n Device = %s \n MType = %s \n NORC : %i \n Channel: %i \n Value: %i", mess.device_name.qtocs(),
		     mess.message_type.qtocs(), mess.NORC, mess.channel, mess.value);
		ui->mapping_lbl_device_name->setText(mess.device_name);
		ui->sb_channel->setValue(mess.channel);
		ui->sb_norc->setValue(mess.NORC);
		ui->slider_value->setValue(mess.value);
		ui->cb_mtype->setCurrentText(mess.message_type);
		ui->btn_Listen_one->setChecked(false);
	}
}

PluginWindow::~PluginWindow()
{
	delete ui;
}

void PluginWindow::reset_to_defaults() const
{
	load_table();
}
void PluginWindow::clear_actions_box(QLayout *layout) const
{
	if (layout) {
		QLayoutItem *item;
		while ((item = layout->takeAt(0))) {
			if (item->layout()) {
				clear_actions_box(item->layout());
				delete item->layout();
			}
			if (item->widget()) {
				delete item->widget();
			}
		}
		delete layout;
	}
}

void PluginWindow::obs_actions_select(const QString &action) const
{
	//clear_actions_box(ui->box_action->layout());
	Actions *AC = Actions::make_action(Utils::untranslate(action));
	ui->box_action->setLayout(AC->set_widgets());
	ui->btn_reset->setEnabled(true);
} 
void PluginWindow::set_edit_mode() {}
void PluginWindow::save_edit() {}

void PluginWindow::add_new_mapping() {
	WizardWindow *wizard = new WizardWindow(this);
	wizard->show();
}

bool PluginWindow::map_exists() const
{
	auto devicemanager = GetDeviceManager();
	const auto hooks = devicemanager->get_midi_hooks(ui->mapping_lbl_device_name->text());
	return false;
}

void PluginWindow::set_all_cell_colors(const int row) const
{
	const QColor midi_color(0, 170, 255);
	const QColor action_color(170, 0, 255);

}
void PluginWindow::add_row_from_hook(const MidiMapping *hook) const
{
	if (hook == nullptr)
		return;
	ui->list_mapping->addItem(hook->actions->get_action_string());
}
void PluginWindow::load_table() const
{
	ui->list_mapping->clear();
	const auto hooks = GetDeviceManager()->get_midi_hooks(ui->mapping_lbl_device_name->text());
	if (hooks.count() > 0) {
		for (auto *hook : hooks) {
			add_row_from_hook(hook);
		}
	}
}
void PluginWindow::tab_changed(const int tab) const
{
	
	if (tab == 1) {
		ui->mapping_lbl_device_name->setText(ui->list_midi_dev->currentItem()->text());
		Utils::build_hotkey_map();
	}
	reset_to_defaults();

	// this->ui->table_mapping->resizeColumnsToContents();
}
void PluginWindow::table_select(int selection) {
	const auto hooks = GetDeviceManager()->get_midi_hooks(ui->mapping_lbl_device_name->text());
	auto hook = hooks.at(selection);
	midi_message_select(hook);
	clear_actions_box(ui->box_action->layout());
	ui->box_action->setLayout(hook->actions->set_widgets());
	ui->btn_reset->setEnabled(true);
}

void PluginWindow::midi_message_select(MidiMapping *hook)
{
	ui->sb_channel->setValue(hook->channel);
	ui->sb_norc->setValue(hook->norc);
	ui->cb_mtype->setCurrentText(hook->message_type);
}

void PluginWindow::clear_table() const
{

} 

void PluginWindow::remove_hook(MidiMapping *hook) const
{
	GetDeviceManager()->get_midi_device(ui->mapping_lbl_device_name->text())->remove_MidiMapping(hook);
	GetConfig()->Save();
}
void PluginWindow::delete_mapping() const
{

	reset_to_defaults();
}
void PluginWindow::edit_mapping()
{

}

bool PluginWindow::verify_mapping() const
{
	return true;
}
