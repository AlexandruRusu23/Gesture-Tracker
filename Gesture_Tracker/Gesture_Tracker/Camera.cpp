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
		object.printPoints();
		this->Stop();
	}
}

void Camera::CreateControlWindow()
{
	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	cv::resizeWindow("Control", 400, 300);

	iLowH = 41;
	iHighH = 87;

	iLowS = 71;
	iHighS = 160;

	iLowV = 0;
	iHighV = 255;

	//Create track bars in "Control" window
	cv::createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cv::createTrackbar("HighH", "Control", &iHighH, 179);

	cv::createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cv::createTrackbar("HighS", "Control", &iHighS, 255);

	cv::createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	cv::createTrackbar("HighV", "Control", &iHighV, 255);
}

void Camera::SearchForMove(cv::Mat thresholdImage, cv::Mat &cameraFeed)
{
	bool objectDetected = false;
	cv::Mat temp;
	thresholdImage.copyTo(temp);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size() > 0)
		objectDetected = true;

	if (objectDetected)
	{
		std::vector<std::vector<cv::Point>> largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));

		_objectBoundingRectangle = cv::boundingRect(largestContourVec.at(0));
		int xPos = _objectBoundingRectangle.x + _objectBoundingRectangle.width / 2;
		int yPos = _objectBoundingRectangle.y + _objectBoundingRectangle.height / 2;

		_theObject[0] = xPos;
		_theObject[1] = yPos;
	}

	int x = _theObject[0];
	int y = _theObject[1];

	cv::circle(cameraFeed, cv::Point(x, y), 20, cv::Scalar(0, 255, 0), 2);
	cv::line(cameraFeed, cv::Point(x, y), cv::Point(x, y-25), cv::Scalar(0, 255, 0), 2);
	cv::line(cameraFeed, cv::Point(x, y), cv::Point(x, y+25), cv::Scalar(0, 255, 0), 2);
	cv::line(cameraFeed, cv::Point(x, y), cv::Point(x-25, y), cv::Scalar(0, 255, 0), 2);
	cv::line(cameraFeed, cv::Point(x, y), cv::Point(x+25, y), cv::Scalar(0, 255, 0), 2);
}

void Camera::Record()
{
	CreateControlWindow();

	int iLastX = -1;
	int iLastY = -1;

	cv::Mat imgTmp;
	_webCameraID.read(imgTmp);

	cv::Mat imgLines = cv::Mat::zeros(imgTmp.size(), CV_8UC3);

	std::vector<Core::Point> tracePoints;

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

		//  cv::Scalar(37, 30, 70), cv::Scalar(68, 175, 170)
		cv::inRange(_imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), _imgThresh);
	
		//morphological opening (removes small objects from the foreground)
		erode(_imgThresh, _imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(_imgThresh, _imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(_imgThresh, _imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		erode(_imgThresh, _imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		//Calculate the moments of the thresholded image
		cv::Moments oMoments = cv::moments(_imgThresh);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		if (dArea > 10000)
		{
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				//Draw a red line from the previous point to the current point
				line(imgLines, cv::Point(posX, posY), cv::Point(iLastX, iLastY), cv::Scalar(0, 0, 255), 2);
			}

			iLastX = posX;
			iLastY = posY;
		}
		Core::Point point(iLastX, iLastY);
		tracePoints.push_back(point);
		object = tracePoints;

		// declare windows
		cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgThresh", CV_WINDOW_AUTOSIZE);	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default

		SearchForMove(_imgThresh, _imgOriginal);

		_imgOriginal = _imgOriginal + imgLines;
		cv::imshow("imgOriginal", _imgOriginal);			// show windows
		cv::imshow("imgThresh", _imgThresh);

		KeyboardEvent();
	}
}