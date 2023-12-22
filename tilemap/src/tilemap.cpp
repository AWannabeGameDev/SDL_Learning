#include <string>
#include <sstream>
#include <tilemap/tinyxml2.h>
#include "tilemap/tilemap.h"

// TilesetInfo ctor is a template function, defined in the header

TileMap::TileMap(const std::string& mapPath, SDLOO::Renderer& renderer) {

	namespace xml = tinyxml2;

	std::string pathPrefix {mapPath.substr(0, mapPath.find_last_of('/') + 1)};

	xml::XMLDocument map;
	map.LoadFile(mapPath.data());

	xml::XMLElement* mapHeader = map.RootElement();

	m_tilewidth = mapHeader->FindAttribute("tilewidth")->IntValue();
	m_tileheight = mapHeader->FindAttribute("tileheight")->IntValue();
	m_rows = mapHeader->FindAttribute("height")->IntValue();
	m_cols = mapHeader->FindAttribute("width")->IntValue();

	// LOAD TILESET

	for(xml::XMLElement* tilesetElem = mapHeader->FirstChildElement("tileset");
		tilesetElem != nullptr;
		tilesetElem = tilesetElem->NextSiblingElement("tileset"))
	{

		xml::XMLDocument tilesetFile;
		tilesetFile.LoadFile((pathPrefix + tilesetElem->FindAttribute("source")->Value()).c_str());

		xml::XMLElement* tilesetHeader = tilesetFile.RootElement();

		std::string sourcePath = pathPrefix + 
								  tilesetHeader->FirstChildElement("image")->FindAttribute("source")->Value();
		int cols = tilesetHeader->FindAttribute("columns")->IntValue();
		int fgid = tilesetElem->FindAttribute("firstgid")->IntValue();

		m_tilesets.emplace_back(fgid, sourcePath.c_str(), renderer, cols, m_tilewidth, m_tileheight);

	}

	// END LOAD TILESET

	if(mapHeader->FindAttribute("infinite")->IntValue() == 0) {

		// LOAD FINITE MAP

		for(xml::XMLElement* layerElem = mapHeader->FirstChildElement("layer");
			layerElem != nullptr;
			layerElem = layerElem->NextSiblingElement("layer"))
		{

			TileLayer& layer = m_tileLayers.emplace_back(m_tilewidth, m_tileheight);

			const char* nameCstr = layerElem->Attribute("name");
			if(nameCstr) m_tileLayersNameMap[std::string{nameCstr}] = m_tileLayers.size() - 1;

			std::istringstream tileData {layerElem->FirstChildElement("data")->GetText()};
			std::string tileRow;
			std::istringstream tileRowData;
			std::string tile;

			tileData.get();
			std::getline(tileData, tileRow);

			for(int r = 0; r < m_rows; r++, std::getline(tileData, tileRow)) {

				tileRowData = std::istringstream{tileRow};
				std::getline(tileRowData, tile, ',');

				for(int c = 0; c < m_cols; c++, std::getline(tileRowData, tile, ',')) {

					int gid = std::stoi(tile);
					if(gid == 0) continue;

					size_t tilesetIdx = 0;

					for(size_t i = 1; i < m_tilesets.size(); i++) {

						if(m_tilesets[i].fgid > gid) break;
						tilesetIdx++;

					}

					layer.addTile(r, c, m_tilesets[tilesetIdx].tileset, gid - m_tilesets[tilesetIdx].fgid);

				}

			}

		}

		// END LOAD FINITE MAP

	} else { 

		// LOAD INFINITE MAP

		for(xml::XMLElement* layerElem = mapHeader->FirstChildElement("layer");
			layerElem != nullptr;
			layerElem = layerElem->NextSiblingElement("layer"))
		{

			TileLayer& layer = m_tileLayers.emplace_back(m_tilewidth, m_tileheight);

			const char* nameCstr = layerElem->Attribute("name");
			if(nameCstr) m_tileLayersNameMap[std::string{nameCstr}] = m_tileLayers.size() - 1;

			for(xml::XMLElement* chunkElem = layerElem->FirstChildElement("data")->FirstChildElement("chunk");
				chunkElem != nullptr;
				chunkElem = chunkElem->NextSiblingElement("chunk"))
			{

				int chunkX = chunkElem->FindAttribute("x")->IntValue();
				int chunkY = chunkElem->FindAttribute("y")->IntValue();
				int chunkWidth = chunkElem->FindAttribute("width")->IntValue();
				int chunkHeight = chunkElem->FindAttribute("height")->IntValue();

				std::stringstream tileData {chunkElem->GetText()};
				std::string tileRow; 
				std::stringstream tileRowData;
				std::string tile;

				tileData.get();
				std::getline(tileData, tileRow);

				for(int r = 0; r < chunkHeight; r++, std::getline(tileData, tileRow)) {

					tileRowData = std::stringstream{tileRow};
					std::getline(tileRowData, tile, ',');

					for(int c = 0; c < chunkWidth; c++, std::getline(tileRowData, tile, ',')) {

						int gid = std::stoi(tile);
						if(gid == 0) continue;

						size_t tilesetIdx = 0;

						for(size_t i = 1; i < m_tilesets.size(); i++) {

							if(m_tilesets[i].fgid > gid) break;
							tilesetIdx++;

						}

						layer.addTile(chunkY + r, chunkX + c, 
									  m_tilesets[tilesetIdx].tileset, gid - m_tilesets[tilesetIdx].fgid);

					}

				}

			}

		}

		// END LOAD INFNITE MAP
	}

	// LOAD OBJECTS

	for(xml::XMLElement* layerElem = mapHeader->FirstChildElement("objectgroup");
		layerElem != nullptr;
		layerElem = layerElem->NextSiblingElement("objectgroup"))
	{

		ObjectLayer& layer = m_objLayers.emplace_back();

		const char* nameCstr = layerElem->Attribute("name");
		if(nameCstr) m_objLayersNameMap[std::string{nameCstr}] = m_objLayers.size() - 1;
		
		for(xml::XMLElement* objElem = layerElem->FirstChildElement("object");
			objElem != nullptr;
			objElem = objElem->NextSiblingElement("object"))
		{

			nameCstr = objElem->Attribute("name");
			std::string name {nameCstr ? nameCstr : ""};

			// LOAD RECTANGLE

			if(!objElem->FirstChild()) { 

				float x = objElem->FindAttribute("x")->FloatValue();
				float y = objElem->FindAttribute("y")->FloatValue();
				float w = objElem->FindAttribute("width")->FloatValue();
				float h = objElem->FindAttribute("height")->FloatValue();

				layer.addObject<RectangleType>(name, x, y, w, h);
			
			} 
			
			// END LOAD RECTANGLE

			// LOAD POINT

			else if(xml::XMLElement* polyElem = objElem->FirstChildElement("point")) {

				float x = objElem->FindAttribute("x")->FloatValue();
				float y = objElem->FindAttribute("y")->FloatValue();

				layer.addObject<PointType>(name, x, y);

			}

			// END LOAD POINT

			// LOAD POLYGON

			else if(xml::XMLElement* polyElem = objElem->FirstChildElement("polygon")) { 

				PolygonMapObject& obj = layer.addObject<PolygonType>(name).get<PolygonMapObject>();
				float posX = objElem->FindAttribute("x")->FloatValue();
				float posY = objElem->FindAttribute("y")->FloatValue();

				std::istringstream data {polyElem->FirstAttribute()->Value()};
				std::string coordinateStr;
				std::istringstream coordinateStream;
				std::string coordinate;

				while(data.good()) {

					std::getline(data, coordinateStr, ' ');
					coordinateStream = std::istringstream{coordinateStr};

					std::getline(coordinateStream, coordinate, ',');
					float x = std::stof(coordinate) + posX;
					std::getline(coordinateStream, coordinate, ',');
					float y = std::stof(coordinate) + posY;

					obj.vertices.emplace_back(x, y);

 				}

			}

			// END LOAD POLYGON
		}

	}

	// END LOAD OBJECTS
}

