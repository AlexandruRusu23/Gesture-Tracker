#include "Camera.h"

using namespace Video;

Camera::Camera()
{
	// by default the class will try to connect for primary web cam installed on device
	this->_webUpCameraID.open(2);
	if (this->_webUpCameraID.isOpened() == false)
		Stop();
	this->_webSideCameraID.open(1);
	if (this->_webSideCameraID.isOpened() == false)
		Stop();
	this->_isRunning = true;
}

Camera::~Camera()
{

}

bool Camera::Start()
{
	// check if VideoCapture object was associated to web cam successfully
	if (this->_webUpCameraID.isOpened() == false) {
		// if not, print error message to std out
		std::cout << "error: capWebcam1 not accessed successfully\n\n";
		// and exit program
		return false;
	}
	// check if VideoCapture object was associated to web cam successfully
	if (this->_webSideCameraID.isOpened() == false) {
		// if not, print error message to std out
		std::cout << "error: capWebcam2 not accessed successfully\n\n";
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

	int iLast1X = -1;
	int iLast1Y = -1;
	int iLast2X = -1;
	int iLast2Y = -1;

	cv::Mat imgTmp1;
	cv::Mat imgTmp2;

	_webUpCameraID.read(imgTmp1);
	_webSideCameraID.read(imgTmp2);

	cv::Mat imgLines1 = cv::Mat::zeros(imgTmp1.size(), CV_8UC3);
	cv::Mat imgLines2 = cv::Mat::zeros(imgTmp2.size(), CV_8UC3);

	std::vector<Core::Point> tracePoints;

	while (_isRunning)
	{
		if (!_webUpCameraID.isOpened())
		{
			std::cout << "error: capWebcam not accessed successfully\n\n";
			this->Stop();
		}
		if (!_webSideCameraID.isOpened())
		{
			std::cout << "error: capWebcam not accessed successfully\n\n";
			this->Stop();
		}

		if (!_webUpCameraID.read(_imgOriginalUpView) || _imgOriginalUpView.empty())
		{
			std::cout << "error: frame not read from web cam\n";
			this->Stop();
		}
		if (!_webSideCameraID.read(_imgOriginalSideView) || _imgOriginalSideView.empty())
		{
			std::cout << "error: frame not read from web cam\n";
			this->Stop();
		}

		cv::cvtColor(_imgOriginalUpView, _img1HSV, CV_BGR2HSV);
		cv::cvtColor(_imgOriginalSideView, _img2HSV, CV_BGR2HSV);

		//  cv::Scalar(37, 30, 70), cv::Scalar(68, 175, 170)
		cv::inRange(_img1HSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), _img1Thresh);
		cv::inRange(_img2HSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), _img2Thresh);
	
		//morphological opening (removes small objects from the foreground)
		erode(_img1Thresh, _img1Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(_img1Thresh, _img1Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		//morphological opening (removes small objects from the foreground)
		erode(_img2Thresh, _img2Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(_img2Thresh, _img2Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(_img1Thresh, _img1Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		erode(_img1Thresh, _img1Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(_img2Thresh, _img2Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		erode(_img2Thresh, _img2Thresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		//Calculate the moments of the thresholded image
		cv::Moments oMoments1 = cv::moments(_img1Thresh);
		//Calculate the moments of the thresholded image
		cv::Moments oMoments2 = cv::moments(_img2Thresh);

		double d1M01 = oMoments1.m01;
		double d1M10 = oMoments1.m10;
		double d1Area = oMoments1.m00;

		if (d1Area > 10000)
		{
			//calculate the position of the ball
			int posX = d1M10 / d1Area;
			int posY = d1M01 / d1Area;

			if (iLast1X >= 0 && iLast1Y >= 0 && posX >= 0 && posY >= 0)
			{
				if ((abs(iLast1X - posX) >= 5 && abs(iLast1X - posX) <= 50) || (abs(iLast1Y - posY) >= 5 && abs(iLast1Y - posY) <= 50))
				{
					//Draw a red line from the previous point to the current point
					line(imgLines1, cv::Point(posX, posY), cv::Point(iLast1X, iLast1Y), cv::Scalar(0, 0, 255), 2);
				}
			}

			iLast1X = posX;
			iLast1Y = posY;
		}

		double d2M01 = oMoments2.m01;
		double d2M10 = oMoments2.m10;
		double d2Area = oMoments2.m00;

		if (d2Area > 10000)
		{
			//calculate the position of the ball
			int posX = d2M10 / d2Area;
			int posY = d2M01 / d2Area;

			if (iLast2X >= 0 && iLast2Y >= 0 && posX >= 0 && posY >= 0)
			{
				if ((abs(iLast2X - posX) >= 5 && abs(iLast2X - posX) <= 50) || (abs(iLast2Y - posY) >= 5 && abs(iLast2Y - posY) <= 50))
				{
					//Draw a red line from the previous point to the current point
					line(imgLines2, cv::Point(posX, posY), cv::Point(iLast2X, iLast2Y), cv::Scalar(0, 0, 255), 2);
				}
			}

			iLast2X = posX;
			iLast2Y = posY;
		}

		Core::Point point(iLast1X, iLast1Y);
		tracePoints.push_back(point);
		object = tracePoints;

		// declare windows
		cv::namedWindow("imgOriginalUpView", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("img2Thresh", CV_WINDOW_AUTOSIZE);	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image CV_WINDOW_AUTOSIZE is the default
		cv::namedWindow("imgOriginalSideView", CV_WINDOW_AUTOSIZE);
		cv::namedWindow("img2Thresh", CV_WINDOW_AUTOSIZE);

		SearchForMove(_img1Thresh, _imgOriginalUpView);
		SearchForMove(_img2Thresh, _imgOriginalSideView);

		_imgOriginalUpView = _imgOriginalUpView + imgLines1;
		_imgOriginalSideView = _imgOriginalSideView + imgLines2;
		cv::imshow("imgOriginalUpView", _imgOriginalUpView); // show windows
		cv::imshow("imgOriginalSideView", _imgOriginalSideView); // show windows
		cv::imshow("img1Thresh", _img1Thresh);
		cv::imshow("img2Thresh", _img2Thresh);

		KeyboardEvent();
	}
}