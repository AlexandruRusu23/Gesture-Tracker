#include "ClosedObject.h"

using namespace Core;

Core::ClosedObject::ClosedObject(const Object & object)
{
	this->_type = object._type;
	this->_vecPathCoords.clear();
	this->_vecPathCoords = object._vecPathCoords;
}

ClosedObject::~ClosedObject()
{
}

void ClosedObject::addObject(const Object& object)
{
	this->_vecPathCoords.clear();
	this->_type.clear();
	this->_type = object._type;
	this->_vecPathCoords = object._vecPathCoords;
}

void ClosedObject::addClosedObject(const ClosedObject& closedObject)
{
	this->_centerCoords._xCoord = -1;
	this->_centerCoords._yCoord = -1;
	this->_type.clear();
	this->_vecPathCoords.clear();
	this->_vecVectorialCoords.clear();

	this->_centerCoords = closedObject._centerCoords;
	this->_type = closedObject._type;
	this->_vecPathCoords = closedObject._vecPathCoords;
	this->_vecVectorialCoords = closedObject._vecVectorialCoords;
}