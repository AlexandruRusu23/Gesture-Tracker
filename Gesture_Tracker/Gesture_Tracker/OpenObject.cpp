#include "OpenObject.h"

using namespace Core;

Core::OpenObject::OpenObject(const Object & object)
{
	this->_type = object._type;
	this->_vecPathCoords.clear();
	this->_vecPathCoords = object._vecPathCoords;
}

OpenObject::~OpenObject()
{

}
