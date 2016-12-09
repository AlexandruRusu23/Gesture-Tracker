#ifndef POINT_H
#define POINT_H

#include <iostream>

namespace Core
{
	class Point
	{
	public:
		// constructor
		Point() {};
		Point(float xCoord, float yCoord);
		~Point();

		// coordinates
		float _xCoord, _yCoord;

		//copy constructor
		Point(const Point& point);
		//operator=
		void operator=(const Point& point);
	};
}
#endif // !POINT_H