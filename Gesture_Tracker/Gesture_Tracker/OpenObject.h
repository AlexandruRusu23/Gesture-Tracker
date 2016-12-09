#ifndef OPENOBJECT_H
#define OPENOBJECT_H

#include "Object.h"

namespace Core
{
	class OpenObject : public Object
	{
	public:
		OpenObject(Object &object) : Object(object) {};
		~OpenObject();

		std::vector<Point> _vecVectorialCoords;

	private:

	};
}
#endif
