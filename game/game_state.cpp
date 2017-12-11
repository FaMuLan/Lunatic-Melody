#include "game_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../system.h"
#include "../text_area.h"
#include "../texture_manager.h"
#include "../timer.h"
#include "../song_data.h"
#include "../select/select_state.h"
#include "../select/song_list.h"
#include "../loading/loading_state.h"
#include "../user/character.h"
#include "beatmap.h"

lm::GameState *lm::GameState::m_instance = 0;

void lm::GameState::init()
{
	Beatmap::instance()->load(m_information);

	title_base = new Button;
	score_base = new Sprite;
	score_text = new TextArea;
	wall_l = new Sprite;
	wall_r = new Sprite;
	background_l = new Sprite;
	background_r = new Sprite;
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 56);
	title_base->init("assets/game/title_base.png", System::instance()->GetWindowWidth() / 2 - 360, 0);
	title_base->AddPressedFrame("assets/game/title_base_pressed.png");
	title_base->AddText(m_information->title, title_base->GetW() / 2, title_base->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	score_base->init("assets/game/score_base.png", 0, 64);
	score_text->init("0", 32, 96, "assets/fonts/Audiowide.ttf", 56, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);

	wall_l->init("assets/game/wall_l.png");
	wall_r->init("assets/game/wall_r.png");
	background_l->init("assets/game/background_l.png");
	background_r->init("assets/game/background_r.png");
	background_l->SetSize(327.f * Beatmap::instance()->GetScaleW(), 625.f * Beatmap::instance()->GetScaleH());
	background_r->SetSize(327.f * Beatmap::instance()->GetScaleW(), 625.f * Beatmap::instance()->GetScaleH());
	background_l->SetPos(0, 328 * Beatmap::instance()->GetScaleH());
	background_r->SetPos(System::instance()->GetWindowWidth() - background_r->GetW(), 328 * Beatmap::instance()->GetScaleH());

	wall_l->SetSize(228.f * Beatmap::instance()->GetScaleW(), 584.f * Beatmap::instance()->GetScaleH());
	wall_r->SetSize(228.f * Beatmap::instance()->GetScaleW(), 584.f * Beatmap::instance()->GetScaleH());
}

void lm::GameState::clear()
{
	title_base->clear();
	Beatmap::instance()->clear();
}

void lm::GameState::update()
{
	char *score_ch = new char[6];
	sprintf(score_ch, "%d", Beatmap::instance()->GetScore());
	score_text->SetText(score_ch);
	delete [] score_ch;

	Beatmap::instance()->update();
	if (System::instance()->IsWindowModified())
	{
		title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, 0);
		background_l->SetSize(327.f * Beatmap::instance()->GetScaleW(), 625.f * Beatmap::instance()->GetScaleH());
		background_r->SetSize(327.f * Beatmap::instance()->GetScaleW(), 625.f * Beatmap::instance()->GetScaleH());
		background_l->SetPos(0, 328 * Beatmap::instance()->GetScaleH());
		background_r->SetPos(System::instance()->GetWindowWidth() - background_r->GetW(), 328 * Beatmap::instance()->GetScaleH());
		wall_l->SetSize(228.f * Beatmap::instance()->GetScaleW(), 584.f * Beatmap::instance()->GetScaleH());
		wall_r->SetSize(228.f * Beatmap::instance()->GetScaleW(), 584.f * Beatmap::instance()->GetScaleH());
	}

	DrawWall();
	title_base->update();

	Beatmap::instance()->render();

	score_base->render();
	score_text->render();
	title_base->render();
	if (title_base->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
	}
}

void lm::GameState::SetFile(lm::SongInformation *load_information)
{
	m_information = load_information;
}

void lm::GameState::DrawWall()
{
	static const int start_x = 275;
	static const int start_y = 328;
	static const int end_x = 0;
	static const int end_y = 365;
	static const float start_scale = 0.254386f;
	double process = double(Timer::instance()->GetSystemTime()) / double(Beatmap::instance()->GetDuration());
//	process = process - int(process) + 1;
	double process_sq = process * process * 1.5f;
	background_l->render();
	background_r->render();
	while (process > 0)
	{
		if (process < 1.6f)
		{
			int current_x = start_x + (end_x - start_x) * process_sq;
			int current_y = start_y + (end_y - start_y) * process_sq;
			float current_scale = start_scale + (1.0f - start_scale) * process_sq;
			wall_l->SetPos(current_x * Beatmap::instance()->GetScaleW(), current_y * Beatmap::instance()->GetScaleH());
			wall_l->SetScale(current_scale);
			wall_l->render();
			wall_r->SetPos((System::instance()->GetWindowWidth() - current_x * Beatmap::instance()->GetScaleW() - wall_r->GetW() * current_scale), current_y * Beatmap::instance()->GetScaleH());
			wall_r->SetScale(current_scale);
			wall_r->render();
		}
		process -= 0.54f;
		process_sq = process * process;
	}
}