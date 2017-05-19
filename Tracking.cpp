#include "Tracking.h"

Tracking::Tracking(void) {}

void Tracking::KeyDescripExtract(cv::Mat img, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors)
{
	//-- Step 1: Detect the keypoints using SIFT Detector
	cv::Ptr<cv::FeatureDetector> detector = FeatureDetector::create("SIFT");


    detector->detect(img, keypoints);

    //-- Step 2: Calculate descriptors (feature vectors)
    cv::Ptr<cv::DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");


    extractor->compute(img, keypoints, descriptors);

}


void Tracking::DescripMatcher(cv::Mat descriptors_object, cv::Mat descriptors_scene, std::vector< cv::DMatch > &matches)
{
	//-- Step 3: Matching descriptor vectors using FLANN matcher
    cv::Ptr<cv::DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

    std::vector< DMatch > firstMatches; //They will be filtered

    matcher->match(descriptors_object, descriptors_scene, firstMatches);

    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for (int i = 0; i < descriptors_object.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< cv::DMatch > matches;

    for (int i = 0; i < descriptors_object.rows; i++)
    {
        if (firstMatches[i].distance < 3 * min_dist)
        {
            matches.push_back(firstMatches[i]);
        }
    }
}