#include "beatmap.h"
#include <string>
#include <regex>
#include "column.h"
#include "../message_box.h"
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"

lm::Beatmap *lm::Beatmap::m_instance = 0;

void lm::Beatmap::load(std::string path)
{
	note_duration = 650;
	offset = -200;

	std::string text;
	ReadFile(path, text);
	std::regex note_pattern("(\\d+),\\d+,(\\d+),(\\d+),\\d+,(\\d+):\\d+:\\d+:\\d+:(\\d+:)?");
	bool is_mapped = false;

	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), note_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch line = *i;
		Note *new_note = new Note;
		new_note->time = atoi(std::regex_replace(line.str(), note_pattern, "$2").c_str()) + 2000;
		int type = atoi(std::regex_replace(line.str(), note_pattern, "$3").c_str());
		new_note->time_end = (type % 16 == 0) ? atoi(std::regex_replace(line.str(), note_pattern, "$4").c_str()) + 2000 : new_note->time;
		int column_index = atoi(std::regex_replace(line.str(), note_pattern, "$1").c_str());
		if (!is_mapped)
		{
			switch (column_index)
			{
				case 64:
				case 192:
				case 320:
				case 448:
					for (int i = 0; i < 4; i++)
					{
						Column *new_column = new Column;
						int w = System::instance()->GetWindowWidth() / 4;
						new_column->init(w * i, w);
						m_column.push_back(new_column);
					}
					column_mapper[64] = 0;
					column_mapper[192] = 1;
					column_mapper[320] = 2;
					column_mapper[448] = 3;
				break;
			}
			is_mapped = true;
		}
		m_column[column_mapper[column_index]]->AddNote(new_note);
	}
	Timer::instance()->RunTimer("game");
}

void lm::Beatmap::clear()
{
	m_column.clear();
}

void lm::Beatmap::update()
{
	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->update();
	}
}

void lm::Beatmap::render()
{
	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->render();
	}
}

lm::Judgement lm::Beatmap::judge(int note_time, bool is_pressed, bool is_ln_pressing)
{
	int time_diff = note_time - Timer::instance()->GetTime("game");
	if (is_pressed)
	{
		if (time_diff > 1000 && !is_ln_pressing)
		{
			return JUDGEMENT_NONE;
		}
		else if (time_diff > 1000 && is_ln_pressing)
		{
			score += JUDGEMENT_ER;
			combo = 0;
			MessageBox::instance()->SetText("Error");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 500 || time_diff < -500)
		{
			score += JUDGEMENT_ER;
			combo = 0;
			MessageBox::instance()->SetText("Error");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 200 || time_diff < -200)
		{
			score += JUDGEMENT_GD;
			combo++;
			MessageBox::instance()->SetText("Good");
			return JUDGEMENT_GD;
		}
		else if (time_diff > 100 || time_diff < -100)
		{
			score += JUDGEMENT_GR;
			combo++;
			MessageBox::instance()->SetText("Great");
			return JUDGEMENT_GR;
		}
		else
		{
			score += JUDGEMENT_PG;
			combo++;
			MessageBox::instance()->SetText("Pure");
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -1000)
	{
		score += JUDGEMENT_ER;
		combo = 0;
		MessageBox::instance()->SetText("Error");
		return JUDGEMENT_ER;
	}
	return JUDGEMENT_NONE;
}	//Judgement lm::Beatmap::judge()

int lm::Beatmap::GetDuration()
{
	return note_duration;
}

int lm::Beatmap::GetOffset()
{
	return offset;
}