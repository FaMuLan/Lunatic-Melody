#include "mod_widget.h"
#include "../sprite.h"
#include "../gui/button.h"
#include "../gui/text_area.h"
#include "../animator.h"
#include "../system.h"
#include "../user/setting.h"
#include "prepare_state.h"
#include "../loading/loading_state.h"
#include "../control_handler.h"

fr::ModWidget *fr::ModWidget::m_instance = 0;

void fr::ModWidget::init()
{
	widget_base = new Sprite;

	auto_switch = new Button;
	slide_out_switch = new Button;
	speed_left = new Button;
	speed_left_dual = new Button;
	speed_right = new Button;
	speed_right_dual = new Button;
	offset_left = new Button;
	offset_left_dual = new Button;
	offset_right = new Button;
	offset_right_dual = new Button;
	gameplay_wizard_button = new Button;

	auto_text = new TextArea;
	slide_out_text = new TextArea;
	speed_text = new TextArea;
	offset_text = new TextArea;
	gameplay_wizard_text = new TextArea;
	speed_num = new TextArea;
	offset_num = new TextArea;

	widget_base->init("assets/base/widget_base.png");
	auto_switch->init("assets/base/sort_button.png");
	auto_switch->AddPressedFrame( "assets/base/sort_button_pressed.png");
	auto_switch->AddText(Setting::instance()->IsAuto() ? "ON" : "OFF", Point2Di(auto_switch->GetW() / 2, auto_switch->GetH() / 2), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
	slide_out_switch->init("assets/base/sort_button.png");
	slide_out_switch->AddPressedFrame( "assets/base/sort_button_pressed.png");
	slide_out_switch->AddText(Setting::instance()->IsSlideOut() ? "ON" : "OFF", Point2Di(slide_out_switch->GetW() / 2, slide_out_switch->GetH() / 2), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
	speed_left->init("assets/base/arrow_left.png");
	speed_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	speed_left_dual->init("assets/base/arrow_left_dual.png");
	speed_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	speed_right->init("assets/base/arrow_right.png");
	speed_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	speed_right_dual->init("assets/base/arrow_right_dual.png");
	speed_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	offset_left->init("assets/base/arrow_left.png");
	offset_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	offset_left_dual->init("assets/base/arrow_left_dual.png");
	offset_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	offset_right->init("assets/base/arrow_right.png");
	offset_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	offset_right_dual->init("assets/base/arrow_right_dual.png");
	offset_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	gameplay_wizard_button->init("assets/base/sort_button.png");
	gameplay_wizard_button->AddPressedFrame("assets/base/sort_button_pressed.png");
	gameplay_wizard_button->AddText("START", Point2Di(gameplay_wizard_button->GetW() / 2, gameplay_wizard_button->GetH() / 2), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));

	auto_text->init("AUTO", Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	slide_out_text->init("SLIDE OUT", Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	speed_text->init("SPEED", Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	offset_text->init("OFFSET", Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	gameplay_wizard_text->init("GAMEPLAY WIZARD", Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	char *speed_ch = new char[4];
	char *offset_ch = new char[5];
	sprintf(speed_ch, "%d", Setting::instance()->GetSpeed());
	sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
	speed_num->init(speed_ch, Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
	offset_num->init(offset_ch, Point2Di(0, 0), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
	delete [] speed_ch;
	delete [] offset_ch;

	is_shown = false;
	is_entered = false;
	is_exited = true;
	Animator::instance()->AddAnimation("mod_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("mod_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
}

void fr::ModWidget::clear()
{
	auto_text->clear();
	auto_switch->clear();
	slide_out_text->clear();
	slide_out_switch->clear();
	speed_text->clear();
	speed_left->clear();
	speed_left_dual->clear();
	speed_right->clear();
	speed_right_dual->clear();
	speed_num->clear();
	offset_text->clear();
	offset_left->clear();
	offset_left_dual->clear();
	offset_right->clear();
	offset_right_dual->clear();
	offset_num->clear();
	gameplay_wizard_text->clear();
	gameplay_wizard_button->clear();
	delete auto_text;
	delete auto_switch;
	delete slide_out_text;
	delete slide_out_switch;
	delete speed_text;
	delete speed_left;
	delete speed_left_dual;
	delete speed_right;
	delete speed_right_dual;
	delete speed_num;
	delete offset_text;
	delete offset_left;
	delete offset_left_dual;
	delete offset_right;
	delete offset_right_dual;
	delete offset_num;
	delete gameplay_wizard_text;
	delete gameplay_wizard_button;
}

void fr::ModWidget::update()
{
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() - widget_base->GetH());
			auto_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
			slide_out_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 136);
			speed_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
			speed_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
			speed_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
			speed_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
			offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
			offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
			offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
			offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
			gameplay_wizard_button->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 448);
			auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
			slide_out_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
			speed_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
			offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
			gameplay_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 466);
			speed_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
			offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);
		}
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (!load_finger.moved)
			{
				if (load_finger.x < System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 || load_finger.x > System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2 || load_finger.y < System::instance()->GetWindowHeigh() - widget_base->GetH())
				{
					SwitchShown();
				}
			}
		}
		if (ControlHandler::instance()->IsKeyDown(SDL_SCANCODE_AC_BACK))
		{
			SwitchShown();
		}

		auto_switch->update();
		slide_out_switch->update();
		speed_left->update();
		speed_right->update();
		speed_left_dual->update();
		speed_right_dual->update();
		offset_left->update();
		offset_right->update();
		offset_left_dual->update();
		offset_right_dual->update();
		gameplay_wizard_button->update();

		if (auto_switch->IsReleased())
		{
			Setting::instance()->SwitchAuto();
			auto_switch->ClearText();
			auto_switch->AddText(Setting::instance()->IsAuto() ? "ON" : "OFF", Point2Di(auto_switch->GetW() / 2, auto_switch->GetH() / 2), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
		}

		if (slide_out_switch->IsReleased())
		{
			Setting::instance()->SwitchSlideOut();
			slide_out_switch->ClearText();
			slide_out_switch->AddText(Setting::instance()->IsSlideOut() ? "ON" : "OFF", Point2Di(slide_out_switch->GetW() / 2, slide_out_switch->GetH() / 2), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
		}

		if (speed_left->IsReleased())
		{
			Setting::instance()->SetSpeed(Setting::instance()->GetSpeed() - 1);
			char *speed_ch = new char[4];
			sprintf(speed_ch, "%d", Setting::instance()->GetSpeed());
			speed_num->SetText(speed_ch);
			delete [] speed_ch;
		}
		if (speed_right->IsReleased())
		{
			Setting::instance()->SetSpeed(Setting::instance()->GetSpeed() + 1);
			char *speed_ch = new char[4];
			sprintf(speed_ch, "%d", Setting::instance()->GetSpeed());
			speed_num->SetText(speed_ch);
			delete [] speed_ch;
		}
		if (speed_left_dual->IsReleased())
		{
			Setting::instance()->SetSpeed(Setting::instance()->GetSpeed() - 10);
			char *speed_ch = new char[4];
			sprintf(speed_ch, "%d", Setting::instance()->GetSpeed());
			speed_num->SetText(speed_ch);
			delete [] speed_ch;
		}
		if (speed_right_dual->IsReleased())
		{
			Setting::instance()->SetSpeed(Setting::instance()->GetSpeed() + 10);
			char *speed_ch = new char[4];
			sprintf(speed_ch, "%d", Setting::instance()->GetSpeed());
			speed_num->SetText(speed_ch);
			delete [] speed_ch;
		}

		if (offset_left->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() - 1);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}
		if (offset_right->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() + 1);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}
		if (offset_left_dual->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() - 10);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}
		if (offset_right_dual->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() + 10);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}

		if (gameplay_wizard_button->IsReleased())
		{
			LoadingState::instance()->init(STATE_GAMEPLAY_WIZARD);
		}
	}
	else if (is_shown)
	{
		OnEnter();
	}
	else if (!is_shown && !is_exited)
	{
		OnExit();
	}
}

