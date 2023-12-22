#include "tilemap/tile_layer.h"

// TileInfo ctor is a template function, defined in the header

// addTile is a template function, defined in the header

TileLayer::TileLayer(int tilewidth, int tileheight) :

	m_tiles{}, m_tilewidth(tilewidth), m_tileheight(tileheight) {}

int TileLayer::tilewidth() const {

	return m_tilewidth;

}

int TileLayer::tileheight() const {

	return m_tileheight;

}

size_t TileLayer::size() const {

	return m_tiles.size();

}

Tile& TileLayer::get(size_t index) {

	return m_tiles[index].tile;

}

void TileLayer::render(SDLOO::Renderer& renderer, float x, float y, float scale) {

	for(TileInfo& info : m_tiles) {

		info.tile.render(renderer,
						 (x + (info.col * m_tilewidth)) * scale,
						 (y + (info.row * m_tileheight)) * scale,
						 scale);

	}

}