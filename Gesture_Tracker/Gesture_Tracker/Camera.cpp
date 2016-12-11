#include "Camera.h"
#include "ObjectsManager.h"

using namespace Video;

Camera::Camera()
{
	_objectsManager = Managers::ObjectsManager::Instance();
	// by default the class will try to connect for primary web cam installed on device
	this->_webUpCameraID.open(2);
	if (this->_webUpCameraID.isOpened() == false)
		Stop();
	this->_webSideCameraID.open(1);
	if (this->_webSideCameraID.isOpened() == false)
		Stop();
	this->_isRunning = true;
	this->_isTracking = false;

	//_vecVectorialObject.push_back(cv::Point(100, 100));
	//_vecVectorialObject.push_back(cv::Point(100, 200));
	//_vecVectorialObject.push_back(cv::Point(200, 200));
	//_vecVectorialObject.push_back(cv::Point(200, 100));

	//_mapVectorialObjects["polygon"] = _vecVectorialObject;
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

	Record();

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

	if (keyPress == 'c')
	{
		_imgLines1 = cv::Mat::zeros(_imgTmp1.size(), CV_8UC3);
		_imgLines2 = cv::Mat::zeros(_imgTmp2.size(), CV_8UC3);
	}
}

void Camera::CreateControlWindow()
{
	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	cv::resizeWindow("Control", 400, 300);

	iLowH = 62;
	iHighH = 167;

	iLowS = 72;
	iHighS = 255;

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

void Camera::CreateThreshHold()
{
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
}

void Camera::CalculateTrackedObjectPosition(int &iLast1X, int &iLast1Y, int &iLast2X, int &iLast2Y)
{
	//Calculate the moments of the thresholded image
	cv::Moments oMoments1 = cv::moments(_img1Thresh);
	//Calculate the moments of the thresholded image
	cv::Moments oMoments2 = cv::moments(_img2Thresh);

	if (_isTracking)
	{
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
				if ((abs(iLast1X - posX) >= 2 && abs(iLast1X - posX) <= 50) || (abs(iLast1Y - posY) >= 2 && abs(iLast1Y - posY) <= 50))
				{
					//Draw a red line from the previous point to the current point
					line(_imgLines1, cv::Point(posX, posY), cv::Point(iLast1X, iLast1Y), cv::Scalar(0, 0, 255), 2);
				}
			}

			iLast1X = posX;
			iLast1Y = posY;
		}
	}
	else
	{
		_imgLines1 = cv::Mat::zeros(_imgTmp1.size(), CV_8UC3);
		iLast1X = -1;
		iLast1Y = -1;
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
			if (posY < (2 * _imgOriginalSideView.rows) / 3)
			{
				_isTracking = false;
				_imgLines2 = cv::Mat::zeros(_imgTmp2.size(), CV_8UC3);
				iLast2X = -1;
				iLast2Y = -1;
				return;
			}
			_isTracking = true;
			if ((abs(iLast2X - posX) >= 2 && abs(iLast2X - posX) <= 50) || (abs(iLast2Y - posY) >= 2 && abs(iLast2Y - posY) <= 50))
			{
				//Draw a red line from the previous point to the current point
				line(_imgLines2, cv::Point(posX, posY), cv::Point(iLast2X, iLast2Y), cv::Scalar(0, 0, 255), 2);
			}
		}

		iLast2X = posX;
		iLast2Y = posY;
	}
}

void Video::Camera::AddVectorialViewToImage(cv::Mat imgVectorialView, cv::Mat& imgOriginalView)
{
	for (int y = 0; y < imgVectorialView.rows; y++)
	{
		for (int x = 0; x < imgVectorialView.cols; x++)
		{
			cv::Vec3b color = imgVectorialView.at<cv::Vec3b>(cv::Point(x, y));
			if (color[0] == 255)
			{
				imgOriginalView.at<cv::Vec3b>(cv::Point(x, y)) = color;
			}
		}
	}
}

void Camera::SearchForMove(cv::Mat thresholdImage, cv::Mat &cameraFeed, int imgNumber)
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

		_objectBoundingRectangle[imgNumber] = cv::boundingRect(largestContourVec.at(0));
		int xPos = _objectBoundingRectangle[imgNumber].x + _objectBoundingRectangle[imgNumber].width / 2;
		int yPos = _objectBoundingRectangle[imgNumber].y + _objectBoundingRectangle[imgNumber].height / 2;

		_theObject[0] = xPos;
		_theObject[1] = yPos;
	}

	if (_theObject[0] > 0 && _theObject[1] > 0)
	{
		int x = _theObject[0];
		int y = _theObject[1];

		cv::circle(cameraFeed, cv::Point(x, y), 20, cv::Scalar(0, 255, 0), 2);
		cv::line(cameraFeed, cv::Point(x, y), cv::Point(x, y - 25), cv::Scalar(0, 255, 0), 2);
		cv::line(cameraFeed, cv::Point(x, y), cv::Point(x, y + 25), cv::Scalar(0, 255, 0), 2);
		cv::line(cameraFeed, cv::Point(x, y), cv::Point(x - 25, y), cv::Scalar(0, 255, 0), 2);
		cv::line(cameraFeed, cv::Point(x, y), cv::Point(x + 25, y), cv::Scalar(0, 255, 0), 2);
	}
}

