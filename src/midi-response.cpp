#include "midi-response.h"
MidiResponse::MidiResponse(MidiHook *incoming_hook, const RpcEvent &incoming_event)
{
	hook = incoming_hook;
	event = incoming_event;
	if (incoming_hook != NULL) {
		message = incoming_hook->get_message_from_hook();
		map_events();
	}
}
MidiResponse::~MidiResponse() {}
MidiMessage *MidiResponse::get_message()
{
	return message;
}
void MidiResponse::map_events()
{
	switch (ActionsClass::string_to_event(event.updateType())) {
	case ActionsClass::obs_event_type::SourceDestroyed:
		source_destroyed();
		break;
	case ActionsClass::obs_event_type::SceneChanged:
		scene_changed();
		break;
	case ActionsClass::obs_event_type::SceneListChanged:
		scene_list_changed();
		break;
	case ActionsClass::obs_event_type::SceneCollectionChanged:
		scene_collection_changed();
		break;
	case ActionsClass::obs_event_type::SceneCollectionListChanged:
		scene_collection_list_changed();
		break;
	case ActionsClass::obs_event_type::TransitionChange:
		transition_change();
		break;
	case ActionsClass::obs_event_type::TransitionListChanged:
		break;
	case ActionsClass::obs_event_type::ProfileChanged:
		break;
	case ActionsClass::obs_event_type::ProfileListChanged:
		break;
	case ActionsClass::obs_event_type::StreamStarting:
		stream_starting();
		break;
	case ActionsClass::obs_event_type::StreamStarted:
		stream_started();
		break;
	case ActionsClass::obs_event_type::StreamStopping:
		stream_stopping();
		break;
	case ActionsClass::obs_event_type::StreamStopped:
		stream_stopped();
		break;
	case ActionsClass::obs_event_type::RecordingStarting:
		recording_starting();
		break;
	case ActionsClass::obs_event_type::RecordingStarted:
		recording_started();
		break;
	case ActionsClass::obs_event_type::RecordingStopping:
		recording_stopping();
		break;
	case ActionsClass::obs_event_type::RecordingStopped:
		recording_stopped();
		break;
	case ActionsClass::obs_event_type::RecordingPaused:
		recording_paused();
		break;
	case ActionsClass::obs_event_type::RecordingResumed:
		recording_resumed();
		break;
	case ActionsClass::obs_event_type::ReplayStarting:
		break;
	case ActionsClass::obs_event_type::ReplayStarted:
		break;
	case ActionsClass::obs_event_type::ReplayStopping:
		break;
	case ActionsClass::obs_event_type::ReplayStopped:
		break;
	case ActionsClass::obs_event_type::StudioModeSwitched:
		break;
	case ActionsClass::obs_event_type::PreviewSceneChanged:
		break;
	case ActionsClass::obs_event_type::Exiting:
		break;
	case ActionsClass::obs_event_type::FrontendEventHandler:
		break;
	case ActionsClass::obs_event_type::TransitionBegin:
		break;
	case ActionsClass::obs_event_type::TransitionEnd:
		break;
	case ActionsClass::obs_event_type::TransitionVideoEnd:
		break;
	case ActionsClass::obs_event_type::SourceCreated:
		break;
	case ActionsClass::obs_event_type::SourceDestroy:
		break;
	case ActionsClass::obs_event_type::SourceVolumeChanged:
		break;
	case ActionsClass::obs_event_type::SourceMuteStateChanged:
		break;
	case ActionsClass::obs_event_type::SourceAudioSyncOffsetChanged:
		break;
	case ActionsClass::obs_event_type::SourceAudioMixersChanged:
		break;
	case ActionsClass::obs_event_type::SourceRenamed:
		break;
	case ActionsClass::obs_event_type::SourceFilterAdded:
		break;
	case ActionsClass::obs_event_type::SourceFilterRemoved:
		break;
	case ActionsClass::obs_event_type::SourceFilterVisibilityChanged:
		break;
	case ActionsClass::obs_event_type::SourceFilterOrderChanged:
		break;
	case ActionsClass::obs_event_type::SceneReordered:
		break;
	case ActionsClass::obs_event_type::SceneItemAdd:
		break;
	case ActionsClass::obs_event_type::SceneItemDeleted:
		break;
	case ActionsClass::obs_event_type::SceneItemVisibilityChanged:
		break;
	case ActionsClass::obs_event_type::SceneItemLockChanged:
		break;
	case ActionsClass::obs_event_type::SceneItemTransform:
		break;
	case ActionsClass::obs_event_type::SceneItemSelected:
		break;
	case ActionsClass::obs_event_type::SceneItemDeselected:
		break;
	}
}
void MidiResponse::source_destroyed() {}
void MidiResponse::scene_changed() {}
void MidiResponse::scene_list_changed() {}
void MidiResponse::scene_collection_changed() {}
void MidiResponse::scene_collection_list_changed() {}
void MidiResponse::transition_change() {}
void MidiResponse::transition_list_changed() {}
void MidiResponse::profile_changed() {}
void MidiResponse::profile_list_changed() {}
void MidiResponse::stream_starting() {}
void MidiResponse::studio_mode_switched() {}
void MidiResponse::preview_scene_changed()
{
	message->message_type = "Note Off";
	message->channel = hook->channel;
	message->NORC = last_preview_scene_norc;
	message->value = 0;
	this->send_message_to_midi_device(message->get());
	message->NORC = hook->norc;
	message->message_type = "Note On";
	message->value = 1;
	this->send_message_to_midi_device(message->get());
	last_preview_scene_norc = hook->norc;
}
void MidiResponse::exiting()
{
	closing = true;
}
void MidiResponse::frontend_event_handler() {}
void MidiResponse::transition_begin() {}
void MidiResponse::transition_end() {}
void MidiResponse::transition_video_end() {}
void MidiResponse::source_created() {}
void MidiResponse::source_destroy()
{
	if (!closing) {
		QString from = obs_data_get_string(event.additionalFields(), "sourceName");
		this->remove_MidiHook(hook);
		this->GetData();
		GetConfig()->Save();
	}
}
void MidiResponse::source_volume_changed()
{
	double vol = obs_data_get_double(event.additionalFields(), "volume");
	uint8_t newvol = Utils::mapper2(cbrt(vol));
	message = hook->get_message_from_hook();
	message->value = newvol;
	message->get();
}
void MidiResponse::source_mute_state_changed()
{
	message->value = obs_data_get_bool(event.additionalFields(), "muted");
	message->message_type = "Note On";
	message->channel = hook->channel;
	message->NORC = hook->norc;
}
void MidiResponse::source_audio_sync_offset_changed() {}
void MidiResponse::stream_started()
{
	message->message_type = "Note On";
	message->channel = hook->channel;
	message->NORC = hook->norc;
	message->value = 2;
}
void MidiResponse::stream_stopping()
{
	message->message_type = "Note On";
	message->channel = hook->channel;
	message->NORC = hook->norc;
	message->value = 2;
}
void MidiResponse::stream_stopped()
{
	message->message_type = "Note Off";
	message->channel = hook->channel;
	message->NORC = hook->norc;
	message->value = 0;
}
void MidiResponse::recording_starting() {}
void MidiResponse::recording_started()
{
	message->message_type = "Note On";
	message->channel = hook->channel;
	message->NORC = hook->norc;
	message->value = 2;
}
void MidiResponse::recording_stopping()
{
	message->message_type = "Note On";
	message->channel = hook->channel;
	message->NORC = hook->norc;
	message->value = 2;
}
void MidiResponse::recording_stopped()
{
	message->message_type = "Note Off";
	message->channel = hook->channel;
	message->NORC = hook->norc;
	message->value = 0;
}
void MidiResponse::recording_paused() {}
void MidiResponse::recording_resumed() {}
void MidiResponse::source_audio_mixers_changed() {}
void MidiResponse::source_renamed() {}
void MidiResponse::source_filter_added() {}
void MidiResponse::source_filter_removed() {}
void MidiResponse::source_filter_visibility_changed() {}
void MidiResponse::source_filter_order_changed() {}
void MidiResponse::scene_reordered() {}
void MidiResponse::scene_item_add() {}
void MidiResponse::scene_item_deleted() {}
void MidiResponse::scene_item_visibility_changed() {}
void MidiResponse::scene_item_lock_changed() {}
void MidiResponse::scene_item_transform() {}
void MidiResponse::scene_item_selected() {}
void MidiResponse::scene_item_deselected() {}
void MidiResponse::replay_starting() {}
void MidiResponse::replay_started() {}
void MidiResponse::replay_stopping() {}
void MidiResponse::replay_stopped() {}
