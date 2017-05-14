#include "ofMain.h"
#include "ofApp.h"
#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include "urg_processing.h"
#include "CSV.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <Eigen/Dense>
//using namespace qrk;
//using namespace std;

//========================================================================

int main(int argc, char *argv[]){
	
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	//aaaa
	
	
	ofRunApp(new ofApp(argc, argv));
}
