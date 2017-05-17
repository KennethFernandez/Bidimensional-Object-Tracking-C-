#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "CampbellRobson.h"

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
  cout << "usage: Campbell-Robson [option] [N] [option] [N] ... \n" << endl;  
  cout << "       -r|--rows <M>     the number of rows" 			<< endl;
  cout << "       -c|--columns <N>  the number of columns" 			<< endl;  
  cout << "       -l|--lineal       if the use what linear scale" 	<< endl;  
  cout << "       -x|--rowShow <N>  the number of row to show" 		<< endl;  
  cout << "       -h|--help        show this help\n"				<< endl;    
}


/******************************************************
 * 
 * Function to convert string to int with an argument Value
 * 
 * Inputs: 
 * 		char* argvValue: It's the value of the first Argument to compare
 * 		char* argvValue2: It's the value of the second argument, 
 * 							this was the one you want to transform
 * 		string option: It's the value of the first compare, example "-n"
 * 		string option2: It's the value of the second compare, example "--number"
 * 		int *value: The inout value of the function
 * 
 * Outputs:
 * 		-------
 * 
 * Restrinctions:
 * 		-------
 * 
 * **********************************************/
bool GetIntArgv(char* argvValue, char* argvValue2, string option,string option2, int *value){
	
	// We check the both options
	if (string(argvValue)==option || string(argvValue)==option2){
		
			// Try to transform the value to int
			try {*value = StrToInt(argvValue2);}
			catch(exception const & e){
				cout<<"error : " << e.what() <<endl;
				return false;
			}
			return true;
	}
	return false;
}


/******************************************************
 * 
 * Function to get the position of the argv
 * 
 * Inputs: 
 * 		char *argv[]: The array with the argument value
 * 		int argc: The size of argv
 * 		string options[]: The array with the options
 * 		int lenghtOptions: The size of the vector
 * 		vector<int> posOptions: <InOutput> An array with the position of the options
 * 
 * Outputs:
 * 		bool error: if was an error in the process
 * 
 * Restrinctions:
 * 		char * argv[]: size > 0
 * 		string options[]: size > 0
 * 		vector<int> posOptions: !null
 * 
 * **********************************************/
bool GetPositionArgv(char *argv[], int argc, string options[], int lenghtOptions, int posOptions[]){
	
	// Ask if it's even && bigger than 0
	if(lenghtOptions % 2 == 0 && lenghtOptions > 0){
	
		// A temp value for the position
		int tempPos = 0;
		
		// Iterate over all the options
		for (int i = 0; i < argc; i++)
		{
			for (int j = 0; j < lenghtOptions; j+=2)
			{
				if(options[j] == string(argv[i]) || options[j+1] == string(argv[i])){
					tempPos = j/2;
				}
			}
			
			// Ask if the user insert to times a command like "-r -r"
			if(posOptions[tempPos] != 0 && tempPos > 0){
				cout<< "error: invalid expression" <<endl;
				return true;
			}else{
				posOptions[tempPos] = i;
			}
			
			tempPos = 0;
		}
		
		return false;
		
	}else{
		
		cout<< "error: invalid number of options" <<endl;
		return true;
		
	}
	return true;
}


/******************************************************
 * 
 * The main function
 * 
 * **********************************************/
int main(int argc, char *argv[])
{

	// Set the default values
	int rows = 512, columns = 512, rowShow = -1;
	bool lineal = false;

	bool wasAnError = false;

	// Ask if we have arguments
	if (argc > 1) {
		
		// If the user is asked about info
		if (std::string(argv[1])=="-h" || std::string(argv[1])=="--help") {
			help();
			return EXIT_SUCCESS;
		}
		
		// The options of execution
		string options[] = {"-r", "--rows", "-c", "--columns", "-l", "--lineal","-x","--rowShow"};
		int posOptions[] = {0,0,0,0};
		int lenghtOptions = sizeof(options)/sizeof(options[0]);
		
		// We check if there is an error in the arguments
		bool errorGetPosition = GetPositionArgv(argv,argc,options,lenghtOptions,posOptions);
		
		// Check all the values and charge in the variables
		bool errorRows = (posOptions[0] != 0)?GetIntArgv(argv[posOptions[0]-1],argv[posOptions[0]],"-r","--rows",&rows):false;
		bool errorColumns = (posOptions[1] != 0)? GetIntArgv(argv[posOptions[1]],argv[posOptions[1]+1],"-c","--columns",&columns):false;
		lineal = (posOptions[2] != 0)? (string(argv[posOptions[2]])=="-l" || string(argv[posOptions[2]])=="--lineal"):false;
		bool errorRowShow = (posOptions[3] != 0)? GetIntArgv(argv[posOptions[3]],argv[posOptions[3]+1],"-x","--rowShow",&rowShow):false;

		// Check all the errors
		bool wasAnError = errorGetPosition && errorRows && errorColumns && lineal && errorRowShow;
	
	}
	
	if( !wasAnError || argc ==1){
			
		// Instanciate the object
		CampbellRobson generatorChart = CampbellRobson();
		
		// We get the image
		Mat img =  generatorChart.GenerateMatrix((float)rows,(float)columns,lineal);
		img = (rowShow != -1)?generatorChart.PlotGraph(img, columns, rowShow):img;
		
		// Ask if it's empty
		if(img.empty())
		{
			cerr << "The image is empty." << endl;
			return EXIT_FAILURE;
		}
		
		generatorChart.GenerateImage(img, "./CampbellRobson.jpg");
		
		// Show the image
		namedWindow("imagen",WINDOW_AUTOSIZE);
		imshow("imagen", img);
					
		waitKey(0);
		
		return EXIT_SUCCESS;
			
	}else{
		return EXIT_FAILURE;
	}

  return EXIT_SUCCESS;
  
}
