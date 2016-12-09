#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H

#include "OpenObject.h"
#include "ClosedObject.h"

class ObjectsManager
{
public:
	static ObjectsManager* Instance();
	void printDetails()
	{
		std::cout << "I'm a singleton class";
	}
private:
	ObjectsManager();
	ObjectsManager(const ObjectsManager& objectsManager);
	static ObjectsManager* _pInstance;
};

#endif // !OBJECTMANAGER_H