void fr::ModWidget::render()
{
	if (!is_exited)
	{
		widget_base->render();
		auto_text->render();
		slide_out_text->render();
		speed_text->render();
		offset_text->render();
		gameplay_wizard_text->render();
		auto_switch->render();
		slide_out_switch->render();
		speed_left->render();
		speed_right->render();
		speed_left_dual->render();
		speed_right_dual->render();
		offset_left->render();
		offset_right->render();
		offset_left_dual->render();
		offset_right_dual->render();
		gameplay_wizard_button->render();
		speed_num->render();
		offset_num->render();
	}
}

void fr::ModWidget::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("mod_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("mod_exit");
	}
}

bool fr::ModWidget::IsShown()
{
	return is_shown || !is_exited;
}

void fr::ModWidget::OnEnter()
{
	int widget_x = -widget_base->GetW() + (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("mod_enter");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	auto_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
	slide_out_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 136);
	speed_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
	speed_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
	speed_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
	speed_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
	offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
	offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
	offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
	offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
	gameplay_wizard_button->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 448);
	auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	slide_out_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
	speed_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
	offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
	gameplay_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 466);
	speed_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
	offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);
	if (Animator::instance()->IsTimeUp("mod_enter"))
	{
		Animator::instance()->ResetAnimation("mod_enter");
		is_entered = true;
	}
}

void fr::ModWidget::OnExit()
{
	int widget_x = System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 - (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("mod_exit");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	auto_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
	slide_out_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 136);
	speed_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
	speed_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
	speed_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
	speed_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
	offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
	offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
	offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
	offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
	gameplay_wizard_button->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 448);
	auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	slide_out_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
	speed_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
	offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
	gameplay_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 466);
	speed_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
	offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);
	if (Animator::instance()->IsTimeUp("mod_exit"))
	{
		Animator::instance()->ResetAnimation("mod_exit");
		is_entered = false;
		is_exited = true;
	}
}