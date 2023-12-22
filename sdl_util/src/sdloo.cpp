#include "sdl_util/sdloo.h"

//CLASS TEXTURE ----------------------------------

SDLOO::Texture::Texture(SDL_Texture* texture) :

	m_texture(texture), m_width {}, m_height {}

{

	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_texture, 255);

	SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);

}

SDLOO::Texture::Texture(const char* path, const Renderer& renderer) :

	Texture {IMG_LoadTexture(renderer.m_renderer, path)} {}

int SDLOO::Texture::width() const {

	return m_width;

}

int SDLOO::Texture::height() const {

	return m_height;

}

void SDLOO::Texture::setBlendMode(SDL_BlendMode alpha) {

	SDL_SetTextureBlendMode(m_texture, alpha);

}

SDL_BlendMode SDLOO::Texture::getBlendMode() const {

	SDL_BlendMode mode;
	SDL_GetTextureBlendMode(m_texture, &mode);
	return mode;

}

void SDLOO::Texture::setAlpha(Uint8 alpha) {

	SDL_SetTextureAlphaMod(m_texture, alpha);

}

Uint8 SDLOO::Texture::getAlpha() const {

	Uint8 alpha;
	SDL_GetTextureAlphaMod(m_texture, &alpha);
	return alpha;

}

SDLOO::Texture::~Texture() {

	SDL_DestroyTexture(m_texture);

}

//CLASS WINDOW -----------------------------------

SDLOO::Window::Window(SDL_Window* window) :

	m_window(window) {}

SDLOO::Window::Window(const char* title, int x, int y, int w, int h, Uint32 flags) :

	m_window(SDL_CreateWindow(title, x, y, w, h, flags)) {}

SDLOO::Window::~Window() {

	SDL_DestroyWindow(m_window);

}

//CLASS RENDERER ---------------------------------

SDLOO::Renderer::Renderer(const Window& window, int index, Uint32 flags, float width, float height) :

	m_renderer(SDL_CreateRenderer(window.m_window, index, flags)), 
	m_camera{0.f, 0.f, width, height}, 
	m_viewport{},
	m_winWidth{}, m_winHeight{},
	m_renderScale{} 

{

	updateView();

}

void SDLOO::Renderer::f_updateScale() {

	float widthScale = m_winWidth / m_camera.w;
	float heightScale = m_winHeight / m_camera.h;

	m_renderScale = widthScale <= heightScale ? widthScale : heightScale;

	m_viewport.x = (int)((m_winWidth - (m_camera.w * m_renderScale)) / 2);
	m_viewport.y = (int)((m_winHeight - (m_camera.h * m_renderScale)) / 2);
	m_viewport.w = (int)(m_camera.w * m_renderScale);
	m_viewport.h = (int)(m_camera.h * m_renderScale);

	resetViewport();

}

SDL_FRect SDLOO::Renderer::f_worldToScreenRect(const SDL_FRect& rect) {

	return {(rect.x - m_camera.x) * m_renderScale,
			(rect.y - m_camera.y) * m_renderScale,
			rect.w * m_renderScale,
			rect.h * m_renderScale};

}

SDL_FRect SDLOO::Renderer::f_worldToScreenRect(float x, float y, float w, float h) {

	return {(x - m_camera.x) * m_renderScale,
			(y - m_camera.y) * m_renderScale,
			w * m_renderScale,
			h * m_renderScale};

}

SDL_Rect SDLOO::Renderer::f_worldToScreenRect(const SDL_Rect& rect) {

	return {(int)((rect.x - m_camera.x) * m_renderScale),
			(int)((rect.y - m_camera.y) * m_renderScale),
			(int)(rect.w * m_renderScale),
			(int)(rect.h * m_renderScale)};

}

SDL_Rect SDLOO::Renderer::f_worldToScreenRect(int x, int y, int w, int h) {

	return {(int)((x - m_camera.x) * m_renderScale),
			(int)((y - m_camera.y) * m_renderScale),
			(int)(w * m_renderScale),
			(int)(h * m_renderScale)};

}

void SDLOO::Renderer::clear() {

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);

}

void SDLOO::Renderer::renderPresent() {

	SDL_RenderPresent(m_renderer);

}

