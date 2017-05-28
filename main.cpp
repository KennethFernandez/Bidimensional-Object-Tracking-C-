#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include "opencv2/highgui/highgui.hpp"

#include "Tracking.h"
#include "ConfigFile.h"

/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
	std::cout << "usage: Tracking [imagePath] maskPath \n" << std::endl;
	std::cout << "			-v maskPath: mode video\n" << std::endl;
	std::cout << "			-h or --help: help\n" << std::endl;
	std::cout << "----------------------------------------\n" << std::endl;
	std::cout << "Default mask: \"./Mask.png\" \n" << std::endl;
	
}

/******************************************************
 * 
 * The main function
 * 
 * **********************************************/
int main(int argc, char *argv[])
{

	// Ask if we have arguments
	if (argc > 1 && argc < 4) {


		////********************************** Help section **************************************		
		// If the user is asked about info
		if (std::string(argv[1])=="-h" || std::string(argv[1])=="--help") {
			help();
			return 1;
		}
		////*************************************************************************************


		////***************************** Load Mask section **************************************	
		// Assign the default value of the mask
		std::string maskPath = (argc == 2)? std::string(argv[2]): "./Mask.png";
		// Load the mask image
    		const cv::Mat maskObj = cv::imread(maskPath, CV_LOAD_IMAGE_GRAYSCALE);
		// Ask if the images are not empty
   		if (!maskObj.data)
		{
			std::cout << " --(!) Error reading mask " << std::endl; 
			return -1;
	    	}
		////*************************************************************************************


		////******************** Load configuration section **************************************
		// The basic data structure
		std::vector<cv::KeyPoint> keypointsImg, keypointsMask;
 		cv::Mat descriptorsImg, descriptorsMask;
		std::vector<cv::DMatch> matches;

		// We create the main classes to do the tracking
		Tracking trackController = Tracking();
		ConfigFile configController = ConfigFile("./config.tr");

		// If it's 3 is a new mask
		
		if(argc == 3){
			trackController.KeyDescripExtract(maskObj,keypointsMask,descriptorsMask);
			configController.SaveData(keypointsMask,descriptorsMask);
		}
		else if (argc == 2)
		{
			configController.LoadData(keypointsMask,descriptorsMask);
		}
		////*************************************************************************************




		////***************************** Video Section ******************************************

		if(std::string(argv[1])=="-v"){
			int r_int = 0;
			std::string imageName = "Reference2.png";
			cv::Mat save_img;
			cv::Mat reference_img;
			std::cout << "Press 'y' to take the reference photo" << std::endl;
			// We take the image from the camera
			while(true)
			{				
				cv::VideoCapture stream(0);
    			cv::namedWindow("video",1);
    			
       			stream >> save_img;

       			if(save_img.empty())
      			{
			    	std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
			    }
			    cv::imshow("video", save_img);

			    r_int = cv::waitKey(30);

			    if (r_int == 1048697) { // 'y'
			    	reference_img = save_img;
			    	cv::imwrite(imageName, reference_img); // A PNG FILE IS BEING SAVED
			    	break;
			    }

			    //std::cout << r_int << std::endl;

       		}

       		////***************************** Tracking section **************************************
			// Get the other descriptor and get the mathes	

       		cv::Mat save_img2;
       		trackController.KeyDescripExtract(reference_img,keypointsMask,descriptorsMask);
       		while(true)
       		{
       			cv::VideoCapture stream2(0);
    			cv::namedWindow("video2",1);
    			stream2 >> save_img2;

       			if(save_img.empty())
      			{
			    	std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
			    }
			    cv::imshow("video2", save_img2);

			    trackController.KeyDescripExtract(save_img2,keypointsImg,descriptorsImg);
			    trackController.DescripMatcher(descriptorsImg,descriptorsMask,matches);
			    trackController.DrawMatches(matches,keypointsImg,keypointsMask,save_img2,reference_img);
			    cv::waitKey(30);
       		}

			// Draw the matches
				// We need a new method to draw the matches in real time
				////*************************************************************************************			
				
				
			


		}

		////*************************************************************************************

		////***************************** Picture Section ***************************************
		else
		{

			
			// is the arg is equal to "default" that we use "./Reference.png"
			std::string imgPath = (std::string(argv[1]) == "default")? "./Reference.png": std::string(argv[1]);
			// Load the mask image
	    		const cv::Mat imgObj = cv::imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
			// Ask if the images are not empty
	   		if (!imgObj.data)
			{
				std::cout << " --(!) Error reading image " << std::endl; 
				return -1;
		    	}

			////***************************** Tracking section **************************************
			// Get the other descriptor and get the mathes
			trackController.KeyDescripExtract(imgObj,keypointsImg,descriptorsImg);
			trackController.DescripMatcher(descriptorsImg,descriptorsMask,matches);	

			// Draw the matches
			trackController.DrawMatches(matches,keypointsImg,keypointsMask,imgObj,maskObj);
			////*************************************************************************************

		}
		////*************************************************************************************


			
	}else{

		std::cout << "Error: incorrect use" << std::endl;
		help();
		return -1;
	}

  return 1;
  
}
