#ifndef TILE_LAYER_H
#define TILE_LAYER_H

#include <vector>
#include "tile.h"
#include <sdl_util/sdloo.h>

class TileLayer {

private:

	class TileInfo {

	public:

		int row, col;
		Tile tile;

		template<typename... Args>
		TileInfo(int row, int col, Args&&... args) :

			row(row), col(col), tile{std::forward<Args>(args)...} {}

	};

	std::vector<TileInfo> m_tiles;
	int m_tilewidth, m_tileheight;

public:

	TileLayer(int tilewidth, int tileheight);

	template<typename... Args>
	void addTile(int row, int col, Args&&... args) {

		m_tiles.emplace_back(row, col, std::forward<Args>(args)...);

	}

	int tilewidth() const;
	int tileheight() const;

	size_t size() const;
	Tile& get(size_t index);

	void render(SDLOO::Renderer& renderer, float x, float y, float scale = 1.f);

};

#endif