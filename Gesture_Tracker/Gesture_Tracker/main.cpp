#include "Global.h"
#include "ObjectsManager.h"
#include "Camera.h"

int main()
{
	Video::Camera camera;
	if (!camera.Start())
		camera.Stop();

	system("pause");

	return 0;
}