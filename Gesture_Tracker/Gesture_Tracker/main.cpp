#include "ObjectsManager.h"

int main()
{
	ObjectsManager *objectManager;
	objectManager = ObjectsManager::Instance();
	objectManager->printDetails();

	system("Pause");

	return 0;
}