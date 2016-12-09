#ifndef CLOSEDOBJECT_H
#define CLOSEDOBJECT_H

#include "Object.h"

namespace Core
{
	class ClosedObject : protected Object
	{
	public:
		ClosedObject(Object &object) : Object(object) {};
		~ClosedObject();
	private:
		//vectorial coordinates
		std::vector<Point> _vecVectorialCoords;
		//center of the object
		Point _centerCoords;
	};
}

#endif
