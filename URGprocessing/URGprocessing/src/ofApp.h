#pragma once

#include "ofMain.h"
#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include <iostream>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <Eigen/Dense>

using namespace qrk;
using namespace std;
class URG_processsing {

public:
	Urg_driver urg;
	URG_processsing();
	vector<long>limitprocessing(vector<long>data, int maxval, int minval);
	void drawdata(vector<long>data,double step,double range1, double range2);
	vector<vector<double>> findthings1(vector<long>data,int length);//半径length内の物体を検知
	vector<vector<double>> findthings2(vector<long>data, int length);//大きさがlengthくらいのものを検知
	vector<vector<double>> findthings3(vector<long>data, vector<long>calibration);//マップをもとに検出
	vector<vector<double>> findthings4(vector<long>data, vector<long>calibration, int length);//マップをもとに,大きさがlengthくらいのものを検知
	vector<vector<double>> findthings5(vector<long>data, int length,double step,double range,double Width,double Xwidth,double Ywidth);//基準なしで大きさがlengthくらいのものを検知
	void drawthings(vector<vector<double>>,double range);
	vector<long> lowpassfilter(vector<long>data, vector<vector<long>>datas);
	
};
class CSV {

public:
	
	CSV();
	vector<long>CSVprocessing(vector<vector<long>>datas);
	vector<vector<long>>CSVloading(string address,double range);
	vector<long>CSVtoData(vector<long>data);
	vector<vector<long>>OtomoToDatas(vector<vector<long>>datas);
};
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
		vector<long> data,data1;
		vector<vector<long>>datas;//ローパスフィルター用
		vector<long>calibrationdata;//キャリブレーション用
		vector<unsigned short> intensity;
		vector<long> Ddata;
		vector<vector<double>>thingspos ;
		void drawinformations(double range);
		ofTrueTypeFont font;
		vector<long> calibration(int sample);
		URG_processsing urg_processing;
		vector<vector<long>> csvdatas;
		vector<long> csvdata;
		bool URGconnecting = false;//URGが接続されている場合true,CSVをもとにする場合false
		bool otomoCSV = true;
		CSV csv;

		int step = 0;
		//いいほう
		double URGRange[3] = {270,1080,30000};//角度、ステップ数、最大距離
		//普通のほう
		//double URGRange[3] = { 240,(double)1024/360*240,30000 };//角度、ステップ数、最大距離
};

