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

using namespace qrk;
using namespace std;
class URG_processsing {

public:
	Urg_driver urg;
	URG_processsing();
	vector<long>limitprocessing(vector<long>data, int maxval, int minval);
	void drawdata(vector<long>data);
	vector<vector<double>> findthings1(vector<long>data,int length);//���alength���̕��̂����m
	vector<vector<double>> findthings2(vector<long>data, int length);//�傫����length���炢�̂��̂����m
	vector<vector<double>> findthings3(vector<long>data, vector<long>calibration);//�}�b�v�����ƂɌ��o
	vector<vector<double>> findthings4(vector<long>data, vector<long>calibration, int length);//�}�b�v�����Ƃ�,�傫����length���炢�̂��̂����m
	void drawthings(vector<vector<double>>);
	vector<long> lowpassfilter(vector<long>data, vector<vector<long>>datas);
	
};
class CSV {

public:
	
	CSV();
	vector<long>CSVprocessing(vector<vector<long>>datas);
	vector<vector<long>>CSVloading(string address);
	vector<long>CSVtoData(vector<long>data);
	
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
		vector<vector<long>>datas;//���[�p�X�t�B���^�[�p
		vector<long>calibrationdata;//�L�����u���[�V�����p
		vector<unsigned short> intensity;
		vector<long> Ddata;
		vector<vector<double>>thingspos ;
		void drawinformations();
		ofTrueTypeFont font;
		vector<long> calibration(int sample);
		URG_processsing urg_processing;
		vector<vector<long>> csvdatas;
		vector<long> csvdata;
		bool URGconnecting = false;//URG���ڑ�����Ă���ꍇtrue,CSV�����Ƃɂ���ꍇfalse
		CSV csv;

		
};

