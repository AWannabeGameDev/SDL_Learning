#ifndef TILE_H
#define TILE_H

#include "tileset.h"
#include <sdl/SDL.h>
#include <sdl_util/sdloo.h>

class Tile {

private:

	Tileset& m_tileset;
	SDL_Rect m_srcRect;
	int m_width, m_height;

public:

	Tile(Tileset& tileset, int lid);

	int width() const;
	int height() const;

	Tileset& tileset();
	const SDL_Rect& sourceRect() const;

	void render(SDLOO::Renderer& renderer, float x, float y, float scale = 1.f);

};

#endif