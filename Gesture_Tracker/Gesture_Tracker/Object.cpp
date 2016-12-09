#include "Object.h"

Object::Object(std::vector<Point> vecPathCoords)
{
	this->_vecPathCoords = vecPathCoords;
	type = "Unknown";
}

Object::~Object()
{
	_vecPathCoords.clear();
}

Object::Object(Object &object)
{
	this->_vecPathCoords = object._vecPathCoords;
	this->type.assign(object.type);
}

void Object::operator=(Object &object)
{
	this->_vecPathCoords = object._vecPathCoords;
	this->type.assign(object.type);
}