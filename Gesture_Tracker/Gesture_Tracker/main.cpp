#include "Global.h"
#include "ObjectsManager.h"

int main()
{
	Managers::ObjectsManager *objectManager;
	objectManager = Managers::ObjectsManager::Instance();
	objectManager->printDetails();

	system("Pause");

	return 0;
}