void Camera::WindowsManipulation()
{
	// declare windows
	cv::namedWindow("imgOriginalUpView", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("img2Thresh", CV_WINDOW_AUTOSIZE);	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image CV_WINDOW_AUTOSIZE is the default
	cv::namedWindow("imgOriginalSideView", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("img2Thresh", CV_WINDOW_AUTOSIZE);

	SearchForMove(_img1Thresh, _imgOriginalUpView, 0);
	SearchForMove(_img2Thresh, _imgOriginalSideView, 1);

	cv::Mat imgTemporar;
	_webSideCameraID.read(imgTemporar);
	cv::Mat imgTrackLimit = cv::Mat::zeros(imgTemporar.size(), CV_8UC3);
	// draw tracking limit line for side camera
	line(imgTrackLimit, cv::Point( 0, (2 * _imgOriginalSideView.rows) / 3), cv::Point(imgTemporar.cols, (2 * _imgOriginalSideView.rows) / 3), cv::Scalar(255, 0, 0), 5);

	AddVectorialViewToImage(_imgVectorialView, _imgOriginalUpView);
	AddVectorialViewToImage(imgTrackLimit, _imgOriginalSideView);

	_imgOriginalUpView += _imgLines1;
	_imgOriginalSideView += _imgLines2;
	
	cv::imshow("imgOriginalUpView", _imgOriginalUpView); // show windows
	cv::imshow("imgOriginalSideView", _imgOriginalSideView); // show windows
	cv::imshow("img1Thresh", _img1Thresh);
	cv::imshow("img2Thresh", _img2Thresh);
}

void Video::Camera::DrawVectorialObject(std::string type, std::vector<std::pair<int, int>> points)
{
	if (!type.compare("circle"))
	{
		cv::circle(_imgVectorialView, cv::Point(points[0].first, points[0].second), points[1].first, cv::Scalar(255, 0, 0), 2);
		AddVectorialViewToImage(_imgVectorialView, _imgOriginalUpView);
	}
	if (!type.compare("rectangle"))
	{
		double distance1 = sqrt((points[0].first - points[1].first)*(points[0].first - points[1].first) + (points[0].second - points[1].second) * (points[0].second - points[1].second));
		double distance2 = sqrt((points[1].first - points[2].first)*(points[1].first - points[2].first) + (points[1].second - points[2].second) * (points[1].second - points[2].second));

		if ((distance1 <= distance2 / 5) || (distance2 <= distance1 / 5))
			cv::line(_imgVectorialView, cv::Point((points[0].first + points[1].first)/2, points[0].second), cv::Point((points[0].first + points[1].first) / 2, points[2].second), cv::Scalar(255, 0, 0), 2);
		else
		{
			cv::line(_imgVectorialView, cv::Point(points[0].first, points[0].second), cv::Point(points[1].first, points[1].second), cv::Scalar(255, 0, 0), 2);
			cv::line(_imgVectorialView, cv::Point(points[1].first, points[1].second), cv::Point(points[2].first, points[2].second), cv::Scalar(255, 0, 0), 2);
			cv::line(_imgVectorialView, cv::Point(points[2].first, points[2].second), cv::Point(points[3].first, points[3].second), cv::Scalar(255, 0, 0), 2);
			cv::line(_imgVectorialView, cv::Point(points[3].first, points[3].second), cv::Point(points[0].first, points[0].second), cv::Scalar(255, 0, 0), 2);
		}
		AddVectorialViewToImage(_imgVectorialView, _imgOriginalUpView);
	}
	if (!type.compare("rhombus"))
	{
		cv::line(_imgVectorialView, cv::Point(points[0].first, points[0].second), cv::Point(points[1].first, points[1].second), cv::Scalar(255, 0, 0), 2);
		cv::line(_imgVectorialView, cv::Point(points[1].first, points[1].second), cv::Point(points[2].first, points[2].second), cv::Scalar(255, 0, 0), 2);
		cv::line(_imgVectorialView, cv::Point(points[2].first, points[2].second), cv::Point(points[3].first, points[3].second), cv::Scalar(255, 0, 0), 2);
		cv::line(_imgVectorialView, cv::Point(points[3].first, points[3].second), cv::Point(points[0].first, points[0].second), cv::Scalar(255, 0, 0), 2);
		AddVectorialViewToImage(_imgVectorialView, _imgOriginalUpView);
	}
	points.clear();
}

void Camera::Record()
{
	CreateControlWindow();

	int iLast1X = -1;
	int iLast1Y = -1;
	int iLast2X = -1;
	int iLast2Y = -1;

	_webUpCameraID.read(_imgTmp1);
	_webSideCameraID.read(_imgTmp2);

	_imgVectorialView = cv::Mat::zeros(_imgTmp1.size(), CV_8UC3);

	_imgLines1 = cv::Mat::zeros(_imgTmp1.size(), CV_8UC3);
	_imgLines2 = cv::Mat::zeros(_imgTmp2.size(), CV_8UC3);

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

		// create the threshold for both cameras
		CreateThreshHold();

		CalculateTrackedObjectPosition(iLast1X, iLast1Y, iLast2X, iLast2Y);

		if (iLast1X > 0 && iLast1Y > 0)
		{
			Core::Point aux;
			aux._xCoord = iLast1X;
			aux._yCoord = iLast1Y;
			_tracePointsUpCamera.push_back(aux);
		}

		if (!_isTracking)
		{
			object.addVector(_tracePointsUpCamera);

			_objectsManager->receiveObjects(object);

			std::string type;
			std::vector<std::pair<int, int>> points;

			_objectsManager->ReceiveResult(type, points);

			DrawVectorialObject(type, points);

			_tracePointsUpCamera.clear();
		}
		
		WindowsManipulation();

		KeyboardEvent();
	}
}