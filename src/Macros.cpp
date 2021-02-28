#include "Macros.h"


Macros::Macros()
{
	hide_all_pairs();
}
Macros::~Macros() {

}


void Macros::show_pair(Pairs Pair)
{
	switch (Pair) {
	case Pairs::Scene:
		ui->label_obs_output_scene_2->show();
		ui->cb_obs_output_scene_2->show();
		ui->cb_obs_output_scene->addItems(Utils::get_scene_names());
		ui->w_scene_2->show();
		break;
	case Pairs::Source:
		ui->label_obs_output_source_2->show();
		ui->cb_obs_output_source_2->show();
		Utils::get_source_names(ui->cb_obs_output_scene->currentText());
		ui->w_source_2->show();
		break;
	case Pairs::Filter:
		ui->label_obs_output_filter_2->show();
		ui->cb_obs_output_filter_2->show();
		ui->cb_obs_output_filter->addItems(Utils::get_filter_names(ui->cb_obs_output_source->currentText()));
		ui->w_filter_2->show();
		break;
	case Pairs::Transition:
		ui->label_obs_output_transition_2->show();
		ui->cb_obs_output_transition_2->show();
		ui->w_transition_2->show();
		ui->cb_obs_output_transition->addItems(Utils::GetTransitionsList());
		break;
	case Pairs::Item:
		ui->label_obs_output_item_2->show();
		ui->cb_obs_output_item_2->show();
		ui->cb_obs_output_item->addItems(Utils::GetSceneItemsList(ui->cb_obs_output_scene->currentText()));
		ui->w_item_2->show();
		break;
	case Pairs::Audio:
		ui->cb_obs_output_audio_source->clear();
		ui->cb_obs_output_audio_source->addItems(Utils::GetAudioSourceNames());
		ui->label_obs_output_audio_source_2->show();
		ui->cb_obs_output_audio_source_2->show();
		ui->w_audio_2->show();
		break;
	case Pairs::Media:
		ui->cb_obs_output_media_source->clear();
		ui->cb_obs_output_media_source->addItems(Utils::GetMediaSourceNames());
		ui->label_obs_output_media_source_2->show();
		ui->cb_obs_output_media_source_2->show();
		ui->w_media_2->show();
		break;
	case Pairs::String:
		break;
	case Pairs::Boolean:
		break;
	case Pairs::Integer:
		break;
	}
}
void Macros::hide_pair(Pairs Pair)
{
	switch (Pair) {
	case Pairs::Scene:
		ui->label_obs_output_scene_2->hide();
		ui->cb_obs_output_scene_2->hide();
		ui->cb_obs_output_scene->clear();
		ui->w_scene_2->hide();
		blog(LOG_DEBUG, "Hide Scene");
		break;
	case Pairs::Source:
		ui->label_obs_output_source_2->hide();
		ui->cb_obs_output_source_2->hide();
		ui->cb_obs_output_source->clear();
		ui->w_source_2->hide();
		blog(LOG_DEBUG, "Hide Source");
		break;
	case Pairs::Filter:
		ui->label_obs_output_filter_2->hide();
		ui->cb_obs_output_filter_2->hide();
		ui->cb_obs_output_filter->clear();
		ui->w_filter_2->hide();
		blog(LOG_DEBUG, "Hide Filter");
		break;
	case Pairs::Transition:
		ui->label_obs_output_transition_2->hide();
		ui->cb_obs_output_transition_2->hide();
		ui->cb_obs_output_transition->clear();
		ui->w_transition_2->hide();
		blog(LOG_DEBUG, "Hide Transition");
		break;
	case Pairs::Item:
		ui->label_obs_output_item_2->hide();
		ui->cb_obs_output_item_2->hide();
		ui->cb_obs_output_item->clear();
		ui->w_item_2->hide();
		blog(LOG_DEBUG, "Hide Item");
		break;
	case Pairs::Audio:
		ui->label_obs_output_audio_source_2->hide();
		ui->cb_obs_output_audio_source_2->hide();
		ui->cb_obs_output_audio_source->clear();
		ui->w_audio_2->hide();
		blog(LOG_DEBUG, "Hide Audio");
		break;
	case Pairs::Media:
		ui->label_obs_output_media_source_2->hide();
		ui->cb_obs_output_media_source_2->hide();
		ui->cb_obs_output_media_source->clear();
		ui->w_media_2->hide();
		blog(LOG_DEBUG, "Hide Media");
		break;
	case Pairs::String:
		break;
	case Pairs::Boolean:
		break;
	case Pairs::Integer:
		break;
	}
}
void Macros::hide_all_pairs()
{
	hide_pair(Pairs::Transition);
	hide_pair(Pairs::Audio);
	hide_pair(Pairs::Media);
	hide_pair(Pairs::Filter);
	hide_pair(Pairs::Scene);
	hide_pair(Pairs::Source);
	hide_pair(Pairs::Item);
	hide_pair(Pairs::String);
	hide_pair(Pairs::Integer);
	hide_pair(Pairs::Boolean);
}

