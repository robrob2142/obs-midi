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
#include "midi_mapping_wizard.h"

#include "../device-manager.h"
#include "../config.h"
#include "Macros.h"
#include <QListWidget>
#include <qwizard.h>

WizardWindow::WizardWindow(QWidget *parent, QString dn) : QWizard(parent), ui(new Ui::Wizard)
{
	ui->setupUi(this);
	device_name = dn;
	ui->mapping_lbl_device_name->setText(dn);
	ac = new Actions();
	ac->make_map();
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(page_handler(int)));

	connect(this, SIGNAL(finished(int)), this, SLOT(finish_handler(int)));
	connect(ui->btn_reset, SIGNAL(pressed()),this, SLOT(reset_midi_values()));
}
WizardWindow::~WizardWindow()
{
	disconnect_midi_message_handler();
}
void WizardWindow::disconnect_midi_message_handler() const
{
	auto devicemanager = GetDeviceManager();
	auto devices = devicemanager->get_active_midi_devices();
	for (auto device : devices) {
		disconnect(device, SIGNAL(broadcast_midi_message(MidiMessage)), this, SLOT(handle_midi_message(MidiMessage)));
	}
}
void WizardWindow::connect_midi_message_handler() const
{
	/**
	 * Disconnects all midi agents from the ui message handler, to ensure only one device is connected to the UI at a time
	 */
	disconnect_midi_message_handler();

	auto devicemanager = GetDeviceManager();

	const auto MAdevice = devicemanager->get_midi_device(device_name);
	connect(MAdevice, SIGNAL(broadcast_midi_message(MidiMessage)), this,
		SLOT(handle_midi_message(MidiMessage))); /// name, mtype, norc, channel
}

void WizardWindow::handle_midi_message(const MidiMessage &mess) const
{


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

void WizardWindow::setup_actions() const
{
	ui->cb_obs_output_action->clear();
	ui->cb_obs_output_action->addItems(Utils::TranslateActions());
	ui->cb_obs_output_action->setCurrentIndex(1);
	ui->cb_obs_output_action->setCurrentIndex(0);
	connect(ui->cb_obs_output_action, SIGNAL(currentTextChanged(QString)), this, SLOT(select_actions(QString)));
}
void WizardWindow::reset_buttons() {
	ui->btn_Listen_many->setChecked(false);
	ui->btn_Listen_one->setChecked(false);
}
void WizardWindow::reset_midi_values()
{

	ui->sb_channel->setValue(0);
	ui->sb_norc->setValue(0);
	ui->slider_value->setValue(0);
	ui->cb_mtype->setCurrentIndex(0);
	ui->btn_Listen_one->setChecked(false);
}
void WizardWindow::page_handler(int page)
{
	blog(LOG_DEBUG, "page changed to %i", page);
	switch (page) {
	case 0:
		reset_buttons();
		connect_midi_message_handler();
		break;
	case 1:
		disconnect_midi_message_handler();
		setup_actions();
		break;
	}
}
void WizardWindow::finish_handler(int state) {
	blog(LOG_DEBUG, "Finished with state");
	switch (state) {
	case 0:
		blog(LOG_DEBUG, "canceled");
		break;
	case 1:
		blog(LOG_DEBUG, "finished");
		break;

	case 2:
		blog(LOG_DEBUG, "%i", state);
		break;
	}
	disconnect_midi_message_handler();
}
void WizardWindow::select_actions(QString action) {
	blog(LOG_DEBUG, "Action Selected, %s", action.toStdString().c_str());
	clear_actions_box(ui->box_action->layout());
	auto tempaction = ac->make_action(Utils::untranslate(action));
	ui->box_action->setLayout(tempaction->set_widgets());
}

void WizardWindow::clear_actions_box(QLayout *layout) const
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
