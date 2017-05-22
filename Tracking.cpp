#include "Tracking.h"

using namespace std;

Tracking::Tracking(void) {}

void Tracking::KeyDescripExtract(const cv::Mat img, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors)
{
	//-- Step 1: Detect the keypoints using SIFT Detector
	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");
	detector->detect(img, keypoints);
	//-- Step 2: Calculate descriptors (feature vectors)
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create("ORB");

	extractor->compute(img, keypoints, descriptors);

}


void Tracking::DescripMatcher(const cv::Mat descriptorsImg,const cv::Mat descriptorsMask, std::vector<cv::DMatch> &matches)
{
	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
	//-- Step 4: They will be filtered
	std::vector<cv::DMatch> firstMatches;
	matcher->match(descriptorsImg, descriptorsMask, firstMatches);
	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptorsImg.rows; i++)
	{
		double dist = firstMatches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	for (int i = 0; i < descriptorsImg.rows; i++)
	{
		if (firstMatches[i].distance <= 3 * min_dist)
		{
		    matches.push_back(firstMatches[i]);
		}
	}
}

void Tracking::DrawMatches(const std::vector<cv::DMatch> matches,const std::vector<cv::KeyPoint> keypointsImg,
			   const std::vector<cv::KeyPoint> keypointsMask, const cv::Mat imgMat, const cv::Mat maskMat)
{

	cv::Mat imgMatches;
	cv::drawMatches(imgMat, keypointsImg, maskMat, keypointsMask,
			matches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
			std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//-- Localize the object
	std::vector<cv::Point2f> img;
	std::vector<cv::Point2f> mask;

	for (std::size_t i = 0; i < matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		img.push_back(keypointsImg[matches[i].queryIdx].pt);
		mask.push_back(keypointsMask[matches[i].trainIdx].pt);
	}
cout << "Puntos de interes en la imagen" << img.size() << endl;
cout << "Punto de interes en la mascara" << mask.size() << endl;
	cv::Mat H = cv::findHomography(img, mask, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<cv::Point2f> imgCorners(4);
	imgCorners[0] = cvPoint(0, 0); 
	imgCorners[1] = cvPoint(imgMat.cols, 0);
	imgCorners[2] = cvPoint(imgMat.cols, imgMat.rows); 
	imgCorners[3] = cvPoint(0, imgMat.rows);
	std::vector<cv::Point2f> maskCorners(4);

	cv::perspectiveTransform(imgCorners, maskCorners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(imgMatches, maskCorners[0] + cv::Point2f(imgMat.cols, 0), maskCorners[1] + cv::Point2f(imgMat.cols, 0), cv::Scalar(0, 255, 0), 4);
	line(imgMatches, maskCorners[1] + cv::Point2f(imgMat.cols, 0), maskCorners[2] + cv::Point2f(imgMat.cols, 0), cv::Scalar(0, 255, 0), 4);
	line(imgMatches, maskCorners[2] + cv::Point2f(imgMat.cols, 0), maskCorners[3] + cv::Point2f(imgMat.cols, 0), cv::Scalar(0, 255, 0), 4);
	line(imgMatches, maskCorners[3] + cv::Point2f(imgMat.cols, 0), maskCorners[0] + cv::Point2f(imgMat.cols, 0), cv::Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", imgMatches);
	cv::waitKey(0);

}

