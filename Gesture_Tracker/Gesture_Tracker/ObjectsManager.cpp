#include "ObjectsManager.h"

using namespace Managers;

ObjectsManager* ObjectsManager::_pInstance = NULL;

ObjectsManager::ObjectsManager()
{

}

ObjectsManager* ObjectsManager::Instance()
{
	if (!_pInstance)
		_pInstance = new ObjectsManager;

	return _pInstance;
}

ObjectsManager::ObjectsManager(const ObjectsManager& objectsManager)
{

}

void ObjectsManager::receiveObjects(const Core::Object objects)
{
	std::vector<Core::Object> object;
	object.push_back(objects);
	_vecObjects = object;
}

void Managers::ObjectsManager::receiceClosedObject(const Core::Object objects)
{
	Core::ClosedObject closeObject(objects);
	this->_vecClosedObjects.push_back(closeObject);
}

void Managers::ObjectsManager::receiceClosedObject(const Core::ClosedObject closeObject)
{
	this->_vecClosedObjects.push_back(closeObject);
}

void Managers::ObjectsManager::receiceOpenObject(const Core::Object object)
{
	Core::OpenObject openObject(object);
	this->_vecOpenObjects.push_back(openObject);
}

void Managers::ObjectsManager::receiceOpenObject(const Core::OpenObject openObject)
{
}

void ObjectsManager::DetectObjectsType()
{
	// TO DO: open/closed object identification

	for (auto &it : _vecObjects)
	{
		for (auto &itr : it._vecPathCoords)
		{

		}
	}
}