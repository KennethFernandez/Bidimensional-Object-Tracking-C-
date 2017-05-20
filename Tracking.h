#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <opencv/cv.h>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Tracking
{
	public:
		/******************************************************
		 * 
		 * Constructor of the class Tracking with default values
		 * 
		 * **********************************************
		 * 
		 * Inputs: 
		 * 		-------
		 * 
		 * Outputs:
		 * 		-------
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		Tracking();

		/******************************************************
		 * 
		 * Key points and descriptors extrator from a given image
		 * 
		 * **********************************************
		 * 
		 * Inputs: 
		 * 		Mat img: The matrix of the image from which the key points and descriptors will be extracted
		 * 
		 * InOutputs:
		 *		std::vector<cv::KeyPoint> &keypoints: The keypoints obtained of the image 
		 *		cv::Mat &descriptors: The descriptors
		 *		
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		void KeyDescripExtract(const cv::Mat img, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

		/******************************************************
		 * 
		 * Match the descriptors from the object with the descriptors from the scene (which should include the object)
		 * 
		 * **********************************************
		 * 
		 * Inputs: 
		 * 		Mat descriptors_object: The descriptors from the object
		 *		Mat descriptors_scene: The descriptors from the scene
		 * 
		 * InOutputs:
		 * 		std::vector<cv::DMatch> &matches: Vector with the points obtained
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		void DescripMatcher(const cv::Mat descriptors_object, const cv::Mat descriptors_scene, std::vector<cv::DMatch> &matches);
	
		/******************************************************
		 * 
		 * Draw the matches to show the result
		 * 
		 * **********************************************
		 * 
		 * Inputs: 
		 * 		
		 * 
		 * InOutputs:
		 * 		
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		void DrawMatches(const std::vector< cv::DMatch > matches,const std::vector<cv::KeyPoint> keypointsImg,
			   	 const std::vector<cv::KeyPoint> keypointsMask, const cv::Mat imgMat, const cv::Mat maskMat);

	private:
		
};
