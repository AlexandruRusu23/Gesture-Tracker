#include "Camera.h"

using namespace Video;

Camera::Camera()
{
	// by default the class will try to connect for primary web cam installed on device
	this->_webCameraID.open(0);
	if (this->_webCameraID.isOpened() == false)
		Stop();
	this->_isRunning = true;
}

Camera::~Camera()
{

}

bool Camera::Start()
{
	// check if VideoCapture object was associated to web cam successfully
	if (this->_webCameraID.isOpened() == false) {
		// if not, print error message to std out
		std::cout << "error: capWebcam not accessed successfully\n\n";
		// and exit program
		return false;
	}
	return true;
}

bool Camera::Stop()
{
	this->_isRunning = false;
	cv::destroyAllWindows();

	return true;
}

void Camera::KeyboardEvent()
{
	char keyPress;
	// wait 1 ms and get key press, if any
	keyPress = cv::waitKey(1);

	if (keyPress == 27)
	{
		std::cout << "Escape key pressed\n";
		this->Stop();
	}
}

void Camera::Record()
{
	while (_isRunning)
	{
		if (!_webCameraID.isOpened())
		{
			std::cout << "error: capWebcam not accessed successfully\n\n";
			this->Stop();
		}

		if (!_webCameraID.read(_imgOriginal) || _imgOriginal.empty())
		{
			std::cout << "error: frame not read from web cam\n";
			this->Stop();
		}

		cv::cvtColor(_imgOriginal, _imgHSV, CV_BGR2HSV);

		cv::inRange(_imgHSV, cv::Scalar(37, 30, 70), cv::Scalar(68, 175, 170), _imgThresh);
		//cv::inRange(_imgHSV, cv::Scalar(38, 98, 116), cv::Scalar(55, 224, 136), _imgThresh);

		//cv::add(_imgThreshLow, _imgThreshHigh, _imgThresh);

		cv::GaussianBlur(_imgThresh, _imgThresh, cv::Size(3, 3), 0);

		cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

		cv::dilate(_imgThresh, _imgThresh, structuringElement);
		cv::erode(_imgThresh, _imgThresh, structuringElement);

		// fill circles vector with all circles in processed image
		cv::HoughCircles(_imgThresh,			// input image
			_v3fCircles,							// function output (must be a standard template library vector
			CV_HOUGH_GRADIENT,					// two-pass algorithm for detecting circles, this is the only choice available
			2,									// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
			_imgThresh.rows / 4,				// min distance in pixels between the centers of the detected circles
			100,								// high threshold of Canny edge detector (called by cvHoughCircles)						
			70,									// low threshold of Canny edge detector (set at 1/2 previous value)
			10,									// min circle radius (any circles with smaller radius will not be returned)
			200);								// max circle radius (any circles with larger radius will not be returned)

		for (int i = 0; i < _v3fCircles.size(); i++) {		// for each circle . . .
															// show ball position x, y, and radius to command line
			//std::cout << "ball position x = " << v3fCircles[i][0]			// x position of center point of circle
			//	<< ", y = " << v3fCircles[i][1]								// y position of center point of circle
			//	<< ", radius = " << v3fCircles[i][2] << "\n";				// radius of circle

			// draw small green circle at center of detected object
			cv::circle(_imgOriginal,												// draw on original image
				cv::Point((int)_v3fCircles[i][0], (int)_v3fCircles[i][1]),		// center point of circle
				3,																// radius of circle in pixels
				cv::Scalar(0, 255, 0),											// draw pure green (remember, its BGR, not RGB)
				CV_FILLED);														// thickness, fill in the circle

																				// draw red circle around the detected object
			cv::circle(_imgOriginal,												// draw on original image
				cv::Point((int)_v3fCircles[i][0], (int)_v3fCircles[i][1]),		// center point of circle
				(int)_v3fCircles[i][2],											// radius of circle in pixels
				cv::Scalar(0, 0, 255),											// draw pure red (remember, its BGR, not RGB)
				3);																// thickness of circle in pixels
		}	// end for

		// declare windows
		cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgThresh", CV_WINDOW_AUTOSIZE);	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default

		cv::imshow("imgOriginal", _imgOriginal);			// show windows
		cv::imshow("imgThresh", _imgThresh);

		KeyboardEvent();
	}
}