#include "tilemap/tileset.h"

Tileset::Tileset(const char* path, SDLOO::Renderer& renderer,
				 int cols, int tilewidth, int tileheight) :

	m_source{path, renderer},
	m_cols(cols), m_tilewidth(tilewidth), m_tileheight(tileheight) {}

SDL_Rect Tileset::getTile(int lid) const {

	return {(lid % m_cols) * m_tilewidth, (lid / m_cols) * m_tileheight, m_tilewidth, m_tileheight};

}

const SDLOO::Texture& Tileset::source() const {

	return m_source;

}

int Tileset::tilewidth() const {

	return m_tilewidth;

}

int Tileset::tileheight() const {

	return m_tileheight;

}

int Tileset::columns() const {

	return m_cols;

}