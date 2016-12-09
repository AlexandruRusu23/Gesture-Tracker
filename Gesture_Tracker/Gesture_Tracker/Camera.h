#ifndef CAMERA_H
#define CAMERA_H

#include "Global.h"

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

	private:
		// handler for Camera
		bool _isRunning;
		// object associated to web camera
		cv::VideoCapture _webCameraID;
		// images
		cv::Mat _imgOriginal;
		cv::Mat _imgHSV;
		cv::Mat _imgThreshLow;
		cv::Mat _imgThreshHigh;
		cv::Mat _imgThresh;

		// circles found in image
		std::vector<cv::Vec3f> _v3fCircles;

		// keyboard event watcher
		void KeyboardEvent();
	};
}

#endif // !CAMERA_H
