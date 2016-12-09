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

void ObjectsManager::receiveObjects(const std::vector<Core::Object> objects)
{
	_vecObjects = objects;
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