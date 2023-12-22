#include "tilemap/tile.h"

Tile::Tile(Tileset& tileset, int lid) :

	m_tileset(tileset),
	m_srcRect(tileset.getTile(lid)),
	m_width(tileset.tilewidth()), m_height(tileset.tileheight()) {}

int Tile::width() const {

	return m_width;

}

int Tile::height() const {

	return m_height;

}

Tileset& Tile::tileset() {

	return m_tileset;

}

const SDL_Rect& Tile::sourceRect() const {

	return m_srcRect;

}

void Tile::render(SDLOO::Renderer& renderer, float x, float y, float scale) {

	renderer.renderCopy(m_tileset.source(), x, y, scale, m_srcRect);

}