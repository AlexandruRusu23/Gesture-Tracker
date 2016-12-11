#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H

#include "OpenObject.h"
#include "ClosedObject.h"

#include <algorithm>

namespace Managers
{
	// singleton class
	class ObjectsManager
	{
	private:
		std::string _type;
		std::vector<Core::Point> _points;
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
		void receiveObjects(const Core::Object objects);
		void receiceClosedObject(const Core::Object object);
		void receiceClosedObject(const Core::ClosedObject closeObject);
		void receiceOpenObject(const Core::Object object);
		void receiceOpenObject(const Core::OpenObject openObject);

		void ReceiveResult(std::string& type, std::vector<std::pair<int, int>>& points);

	private:
		// all objects stored in std::vector
		std::vector<Core::Object> _vecObjects;
		std::vector<Core::OpenObject> _vecOpenObjects;
		std::vector<Core::ClosedObject> _vecClosedObjects;

		void CreateRectangleEtalon(Core::Object& rectangleBorder, Core::Point downLeft, Core::Point aboveLeft, Core::Point downRight, Core::Point aboveRight);
		void CreateCircleEtalon(Core::Object& circleborder, float radius, Core::Point circleCenter, Core::Point aboveLeft, Core::Point downRight);
		void CreateRhombusEtalon(Core::Object& rhombusborder, Core::Point rhombusLeftMiddle, Core::Point rhombusAboveMiddle, Core::Point rhombusRightMiddle, Core::Point rhombusDownMiddle, Core::Point aboveLeft, Core::Point downRight);

		void CrossValidation(Core::Object& object, Core::Object etalon, double& sum);
		//void CircleCrossValidation(Core::Object& object, Core::Object& circleBorder, double& circleSum);

		bool CheckIfOnCircle(float objectX, float objectY, float centerX, float centerY, float calculatedRadius);

	};
}

#endif // !OBJECTMANAGER_H