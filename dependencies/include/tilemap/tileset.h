#ifndef TILESET_H
#define TILESET_H

#include <sdl/SDL.h>
#include <sdl_util/sdloo.h>

class Tileset {

private:

	SDLOO::Texture m_source;
	int m_cols;
	int m_tilewidth, m_tileheight;

public:

	Tileset(const char* path, SDLOO::Renderer& renderer, int cols, int tilewidth, int tileheight);

	SDL_Rect getTile(int lid) const;

	const SDLOO::Texture& source() const;

	int tilewidth() const;
	int tileheight() const;
	int columns() const;

};

#endif