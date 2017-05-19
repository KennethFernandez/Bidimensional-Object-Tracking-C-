#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <opencv/cv.h>

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
		 * Outputs:
		 * 		-------
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		void KeyDescripExtract(cv::Mat img, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

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
		 * Outputs:
		 * 		-------
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		void DescripMatcher(cv::Mat descriptors_object, cv::Mat descriptors_scene, std::vector< cv::DMatch > &matches);
	private:
		
};
