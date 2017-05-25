#include "ConfigFile.h"

std::string configFilePath;

// The constructor of the class
ConfigFile::ConfigFile(const std::string path){

	configFilePath = path;

}

// To load the data
bool ConfigFile::LoadData(std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors){

	cv::FileStorage fs;
        fs.open(configFilePath, cv::FileStorage::READ);
	bool isOpen = fs.isOpened();

	if (isOpen)
        {
		
		cv::FileNode kpFileNode = fs["keypoints"];
  		read(kpFileNode, keypoints);
		fs["descriptors"]  >> descriptors;

	}

	fs.release();
	return isOpen;
        
}

//To save the data
bool ConfigFile::SaveData(const std::vector<cv::KeyPoint> keypoints, const cv::Mat descriptors){

	cv::FileStorage fs;
	fs.open(configFilePath, cv::FileStorage::WRITE);
	bool isOpen = fs.isOpened();

	if (isOpen)
        {

		fs << "keypoints" << keypoints;
		fs << "descriptors" << descriptors;
	}

	fs.release();
	return isOpen;


}
		