int TileMap::tilewidth() const {

	return m_tilewidth;

}

int TileMap::tileheight() const {

	return m_tileheight;

}

int TileMap::rows() const {

	return m_rows;

}

int TileMap::columns() const {

	return m_cols;

}

size_t TileMap::numberOfTileLayers() const {

	return m_tileLayers.size();

}

size_t TileMap::numberOfObjectLayers() const {

	return m_objLayers.size();

}

size_t TileMap::numberOfTilesets() const {

	return m_tilesets.size();

}

TileLayer& TileMap::getTileLayer(size_t index) {

	return m_tileLayers[index];

}

TileLayer& TileMap::getTileLayer(const std::string& name) {

	return m_tileLayers[m_tileLayersNameMap.at(name)];
	
}

ObjectLayer& TileMap::getObjectLayer(size_t index) {

	return m_objLayers[index];

}

ObjectLayer& TileMap::getObjectLayer(const std::string& name) {

	return m_objLayers[m_objLayersNameMap.at(name)];
	
}

Tileset& TileMap::getTileset(size_t index) {

	return m_tilesets[index].tileset;

}

void TileMap::render(SDLOO::Renderer& renderer, float x, float y, float scale) {

	for(TileLayer& layer : m_tileLayers) {

		layer.render(renderer, x, y, scale);

	}

}