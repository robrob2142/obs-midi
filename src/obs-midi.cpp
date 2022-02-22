#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>

#include <iostream>
#include <obs-module.h>
#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif
#include "libremidi/libremidi.hpp"
#include <obs-data.h>
#include <utility>

#include "obs-midi.h"

#include "src/forms/settings-dialog.h"
#include "config.h"
#include "device-manager.h"

#include "events.h"
using namespace std;

void ___data_item_dummy_addref(obs_data_item_t *) {}
void ___data_item_release(obs_data_item_t *dataItem)
{
	obs_data_item_release(&dataItem);
}

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-midi", "en-US")
ConfigPtr _config;
DeviceManagerPtr _deviceManager;
eventsPtr _eventsSystem;
bool obs_module_load(void)
{
	blog(LOG_INFO, "MIDI LOADED! :)");
	blog(LOG_INFO, "obs-midi version %s", GIT_TAG);
	qRegisterMetaType<MidiMessage>();
	_eventsSystem = eventsPtr(new Events());
	_deviceManager = DeviceManagerPtr(new DeviceManager());
	_config = ConfigPtr(new Config());
	blog(LOG_DEBUG, "Setup UI");
	auto *mainWindow = (QMainWindow *)obs_frontend_get_main_window();
	plugin_window = new PluginWindow(mainWindow);
	const char *menuActionText = obs_module_text("OBS MIDI Settings");
	auto *menuAction = (QAction *)obs_frontend_add_tools_menu_qaction(menuActionText);
	QObject::connect(menuAction, SIGNAL(triggered()), plugin_window, SLOT(ToggleShowHide()));
	blog(LOG_DEBUG, "OBSMIDI: Setup Complete");
	return true;
}

void obs_module_unload()
{
    _eventsSystem.get()->shutdown();
	_eventsSystem.reset();
	_deviceManager.reset();
	_config.reset();

	blog(LOG_DEBUG, "goodbye!");
}

ConfigPtr GetConfig()
{
	return _config;
}

DeviceManagerPtr GetDeviceManager()
{
	return _deviceManager;
}

eventsPtr GetEventsSystem()
{
	return _eventsSystem;
}
