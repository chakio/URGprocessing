#pragma once

#include "ofMain.h"
#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include "urg_processing.h"
#include "CSV.h"
#include <iostream>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <Eigen/Dense>
#include "ofxGui.h"

using namespace qrk;
using namespace std;


class ofApp : public ofBaseApp{
	
	public:
		ofApp(int argc, char *argv[]);
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		enum { Capture_times = 100 };
		Urg_driver urg;
		vector<long> data,data1,timedatas;
		vector<vector<long>>datas;//ローパスフィルター用
		vector<long>calibrationdata;//キャリブレーション用
		vector<unsigned short> intensity;
		vector<long> Ddata;
		vector<vector<double>>thingspos ;//x,y,width,dataの始まりの場所、終わりの場所
		void drawinformations(double range);
		void drawGraph(vector<double> humandirect,double valueWidth);
		vector<vector<double>> humandirects;
		ofTrueTypeFont font;
		vector<long> calibration(int sample);
		URG_processsing urg_processing;
		vector<vector<long>> csvdatas;
		vector<long> csvdata;
		bool URGconnecting = false;//URGが接続されている場合true,CSVをもとにする場合false
		bool otomoCSV = true;

		int csvNum = 0;
		CSV csv;

		int step = 0;
		//いいほう
		double URGRange[3] = {270,1080,30000};//角度、ステップ数、最大距離
		//普通のほう
		//double URGRange[3] = { 240,(double)1024/360*240,30000 };//角度、ステップ数、最大距離

		vector<vector<double>>humanpoints;//ヒトだと思わしき点の集まりx,y
		vector<ofPoint> Squarepoint;
		vector<double> EllipseElements;
		vector<double> LinearElements;
		vector<double> QuadraticElements;

		ofxFloatSlider framerate;
		ofxFloatSlider LinearE;
		ofxFloatSlider QuadraticE;

		ofxToggle thing;
		ofxToggle Linear;
		ofxToggle Quadratic;
		ofxToggle Ellipse;
		ofxToggle Square;
		ofxToggle Humandirect;
		ofxPanel gui;
		
		

};

