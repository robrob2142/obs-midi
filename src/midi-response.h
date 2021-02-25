#include <qobject.h>
#include "utils.h"
#include "rpc/RpcEvent.h"
#include "midi-agent.h"
class MidiResponse : public QObject {
	Q_OBJECT
	// BUTTON ACTIONS

public:
	explicit MidiResponse(MidiHook *incoming_hook, const RpcEvent &incoming_event);
	~MidiResponse();
	MidiMessage *get_message();


	void map_events();
	void source_destroyed();
	void scene_changed();
	void scene_list_changed();
	void scene_collection_changed();
	void scene_collection_list_changed();
	void transition_change();
	void transition_list_changed();
	void profile_changed();
	void profile_list_changed();
	void stream_starting();
	void stream_started();
	void stream_stopping();
	void stream_stopped();
	void recording_starting();
	void recording_started();
	void recording_stopping();
	void recording_stopped();
	void recording_paused();
	void recording_resumed();
	void replay_starting();
	void replay_started();
	void replay_stopping();
	void replay_stopped();
	void studio_mode_switched();
	void preview_scene_changed();
	void exiting();
	void frontend_event_handler();
	void transition_begin();
	void transition_end();
	void transition_video_end();
	void source_created();
	void source_destroy();
	void source_volume_changed();
	void source_mute_state_changed();
	void source_audio_sync_offset_changed();
	void source_audio_mixers_changed();
	void source_renamed();
	void source_filter_added();
	void source_filter_removed();
	void source_filter_visibility_changed();
	void source_filter_order_changed();
	void scene_reordered();
	void scene_item_add();
	void scene_item_deleted();
	void scene_item_visibility_changed();
	void scene_item_lock_changed();
	void scene_item_transform();
	void scene_item_selected();
	void scene_item_deselected();

private:
	MidiHook *hook;
	RpcEvent event;
	MidiMessage *message;
	obs_data_t *data;
	bool closing = false;
};
