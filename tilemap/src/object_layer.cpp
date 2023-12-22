#include "tilemap/object_layer.h"

// addObject is a template function, defined in the header

size_t ObjectLayer::size() const {

	return m_objects.size();

}

MapObjectContainer& ObjectLayer::getObject(size_t index) {

	return m_objects[index];

}

MapObjectContainer& ObjectLayer::getObject(const std::string& name) {

	return m_objects[m_nameMap.at(name)];
	
}
