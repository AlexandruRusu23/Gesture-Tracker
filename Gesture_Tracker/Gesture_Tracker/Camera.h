#ifndef CAMERA_H
#define CAMERA_H

#include "Global.h"
#include "Object.h"
#include "ObjectsManager.h"

namespace Video
{
	// this class will access the Web Camera and after processing the data, will send them to Engine
	class Camera
	{
	public:
		Camera();
		~Camera();

		// start the camera
		bool Start();
		// stop the camera
		bool Stop();

		Core::Object object;

	private:
		// handler for Camera
		bool _isRunning;
		bool _isTracking;
		bool _isRecording;

		// object associated to web camera
		cv::VideoCapture _webUpCameraID;
		cv::VideoCapture _webSideCameraID;
		// images from cameras
		cv::Mat _imgOriginalUpView;
		cv::Mat _imgOriginalSideView;
		// upper camera HSV mapped
		cv::Mat _img1HSV;
		// side camera HSV mapped
		cv::Mat _img2HSV;
		// thresh map for Upper camera
		cv::Mat _img1Thresh;
		// thresh map for Side camera
		cv::Mat _img2Thresh;
		// tracker lines for Upper camera
		cv::Mat _imgLines1;
		// tracker lines for Side camera
		cv::Mat _imgLines2;
		// temp image for Upper camera manipulations
		cv::Mat _imgTmp1;
		// temp image for Side camera manipulations
		cv::Mat _imgTmp2;
		// Vectorial representation of draw objects
		cv::Mat _imgVectorialView;

		// thresh map limits
		int iLowH, iLowS, iLowV, iHighH, iHighS, iHighV;

		std::vector<Core::Point> _tracePointsUpCamera;

		// object moving mark
		cv::Rect _objectBoundingRectangle[2];
		int _theObject[2] = { 0, 0 };

		Managers::ObjectsManager *_objectsManager;

		std::vector<cv::Point> _vecVectorialObject;
		std::map<std::string, std::vector<cv::Point>> _mapVectorialObjects;

		void Record();

		// Threshold window
		void CreateControlWindow();

		void CreateThreshHold();

		void CalculateTrackedObjectPosition(int &iLast1X, int &iLast1Y, int &iLast2X, int &iLast2Y);

		void AddVectorialViewToImage(cv::Mat imgVectorialView, cv::Mat& imgOriginalView);

		void WindowsManipulation();

		// search object moving
		void SearchForMove(cv::Mat thresholdImage, cv::Mat &cameraFeed, int imgNumber);

		// keyboard event watcher
		void KeyboardEvent();
	};
}

#endif // !CAMERA_H
