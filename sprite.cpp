#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"

void lm::Sprite::init(std::string path, int x, int y, int w, int h)
{
	TextureManager::instance()->load(path, m_w, m_h);
	frame.push_back(path);
	m_x = x;
	m_y = y;
	if (w != 0 || h != 0)
	{
		m_w = w;
		m_h = h;
	}
	scale = 1;
	base_index = 0;
	start_time = 0;
	duration = 0;
	current_index = 0;
}

void lm::Sprite::update()
{
	if (duration != 0)
	{
		current_index = (frame_end - frame_start) != 0 ? ((Timer::instance()->GetSystemTime() - start_time) / duration % (frame_end + 1 - frame_start) + frame_start) : frame_start;
		if (Timer::instance()->GetSystemTime() - start_time >= duration)
		{
			duration = 0;
			start_time = 0;
		}
	}
	else
	{
		current_index = base_index;
	}
}

void lm::Sprite::render()
{
	TextureManager::instance()->render(frame[current_index], m_x, m_y, m_w * scale, m_h * scale);
}

void lm::Sprite::clear()
{
//	TextureManager::instance()->clear(m_path);
}

void lm::Sprite::AddFrame(std::string path)
{
	int temp_w, temp_h;
	TextureManager::instance()->load(path, temp_w, temp_h);
	frame.push_back(path);
}

void lm::Sprite::SetAnimate(int load_start, int load_end, int load_duration)
{
	frame_start = load_start;
	frame_end = load_end;
	duration = load_duration;
	start_time = Timer::instance()->GetSystemTime();
}

void lm::Sprite::SetBaseFrame(int index)
{
	base_index = index;
}

void lm::Sprite::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

void lm::Sprite::SetScale(float load_scale)
{
	scale = load_scale;
}

int lm::Sprite::GetX()
{
	return m_x;
}

int lm::Sprite::GetY()
{
	return m_y;
}

int lm::Sprite::GetW()
{
	return m_w;
}

int lm::Sprite::GetH()
{
	return m_h;
}