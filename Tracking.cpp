#include "Tracking.h"



Tracking::Tracking(void) {}

void Tracking::KeyDescripExtract(const cv::Mat img, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors)
{
	//-- Step 1: Detect the keypoints using SIFT Detector
	cv::initModule_nonfree();
	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("SIFT");
	detector->detect(img, keypoints);
	//-- Step 2: Calculate descriptors (feature vectors)
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create("SIFT");

	extractor->compute(img, keypoints, descriptors);

}


void Tracking::DescripMatcher(const cv::Mat descriptorsImg,const cv::Mat descriptorsMask, std::vector<cv::DMatch> &matches)
{
	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");
	//-- Step 4: They will be filtered
	std::vector<cv::DMatch> firstMatches;
	
	matcher->match(descriptorsMask, descriptorsImg, firstMatches);
	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptorsMask.rows; i++)
	{
		double dist = firstMatches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	for (int i = 0; i < descriptorsMask.rows; i++)
	{
		if (firstMatches[i].distance <= 5 * min_dist)
		{
		    matches.push_back(firstMatches[i]);
		}
	}
}


void Tracking::DrawMatches(const std::vector<cv::DMatch> matches,const std::vector<cv::KeyPoint> keypointsImg,
			   const std::vector<cv::KeyPoint> keypointsMask, const cv::Mat imgMat, const cv::Mat maskMat)
{
	cv::Mat imgMatches;
	//std::cout << "keypointsMask " <<  keypointsMask.size() << std::endl;
	//std::cout << "keypointsImg " <<  keypointsImg.size() << std::endl;
	//std::cout << "matches " <<  matches.size() << std::endl;

	try
  	{
    cv::drawMatches(maskMat, keypointsMask, imgMat, keypointsImg,
			matches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
			std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
  	}
  	catch (cv::Exception& e)
  	{
    //std::cout << "An exception occurred. Exception: " << e.what() << '\n';
  	}
	
	std::cout << "pasa2" << std::endl;
	//-- Localize the object
	std::vector<cv::Point2f> img;
	std::vector<cv::Point2f> mask;

	for (std::size_t i = 0; i < matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		img.push_back(keypointsImg[matches[i].trainIdx].pt);
		mask.push_back(keypointsMask[matches[i].queryIdx].pt);
	}

	
	cv::Mat H = cv::findHomography(mask, img, CV_RANSAC);
	
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<cv::Point2f> maskCorners(4);
	maskCorners[0] = cvPoint(0, 0); 
	maskCorners[1] = cvPoint(maskMat.cols, 0);
	maskCorners[2] = cvPoint(maskMat.cols, maskMat.rows); 
	maskCorners[3] = cvPoint(0, maskMat.rows);
	std::vector<cv::Point2f> imgCorners(4);
	
	cv::perspectiveTransform(maskCorners, imgCorners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(imgMatches, imgCorners[0] + cv::Point2f(maskMat.cols, 0), imgCorners[1] + cv::Point2f(maskMat.cols, 0), cv::Scalar(0, 255, 0), 4);
	line(imgMatches, imgCorners[1] + cv::Point2f(maskMat.cols, 0), imgCorners[2] + cv::Point2f(maskMat.cols, 0), cv::Scalar(0, 255, 0), 4);
	line(imgMatches, imgCorners[2] + cv::Point2f(maskMat.cols, 0), imgCorners[3] + cv::Point2f(maskMat.cols, 0), cv::Scalar(0, 255, 0), 4);
	line(imgMatches, imgCorners[3] + cv::Point2f(maskMat.cols, 0), imgCorners[0] + cv::Point2f(maskMat.cols, 0), cv::Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", imgMatches);
	//cv::waitKey(30);

}

