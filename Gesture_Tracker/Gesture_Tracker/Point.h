#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point
{
public:
	// constructor
	Point() {};
	Point(float xCoord, float yCoord);
	~Point();

	//copy constructor
	Point(const Point& point);
	//operator=
	void operator=(const Point& point);

private:
	// coordinates
	float _xCoord, _yCoord;
};

#endif // !POINT_H