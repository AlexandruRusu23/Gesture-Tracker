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
	Core::Object aux;
	Core::Object myData = objects;
	for (std::vector<Core::Point>::iterator it = myData._vecPathCoords.begin(); it != myData._vecPathCoords.end(); it++)
	{
		if (it->_xCoord <= 5 && it->_yCoord <= 5)
		{

		}
		else
		{
			Core::Point auxPoint(it->_xCoord, it->_yCoord);
			aux.addPoint(auxPoint);
		}
	}
	std::vector<Core::Object> object;
	object.push_back(aux);
	_vecObjects = object;
	DetectObjectsType();
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

void ObjectsManager::ReceiveResult(std::string& type, std::vector<std::pair<int, int>>& points)
{
	DetectObjectsType();
	type = _type;
	for (std::vector<Core::Point>::iterator it = _points.begin(); it != _points.end(); it++)
	{
		std::pair<int, int> aux;
		aux.first = it->_xCoord;
		aux.second = it->_yCoord;
		points.push_back(aux);
	}
	_points.clear();
}

void ObjectsManager::CreateRectangleEtalon(Core::Object& border, Core::Point downLeft, Core::Point aboveLeft, Core::Point downRight, Core::Point aboveRight)
{

	// dubluri
	float DLX, DLY, ALX, ALY, DRX, DRY, ARX, ARY;

	//DLX = downLeft._xCoord;
	//DLY = downLeft._yCoord;
	//ALX = aboveLeft._xCoord;
	//ALY = aboveLeft._yCoord;
	//DRX = downRight._xCoord;
	//DRY = downRight._yCoord;
	ARX = aboveRight._xCoord;
	//ARY = aboveRight._yCoord;

	//creez latura 1 -> 2
	border._vecPathCoords.push_back(aboveRight);
	aboveRight._yCoord = aboveRight._yCoord - 1;

	while (aboveRight._yCoord > downRight._yCoord)
	{
		border._vecPathCoords.push_back(aboveRight);
		aboveRight._yCoord = aboveRight._yCoord - 1;
	}

	//creez latura 2 -> 3
	border._vecPathCoords.push_back(downRight);
	downRight._xCoord = downRight._xCoord - 1;
	while (downRight._xCoord > downLeft._xCoord)
	{
		border._vecPathCoords.push_back(downRight);
		downRight._xCoord = downRight._xCoord - 1;
	}

	//creez latura 3 -> 4
	border._vecPathCoords.push_back(downLeft);
	downLeft._yCoord = downLeft._yCoord + 1;
	while (downLeft._yCoord < aboveLeft._yCoord)
	{
		border._vecPathCoords.push_back(downLeft);
		downLeft._yCoord = downLeft._yCoord + 1;
	}

	//creez latura 4 -> 1
	border._vecPathCoords.push_back(aboveLeft);
	aboveLeft._xCoord = aboveLeft._xCoord + 1;
	while (aboveLeft._xCoord < ARX)
	{
		border._vecPathCoords.push_back(aboveLeft);
		aboveLeft._xCoord = aboveLeft._xCoord + 1;
	}
}

void  ObjectsManager::CreateCircleEtalon(Core::Object& circleborder, float radius, Core::Point circleCenter, Core::Point aboveLeft, Core::Point downRight)
{
	for (int y = (int)downRight._yCoord; y <= (int)aboveLeft._yCoord; y++)
	{
		for (int x = (int)aboveLeft._xCoord; x <= (int)downRight._xCoord; x++)
		{
			if (CheckIfOnCircle((float)x, (float)y, circleCenter._xCoord, circleCenter._yCoord, radius))
			{
				Core::Point punct;
				punct._xCoord = x;
				punct._yCoord = y;
				circleborder._vecPathCoords.push_back(punct);
			}
		}
	}
}

