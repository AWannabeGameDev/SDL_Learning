#ifndef OBJECT_LAYER_H
#define OBJECT_LAYER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "map_object.h"

class ObjectLayer {

private:

	std::vector<MapObjectContainer> m_objects;
	std::unordered_map<std::string, size_t> m_nameMap;

public:

	template<MapObjectType type, typename... ArgTypes>
	MapObjectContainer& addObject(const std::string& name, ArgTypes&&... args) {

		MapObjectContainer& ret = m_objects.emplace_back(in_place_value<type>, std::forward<ArgTypes>(args)...);
		if(name.size() != 0) m_nameMap[name] = m_objects.size() - 1;
		return ret;

	}

	size_t size() const;

	MapObjectContainer& getObject(size_t index);
	MapObjectContainer& getObject(const std::string& name);

};

#endif