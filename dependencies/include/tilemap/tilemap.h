#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <sdl_util/sdloo.h>
#include "tileset.h"
#include "tile_layer.h"
#include "object_layer.h"

class TileMap {

private :

	class TilesetInfo {

	public:

		int fgid;
		Tileset tileset;

		template<typename... Args>
		TilesetInfo(int fgid, Args&&... args) :

			fgid(fgid), tileset{std::forward<Args>(args)...} {}

	};

	std::vector<TilesetInfo> m_tilesets;
	std::vector<TileLayer> m_tileLayers; std::unordered_map<std::string, size_t> m_tileLayersNameMap;
	std::vector<ObjectLayer> m_objLayers; std::unordered_map<std::string, size_t> m_objLayersNameMap;
	int m_tilewidth, m_tileheight;
	int m_rows, m_cols;

public :

	TileMap(const std::string& path, SDLOO::Renderer& renderer);

	int tilewidth() const;
	int tileheight() const;

	int rows() const;
	int columns() const;

	size_t numberOfTileLayers() const;
	size_t numberOfObjectLayers() const;
	size_t numberOfTilesets() const;

	TileLayer& getTileLayer(size_t index);
	TileLayer& getTileLayer(const std::string& name);

	ObjectLayer& getObjectLayer(size_t index);
	ObjectLayer& getObjectLayer(const std::string& name);

	Tileset& getTileset(size_t index);

	void render(SDLOO::Renderer& renderer, float x, float y, float scale = 1.f);

};

#endif