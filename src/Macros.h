#pragma once

#include <qobject.h>
#include "forms/settings-dialog.h"
#include "utils.h"
class Macros :
    public QObject
{
	Macros();
	~Macros();
	void show_pair(Pairs pair);
	void hide_pair(Pairs pair);
	void hide_all_pairs();

private:
	Ui::PluginWindow *ui;
};

