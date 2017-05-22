#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include "opencv2/highgui/highgui.hpp"

#include "Tracking.h"

using namespace cv;
using namespace std;


/******************************************************
 * 
 * Function to convert string to int
 * 
 * Inputs: 
 * 		char* str: the 
 * 
 * Outputs:
 * 		-------
 * 
 * Restrinctions:
 * 		-------
 * 
 * Exceptions:
 * 		invalid_argument: received a not valid number
 * 
 * **********************************************/
int StrToInt(char *str)
{
	// Initialize result
    	int res = 0;
	// Iterate through all characters of input string and update the result
	for (int i = 0; str[i] != '\0'; ++i){
		if(str[i] - '0' >= 0){
			res = res*10 + str[i] - '0';
		}else{
			throw std::invalid_argument( "Received an invalid string" );
		}
	}
	// return result.
	return res;
}


/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
  cout << "usage: Tracking [image] \n" << endl;  
  cout << "       -m|--mask <M>     the mask to use " << endl; 
}

/******************************************************
 * 
 * The main function
 * 
 * **********************************************/
int main(int argc, char *argv[])
{

	// Ask if we have arguments
	if (argc > 1) {

		const cv::Mat imgObj = imread("./Reference.png", CV_LOAD_IMAGE_GRAYSCALE);
    	const cv::Mat maskObj = imread("./Mask.png", CV_LOAD_IMAGE_GRAYSCALE);

   		if (!imgObj.data || !maskObj.data)
		{
			std::cout << " --(!) Error reading images " << std::endl; 
			return -1;
	    	}
		
		std::vector<KeyPoint> keypointsImg, keypointsMask;
 		cv::Mat descriptorsImg, descriptorsMask;
		std::vector<cv::DMatch> matches;
		Tracking trackController = Tracking();
		trackController.KeyDescripExtract(imgObj,keypointsImg,descriptorsImg);
		trackController.KeyDescripExtract(maskObj,keypointsMask,descriptorsMask);
		trackController.DescripMatcher(descriptorsImg,descriptorsMask,matches);
		trackController.DrawMatches(matches,keypointsImg,keypointsMask,imgObj,maskObj);
		
			
	}else{
		return -1;
	}

  return 1;
  
}
