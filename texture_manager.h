#ifndef FORCE_RUSH_TEXTURE_MANAGER_H
#define FORCE_RUSH_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

#include "gui/text_area.h"
#include "data.h"

namespace fr
{
	class Rect;
	struct TextureCache
	{
		GLuint *texture;
		int w;
		int h;
	};

	class TextureManager
	{
		public:
			static TextureManager *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new TextureManager;
					return m_instance;
				}
				return m_instance;
			}
			void init(GLuint program_object);
			void load(std::string path, Rect &output_size);
			void load(GLuint *load_texture, std::string path);
			void LoadFont(std::string path, int size);
			void LoadMatrix(std::string id, glm::mat4x4 load_matrix);
			void clear();
			void clear(std::string path);
			void ClearFont(std::string path, int size);
			void ClearMatrix(std::string id);
			void update();
			void render(std::string path, float *load_vectrices, int alpha = 255, std::string matrix_id = "default");
			void render(GLuint *load_texture, float *input_vectrices, int alpha = 255, std::string matrix_id = "default");
			void render(std::string path, float *load_vectrices, int alpha, glm::mat4x4 input_matrix);
			void render(GLuint *load_texture, float *load_vectrices, int alpha, glm::mat4x4 input_matrix);
			TextureCache *CacheText(std::string input_text, std::string input_font_path, int input_font_size, Color input_color, int input_limited_w = 0, bool input_is_wrapped = false);
			//limit_w = 0 = NO_LIMIT
			glm::mat4x4 GetMatrix(std::string id);
			void DestroyCache(TextureCache *cache);
		private: 
			TextureManager() {}
			~TextureManager() {}
			static TextureManager *m_instance;
			std::map<std::string, TextureCache*> texture;
			//保守起见用指针来确认是否加载好
			std::map<std::string, glm::mat4x4> matrix;
			std::map<std::string, std::map<int, TTF_Font*> > font;
			//存储字体
			GLint position_location;
			GLint texture_coord_location;
			GLint sampler_location;
			GLint mvp_location;
			GLint alpha_location;
	};	//class TextureManager
};	//namespace fr

#endif	//FORCE_RUSH_TEXTURE_MANAGER_H