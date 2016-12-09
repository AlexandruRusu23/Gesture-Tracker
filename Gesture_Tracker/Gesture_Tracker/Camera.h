#ifndef CAMERA_H
#define CAMERA_H

#include "Global.h"
#include "Object.h"

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

		void Record();

		Core::Object object;

	private:
		// handler for Camera
		bool _isRunning;
		// object associated to web camera
		cv::VideoCapture _webCameraID;
		// images
		cv::Mat _imgOriginal;
		cv::Mat _imgHSV;
		cv::Mat _imgThresh;

		// circles found in image
		std::vector<cv::Vec3f> _v3fCircles;

		int iLowH, iLowS, iLowV, iHighH, iHighS, iHighV;

		// object moving mark
		cv::Rect _objectBoundingRectangle = cv::Rect(0, 0, 0, 0);
		int _theObject[2] = { 0, 0 };

		// Threshold window
		void CreateControlWindow();

		// search object moving
		void SearchForMove(cv::Mat thresholdImage, cv::Mat &cameraFeed);

		// keyboard event watcher
		void KeyboardEvent();
	};
}

#endif // !CAMERA_H
