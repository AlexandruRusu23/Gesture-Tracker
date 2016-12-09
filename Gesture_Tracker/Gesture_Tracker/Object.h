#ifndef OBJECT_H
#define OBJECT_H

#include "Point.h"

#include <iostream>
#include <vector>
#include <string>

namespace Core
{
	class Object
	{
	public:
		//constructor
		Object() {};
		void addPoint(Point newPointCoords);
		void printPoints();
		Object(std::vector<Point> vecPathCoords);
		~Object();

		//copy constructor
		Object(const Object& object);
		//operator=
		void operator=(const Object& object);

		//points which represent an object
		std::vector<Point> _vecPathCoords;
		//type of an Object ( default = "Unknown")
		std::string type;

	private:

	};
}
#endif // !OBJECT_H