void ObjectsManager::CreateRhombusEtalon(Core::Object& rhombusborder, Core::Point rhombusLeftMiddle, Core::Point rhombusAboveMiddle, Core::Point rhombusRightMiddle, Core::Point rhombusDownMiddle, Core::Point aboveLeft, Core::Point downRight)
{
	for (int y = (int)downRight._yCoord; y <= (int)aboveLeft._yCoord; y++)
	{
		for (int x = (int)aboveLeft._xCoord; x <= (int)downRight._xCoord; x++)
		{
			if ((x - rhombusLeftMiddle._xCoord) / (rhombusAboveMiddle._xCoord - rhombusLeftMiddle._xCoord) == (y - rhombusLeftMiddle._yCoord) / (rhombusAboveMiddle._yCoord - rhombusLeftMiddle._yCoord))
			{
				Core::Point punct;
				punct._xCoord = x;
				punct._yCoord = y;
				rhombusborder._vecPathCoords.push_back(punct);
			}
			if ((x - rhombusAboveMiddle._xCoord) / (rhombusRightMiddle._xCoord - rhombusAboveMiddle._xCoord) == (y - rhombusAboveMiddle._yCoord) / (rhombusRightMiddle._yCoord - rhombusAboveMiddle._yCoord))
			{
				Core::Point punct;
				punct._xCoord = x;
				punct._yCoord = y;
				rhombusborder._vecPathCoords.push_back(punct);
			}
			if ((x - rhombusRightMiddle._xCoord) / (rhombusDownMiddle._xCoord - rhombusRightMiddle._xCoord) == (y - rhombusRightMiddle._yCoord) / (rhombusDownMiddle._yCoord - rhombusRightMiddle._yCoord))
			{
				Core::Point punct;
				punct._xCoord = x;
				punct._yCoord = y;
				rhombusborder._vecPathCoords.push_back(punct);
			}
			if ((x - rhombusDownMiddle._xCoord) / (rhombusLeftMiddle._xCoord - rhombusDownMiddle._xCoord) == (y - rhombusDownMiddle._yCoord) / (rhombusLeftMiddle._yCoord - rhombusDownMiddle._yCoord))
			{
				Core::Point punct;
				punct._xCoord = x;
				punct._yCoord = y;
				rhombusborder._vecPathCoords.push_back(punct);
			}
		}
	}

	rhombusborder._vecPathCoords.push_back(rhombusLeftMiddle);
	rhombusborder._vecPathCoords.push_back(rhombusAboveMiddle);
	rhombusborder._vecPathCoords.push_back(rhombusRightMiddle);
	rhombusborder._vecPathCoords.push_back(rhombusDownMiddle);

}


bool ObjectsManager::CheckIfOnCircle(float objectX, float objectY, float centerX, float centerY, float calculatedRadius)
{
	//vf daca forma e inchisa sau deschisa 

	float dx = abs((objectX - centerX) * (objectX - centerX));
	float dy = abs((objectY - centerY) * (objectY - centerY));
	float r = calculatedRadius;


	if ((dx + dy <= r * r + 50) && (dx + dy >= r * r - 50))
	{
		return true; //este PE CERC
	}
	return false;
}


void ObjectsManager::CrossValidation(Core::Object& object, Core::Object etalon, double& sum)
{
	std::vector<Core::Point>::iterator obj;
	for (obj = object._vecPathCoords.begin(); obj != object._vecPathCoords.end(); obj++)
	{
		double minDistance = 200000;
		std::vector<Core::Point>::iterator border;
		for (border = etalon._vecPathCoords.begin(); border != etalon._vecPathCoords.end(); border++)
		{
				double distance = sqrt((border->_xCoord - obj->_xCoord)*(border->_xCoord - obj->_xCoord) + (border->_yCoord - obj->_yCoord) * (border->_yCoord - obj->_yCoord));
				if (distance < minDistance)
					minDistance = distance;
		}
		sum = sum + minDistance;
	}
}

