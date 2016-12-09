#include "Object.h"

#include <fstream>

using namespace Core;

void Object::addPoint(Point newPointCoords)
{
	_vecPathCoords.push_back(newPointCoords);
}

void Object::printPoints()
{
	std::ofstream obj("obiect.txt", std::ofstream::out);
	for (auto &it : _vecPathCoords)
	{
		obj << it._xCoord << ' ' << it._yCoord << '\n';
	}
	obj.close();
}

Object::Object(std::vector<Point> vecPathCoords)
{
	this->_vecPathCoords = vecPathCoords;
	type = "Unknown";
}

Object::~Object()
{
	_vecPathCoords.clear();
}

Object::Object(const Object& object)
{
	this->_vecPathCoords = object._vecPathCoords;
	this->type.assign(object.type);
}

void Object::operator=(const Object& object)
{
	this->_vecPathCoords = object._vecPathCoords;
	this->type.assign(object.type);
}