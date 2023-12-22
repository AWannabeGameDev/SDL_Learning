#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <sdl/SDL.h>
#include <vector>
#include <variant>
#include <string_view>

template <auto v>
class in_place_value_t {

public:

	explicit in_place_value_t() = default;

};

template <auto v>
inline constexpr in_place_value_t<v> in_place_value{};

// -----------------------------------------

constexpr enum MapObjectType {

	RectangleType,
	PolygonType,
	CircleType,
	PointType

};

// ------------------------------------------

struct RectangleMapObject {

	float x, y;
	float width, height;

};

// ------------------------------------------

struct PolygonMapObject {

	std::vector<SDL_FPoint> vertices;

};

// -----------------------------------------

struct CircleMapObject {

	float x, y;
	float radius;

};

// -----------------------------------------

struct PointMapObject {

	float x, y;

};

// ------------------------------------------

class MapObjectContainer {

private:

	std::variant<RectangleMapObject, PolygonMapObject, CircleMapObject, PointMapObject> m_obj;
	MapObjectType m_type;

public:

	template<MapObjectType objType, typename... ArgTypes>
	MapObjectContainer(in_place_value_t<objType>, ArgTypes&&... args) :

		m_obj{std::in_place_index<objType>, std::forward<ArgTypes>(args)...},
		m_type(objType) {}

	template<typename ObjectType>
	ObjectType& get() {

		return std::get<ObjectType>(m_obj);

	}

	MapObjectType type() const;

};

#endif