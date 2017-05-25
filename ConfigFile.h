#include <iostream>
#include <string>
#include <sstream>

#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class ConfigFile
{
	public:
		/******************************************************
		 * 
		 * Constructor of the class ConfigFile with default values
		 * 
		 * **********************************************
		 * 
		 * Inputs: 
		 * 		std::string path: The path of the file
		 * 
		 * Outputs:
		 * 		-------
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		ConfigFile(const std::string path);

		/******************************************************
		 * 
		 * Function to load the Key points and descriptors
		 * 
		 * **********************************************
		 * 
		 * InOutputs:
		 *		std::vector<cv::KeyPoint> &keypoints: The keypoints obtained of the image 
		 *		cv::Mat &descriptors: The descriptors
		 *
		 * 
		 * Outputs:
		 *		bool exist: If the file exists
		 *		
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		bool LoadData(std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

		
		/******************************************************
		 * 
		 * Function to save the Key points and descriptors
		 * 
		 * **********************************************
		 * 
		 * Inputs:
		 *		const std::vector<cv::KeyPoint> keypoints: The keypoints obtained of the image 
		 *		const cv::Mat descriptors: The descriptors
		 *
		 * 
		 * Outputs:
		 *		bool exist: If the file exists
		 *		
		 * 
		 * Restrinctions:
		 * 		-------
		 *****/
		bool SaveData(const std::vector<cv::KeyPoint> keypoints, const cv::Mat descriptors);
		

	private:
		std::string configFilePath;
		
};
