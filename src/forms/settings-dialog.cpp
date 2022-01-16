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
#include "settings-dialog.h"
#include "ui_settings-dialog.h"
#include "../device-manager.h"
#include "../config.h"
#include "Macros.h"
#include <QListWidget>
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
	

	// connect(ui->search_mapping, &QLineEdit::textChanged, devmodel, &QSortFilterProxyModel::setFilterWildcard);
	starting = false;
}

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

	/**************Connections to mappper****************/
	connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(add_new_mapping()));
	connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(reset_to_defaults()));
	connect(ui->btn_delete, SIGNAL(clicked()), this, SLOT(delete_mapping()));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
	connect(ui->list_mapping, &QListWidget::currentRowChanged, this, &PluginWindow::table_select);
}
void PluginWindow::setup_actions() const {}
void PluginWindow::ToggleShowHide()
{
	if (!isVisible()) {
		load_devices();
		ui->tabWidget->setCurrentIndex(0);
		ui->list_midi_dev->setCurrentRow(0);
		setVisible(true);
	} else {
		setVisible(false);
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
	}
	GetConfig()->Save();
}
void PluginWindow::on_device_select(const QString &curitem) 
{
	if (!starting) {
		blog(LOG_DEBUG, "on_device_select %s", curitem.qtocs());
		auto devicemanager = GetDeviceManager();
		auto config = GetConfig();
		MidiAgent *MAdevice = devicemanager->get_midi_device(curitem);
		c_device = curitem;
		set_configure_title(curitem);
		// Pull info on if device is enabled, if so set true if not set false
		try {
			if (MAdevice != NULL && MAdevice->isEnabled()) {
				ui->check_enabled->setChecked(true);
			} else {
				ui->check_enabled->setChecked(false);
			}
			/// HOOK up the Message Handler
		} catch (...) {
		}
	}
}
void PluginWindow::set_configure_title(const QString &title) const
{
	ui->tabWidget->setTabText(1, QString("Configure - ").append(title));
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

}


void PluginWindow::set_edit_mode() {}
void PluginWindow::save_edit() {}

void PluginWindow::add_new_mapping() {
	
	WizardWindow *wizard = new WizardWindow(this, c_device);
	wizard->show();
}

bool PluginWindow::map_exists() const
{
	auto devicemanager = GetDeviceManager();
	const auto hooks = devicemanager->get_midi_hooks(c_device);
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

}
void PluginWindow::tab_changed(const int tab) const
{
	
	if (tab == 1) {
		Utils::build_hotkey_map();
	}
	reset_to_defaults();

}
void PluginWindow::table_select(int selection) {
	
}

void PluginWindow::midi_message_select(MidiMapping *hook)
{
	
}

void PluginWindow::clear_table() const
{

} 

void PluginWindow::remove_hook(MidiMapping *hook) const
{

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
