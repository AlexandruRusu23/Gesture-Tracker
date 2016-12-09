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