void SDLOO::Renderer::renderCopy(const Texture& texture, float x, float y,
				float scale, const std::optional<SDL_Rect>& clip)
{

	SDL_Rect clipRect = clip ? *clip : SDL_Rect{0, 0, texture.m_width, texture.m_height};
	SDL_FRect dstRect = f_worldToScreenRect(x, y, clipRect.w * scale, clipRect.h * scale);

	SDL_RenderCopyF(m_renderer, texture.m_texture, &clipRect, &dstRect);

}

void SDLOO::Renderer::renderCopy(const Texture& texture, const SDL_FRect& dstRect,
				const std::optional<SDL_Rect>& clip)
{

	SDL_Rect clipRect = clip ? *clip : SDL_Rect {0, 0, texture.m_width, texture.m_height};
	SDL_FRect newDstRect = f_worldToScreenRect(dstRect);

	SDL_RenderCopyF(m_renderer, texture.m_texture, &clipRect, &newDstRect);

}

// Order of operations : clipping, flipping / scaling, rotating
void SDLOO::Renderer::renderCopyEx(const Texture& texture, float x, float y, float scale, 
								 float degrees, const std::optional<SDL_FPoint>& center, SDL_RendererFlip flip,
								 const std::optional<SDL_Rect>& clip) 
{

	SDL_Rect clipRect = clip ? *clip : SDL_Rect {0, 0, texture.m_width, texture.m_height};
	SDL_FRect dstRect = f_worldToScreenRect(x, y, clipRect.w * scale, clipRect.h * scale);

	SDL_RenderCopyExF(m_renderer, texture.m_texture, &clipRect, &dstRect, 
					  (double)degrees, center ? &*center : nullptr, flip);

}

//Order of operations : clipping, flipping / scaling, rotating
void SDLOO::Renderer::renderCopyEx(const Texture& texture, const SDL_FRect& dstRect,
								 float degrees, const std::optional<SDL_FPoint>& center, SDL_RendererFlip flip,
								 const std::optional<SDL_Rect>& clip)
{

	SDL_Rect clipRect = clip ? *clip : SDL_Rect {0, 0, texture.m_width, texture.m_height};
	SDL_FRect newDstRect = f_worldToScreenRect(dstRect);

	SDL_RenderCopyExF(m_renderer, texture.m_texture, &clipRect, &newDstRect, 
					  (double)degrees, center ? &*center : nullptr, flip);

}

void SDLOO::Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);

}

void SDLOO::Renderer::setViewport(const SDL_Rect& rect) {

	SDL_Rect viewport = f_worldToScreenRect(rect);
	SDL_RenderSetViewport(m_renderer, &viewport);

}

void SDLOO::Renderer::resetViewport() {

	SDL_RenderSetViewport(m_renderer, &m_viewport);

}

void SDLOO::Renderer::updateView() {

	SDL_GetWindowSize(SDL_RenderGetWindow(m_renderer), &m_winWidth, &m_winHeight);
	f_updateScale();

}

void SDLOO::Renderer::setCameraPosition(float x, float y) {

	m_camera.x = x;
	m_camera.y = y;

}

void SDLOO::Renderer::moveCamera(float dx, float dy) {

	m_camera.x += dx;
	m_camera.y += dy;

}

void SDLOO::Renderer::scaleCameraBy(float scale) {

	m_camera.w *= scale;
	m_camera.h *= scale;
	f_updateScale();

}

void SDLOO::Renderer::setCameraSize(float w, float h) {

	m_camera.w = w;
	m_camera.h = h;
	f_updateScale();

}

const SDL_FRect& SDLOO::Renderer::camera() {

	return m_camera;

}

void SDLOO::Renderer::fillRect(const SDL_FRect& rect) {

	SDL_FRect fillRect = f_worldToScreenRect(rect);
	SDL_RenderFillRectF(m_renderer, &fillRect);

}

void SDLOO::Renderer::drawRect(const SDL_FRect& rect) {

	SDL_FRect drawRect = f_worldToScreenRect(rect);
	SDL_RenderDrawRectF(m_renderer, &drawRect);

}

SDLOO::Renderer::~Renderer() {

	SDL_DestroyRenderer(m_renderer);

}