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
#pragma once
#include <QtWidgets/QDialog>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>
#include <vector>

#include "ui_midi_mapping_wizard.h"
#include "../midi-agent.h"
#include "../version.h"
#include "../Midi_hook.h"
class WizardWindow : public QWizard {
	Q_OBJECT
public:
	explicit WizardWindow(QWidget *parent);
	~WizardWindow() override;

private:
	Ui::Wizard *wiz;


};
