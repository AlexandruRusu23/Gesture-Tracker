#include "Point.h"

Point::Point(float xCoord, float yCoord)
{
	this->_xCoord = xCoord;
	this->_yCoord = yCoord;
}

Point::~Point()
{

}

Point::Point(Point & point)
{
	this->_xCoord = point._xCoord;
	this->_yCoord = point._yCoord;
}

void Point::operator=(Point & point)
{
	this->_xCoord = point._xCoord;
	this->_yCoord = point._yCoord;
}