#ifndef OPENOBJECT_H
#define OPENOBJECT_H

#include "Object.h"

namespace Core
{
	class OpenObject : protected Object
	{
	public:
		OpenObject(Object &object) : Object(object) {};
		~OpenObject();

	private:
		std::vector<Point> _vecVectorialCoords;
	};
}
#endif
