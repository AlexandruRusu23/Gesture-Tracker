#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H

#include "OpenObject.h"
#include "ClosedObject.h"

namespace Managers
{
	// singleton class
	class ObjectsManager
	{
	private:
		//private constructor
		ObjectsManager();
		ObjectsManager(const ObjectsManager& objectsManager);
		//instance pointer
		static ObjectsManager* _pInstance;
		void DetectObjectsType();

	public:
		static ObjectsManager* Instance();
		void printDetails()
		{
			std::cout << "I'm a singleton class";
		}
		void receiveObjects(const std::vector<Core::Object> objects);

	private:
		// all objects stored in std::vector
		std::vector<Core::Object> _vecObjects;
		std::vector<Core::OpenObject> _vecOpenObjects;
		std::vector<Core::ClosedObject> _vecClosedObjects;
	};
}

#endif // !OBJECTMANAGER_H