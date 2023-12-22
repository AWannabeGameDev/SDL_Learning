#ifndef SDLOO_H
#define SDLOO_H

#include <sdl/SDL.h>
#include <sdl_image/SDL_image.h>
#include <optional>

namespace SDLOO {

	class Texture;
	class Window;
	class Renderer;

	class Texture {

		friend class Renderer;

	private :

		SDL_Texture* m_texture;
		int m_width, m_height;

	public :

		Texture(SDL_Texture* texture);
		Texture(const char* path, const Renderer& renderer);
		
		int width() const;
		int height() const;

		void setBlendMode(SDL_BlendMode alpha);
		SDL_BlendMode getBlendMode() const;

		void setAlpha(Uint8 alpha);
		Uint8 getAlpha() const;

		~Texture();

	};

	class Window {

		friend class Renderer;

	private:

		SDL_Window* m_window;

	public:

		Window(SDL_Window* window);
		Window(const char* title, int x, int y, int w, int h, Uint32 flags);

		~Window();

	};

	class Renderer {

		friend class Texture;

	private :

		SDL_Renderer* m_renderer;
		SDL_FRect m_camera;
		SDL_Rect m_viewport;
		int m_winWidth, m_winHeight;
		float m_renderScale;

		void f_updateScale();

		SDL_FRect f_worldToScreenRect(const SDL_FRect& rect);
		SDL_FRect f_worldToScreenRect(float x, float y, float w, float h);
		SDL_Rect f_worldToScreenRect(const SDL_Rect& rect);
		SDL_Rect f_worldToScreenRect(int x, int y, int w, int h);

	public :

		Renderer(const Window& window, int index, Uint32 flags, float width, float height);

		void clear();

		void renderPresent();

		void renderCopy(const Texture& texture, float x, float y, 
						float scale = 1, const std::optional<SDL_Rect>& clip = {});

		void renderCopy(const Texture& texture, const SDL_FRect& dstRect,
						const std::optional<SDL_Rect>& clip = {});

		// Order of operations : clipping, flipping / scaling, rotating
		void renderCopyEx(const Texture& texture, float x, float y, float scale = 1,
						float degrees = 0, const std::optional<SDL_FPoint>& center = {},
						SDL_RendererFlip flip = SDL_FLIP_NONE,
						const std::optional<SDL_Rect>& clip = {});

		// Order of operations : clipping, flipping / scaling, rotating
		void renderCopyEx(const Texture& texture, const SDL_FRect& dstRect,
						float degrees = 0, const std::optional<SDL_FPoint>& center = {},
						SDL_RendererFlip flip = SDL_FLIP_NONE,
						const std::optional<SDL_Rect>& clip = {});

		void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		void setViewport(const SDL_Rect& rect);
		void resetViewport();

		void updateView();

		void setCameraPosition(float x, float y);
		void moveCamera(float dx, float dy);
		void scaleCameraBy(float scale);
		void setCameraSize(float w, float h);
		const SDL_FRect& camera();


		void fillRect(const SDL_FRect& rect);
		void drawRect(const SDL_FRect& rect);

		~Renderer();

	};

}

#endif