#include "Global.h"
#include "ObjectsManager.h"
#include "Camera.h"

int main()
{
	Managers::ObjectsManager *objectManager;
	objectManager = Managers::ObjectsManager::Instance();
	objectManager->printDetails();

	Video::Camera camera;
	if (!camera.Start())
		camera.Stop();

	camera.Record();

	system("pause");

	return 0;
}