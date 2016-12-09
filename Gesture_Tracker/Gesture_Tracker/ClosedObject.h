#ifndef CLOSEDOBJECT_H
#define CLOSEDOBJECT_H

#include "Object.h"

namespace Core
{
	class ClosedObject : protected Object
	{
	public:
		ClosedObject() {};
		ClosedObject(const Object &object);
		~ClosedObject();

		void addObject(const Object& object);
		void addClosedObject(const ClosedObject& closeObject);

		//vectorial coordinates
		std::vector<Point> _vecVectorialCoords;
		//center of the object
		Point _centerCoords;

	private:

	};
}

#endif