void ObjectsManager::DetectObjectsType()
{
	//puncte extreme
	float maxY = 0, minY = 20000, maxX = 0, minX = 20000;

	//colturi border
	Core::Point aboveLeft, downLeft, aboveRight, downRight;

	//creez border-ul (e un obiect)
	Core::Object rectangularBorder, circleBorder, rhombusBorder;

	//trec prin vectorul de obiecte
	std::vector<Core::Object>::iterator it;
	for (it = _vecObjects.begin(); it != _vecObjects.end(); it++)
	{
		//trec prin fiecare punct la obiectului
		std::vector<Core::Point>::iterator itr;
		for (itr = it->_vecPathCoords.begin(); itr != it->_vecPathCoords.end(); itr++)
		{
				//verific daca x e max sau min
				if (itr->_xCoord > maxX) maxX = itr->_xCoord;
				else if (itr->_xCoord < minX) minX = itr->_xCoord;

				//verific daca y e max sau min
				if (itr->_yCoord > maxY) maxY = itr->_yCoord;
				else if (itr->_yCoord < minY) minY = itr->_yCoord;
		}
	}
	//am colturile

	if ((minY <= 0 && maxY >= 10000) && (minX <= 0 && maxX >= 10000))
	{
		_type = "unknown";
		return;
	}
	//PATRAT

	downLeft._xCoord = minX;
	downLeft._yCoord = minY;

	aboveLeft._xCoord = minX;
	aboveLeft._yCoord = maxY;

	downRight._xCoord = maxX;
	downRight._yCoord = minY;

	aboveRight._xCoord = maxX;
	aboveRight._yCoord = maxY;

	CreateRectangleEtalon(rectangularBorder, downLeft, aboveLeft, downRight, aboveRight);

	//CERC

	//gasire mijlocuri
	Core::Point leftMiddle, aboveMiddle, circleCenter;
	float aboveRadius, leftRadius, radius;

	leftMiddle._xCoord = (downLeft._xCoord + aboveLeft._xCoord) / 2;
	leftMiddle._yCoord = (downLeft._yCoord + aboveLeft._yCoord) / 2;

	aboveMiddle._xCoord = (downLeft._xCoord + downRight._xCoord) / 2;
	aboveMiddle._yCoord = (downLeft._yCoord + downRight._yCoord) / 2;

	circleCenter._xCoord = aboveMiddle._xCoord;
	circleCenter._yCoord = leftMiddle._yCoord;

	aboveRadius = sqrt((circleCenter._xCoord - aboveMiddle._xCoord)*(circleCenter._xCoord - aboveMiddle._xCoord) + (circleCenter._yCoord - aboveMiddle._yCoord) * (circleCenter._yCoord - aboveMiddle._yCoord));
	leftRadius = sqrt((circleCenter._xCoord - leftMiddle._xCoord)*(circleCenter._xCoord - leftMiddle._xCoord) + (circleCenter._yCoord - leftMiddle._yCoord) * (circleCenter._yCoord - leftMiddle._yCoord));
	if (aboveRadius < leftRadius) radius = aboveRadius;
	else radius = leftRadius;

	CreateCircleEtalon(circleBorder, radius, circleCenter, aboveLeft, downRight);

	//ROMB


	Core::Point rhombusLeftMiddle, rhombusAboveMiddle, rhombusRightMiddle, rhombusDownMiddle;

	rhombusLeftMiddle._xCoord = (downLeft._xCoord + aboveLeft._xCoord) / 2;
	rhombusLeftMiddle._yCoord = (downLeft._yCoord + aboveLeft._yCoord) / 2;

	rhombusAboveMiddle._xCoord = (downLeft._xCoord + downRight._xCoord) / 2;
	rhombusAboveMiddle._yCoord = (downLeft._yCoord + downRight._yCoord) / 2;

	rhombusRightMiddle._xCoord = (downRight._xCoord + aboveRight._xCoord) / 2;
	rhombusRightMiddle._yCoord = (downRight._yCoord + aboveRight._yCoord) / 2;

	rhombusDownMiddle._xCoord = (aboveRight._xCoord + aboveLeft._xCoord) / 2;
	rhombusDownMiddle._yCoord = (aboveRight._yCoord + aboveLeft._yCoord) / 2;

	CreateRhombusEtalon(rhombusBorder, rhombusLeftMiddle, rhombusAboveMiddle, rhombusRightMiddle, rhombusDownMiddle, aboveLeft, downRight);



	//CROSS VALIDATION

	double rectangularSum = 0, circleSum = 0, rhombusSum = 0, finalSum;

	//trec prin vectorul de obiecte
	std::vector<Core::Object>::iterator it2;
	for (it2 = _vecObjects.begin(); it2 != _vecObjects.end(); it2++)
	{
		CrossValidation(*it2, rectangularBorder, rectangularSum);
		CrossValidation(*it2, circleBorder, circleSum);
		CrossValidation(*it2, rhombusBorder, rhombusSum);
	}

	std::vector<double> sume;
	sume.push_back(rectangularSum);
	sume.push_back(circleSum);
	sume.push_back(rhombusSum);

	finalSum = *min_element(sume.begin(), sume.end());

	if (finalSum == rectangularSum)
	{
		_type = "rectangle";
		_points.push_back(aboveRight);
		_points.push_back(downRight);
		_points.push_back(downLeft);
		_points.push_back(aboveLeft);
	}
	else if (finalSum == circleSum)
	{
		_type = "circle";
		_points.push_back(circleCenter);
		Core::Point raza;
		raza._xCoord = radius;
		raza._yCoord = 0;
		_points.push_back(raza);
	}
	else if (finalSum == rhombusSum)
	{
		_type = "rhombus";
		_points.push_back(rhombusLeftMiddle);
		_points.push_back(rhombusAboveMiddle);
		_points.push_back(rhombusRightMiddle);
		_points.push_back(rhombusDownMiddle);
	}
}