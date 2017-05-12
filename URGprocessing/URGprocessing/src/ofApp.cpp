#include "ofApp.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include<math.h>
#include <Eigen/Dense>
//using namespace qrk;
using namespace std;
using namespace Eigen;

ofApp::ofApp(int argc, char *argv[])
{
	if (URGconnecting)//URGが接続されている場合
	{
		// this kicks off the running of my app
		// can be OF_WINDOW or OF_FULLSCREEN
		// pass in width and height too:
		Connection_information information(argc, argv);
		// Connects to the sensor
		if (!urg.open(information.device_or_ip_name(),
			information.baudrate_or_port_number(),
			information.connection_type())) {
			cout << "Urg_driver::open(): "
				<< information.device_or_ip_name() << ": " << urg.what() << endl;
		}

		// Gets measurement data
#if 1
	// Case where the measurement range (start/end steps) is defined
		urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(+90), 0);//360digree/1024個 180度分で513個
#endif

		urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
	}
	
}
void ofApp::setup(){
	gui.setup(); // most of the time you don't need a name
	gui.add(framerate.setup("framerate"," " ));
	gui.add(LinearE.setup("LinearE", " "));
	gui.add(QuadraticE.setup("QuadraticE", " "));

	ofSetFrameRate(15);
	font.loadFont("Meiryo.ttf", 10);
	thingspos.clear();
	datas.clear();
	data.clear();
	csvdatas.clear();
	csvdata.clear();
	if (URGconnecting)//URGが接続されている場合
	{
		calibrationdata = calibration(50);//50回分の計測結果をもとにmapを作成
	}
	else
	{
		if (otomoCSV)
		{
			//csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs11.csv",URGRange[2]);//直進
			//csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs10.csv", URGRange[2]);//直進

			//csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs21.csv", URGRange[2]);//その場回転
			csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs20.csv", URGRange[2]);//その場回転

			//csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs31.csv", URGRange[2]);//大まわり
			//csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs30.csv", URGRange[2]);//大まわり
			csvdatas = csv.OtomoToDatas(csvdatas);
		}
		else
		{
			csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\roka.csv", URGRange[2]);
			csvdata = csv.CSVprocessing(csvdatas);
			cout << csvdata.size() << endl;
			data = csv.CSVtoData(csvdata);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	if (URGconnecting)
	{
		long time_stamp = 0;
		if (!urg.get_distance(data, &time_stamp)) {
			cout << "Urg_driver::get_distance(): " << urg.what() << endl;
		}
		data = urg_processing.limitprocessing(data, 200000, 20);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (URGconnecting)
	{
		ofSetColor(0, 0, 255);
		double Avedata = 0;
		ofPoint center(ofGetWidth() / 2, ofGetHeight());
		urg_processing.drawdata(data, (double)URGRange[0] / URGRange[1], URGRange[2], URGRange[2]);
		ofSetColor(0, 255, 0);
		urg_processing.drawdata(calibrationdata, (double)URGRange[0] / URGRange[1], URGRange[2], URGRange[2]);
		thingspos = urg_processing.findthings4(data, calibrationdata, 70);
	}
	else if (otomoCSV)
	{
		data = csvdatas[(int)ofGetElapsedTimeMillis()/50%(int)csvdatas.size()];
		//data = csvdatas[10];
		urg_processing.drawdata(data,(double)URGRange[0]/URGRange[1], URGRange[2],6000);
		thingspos = urg_processing.findthings5(data, 300, (double)URGRange[0] / URGRange[1], URGRange[2],200,2500,6000);//大きさ、x座標、y座標
	}
	else
	{
		ofSetColor(0, 255, 0);
		urg_processing.drawdata(data, (double)URGRange[0] / URGRange[1], URGRange[2], URGRange[2]);
		thingspos = urg_processing.findthings5(data, 70, (double)360/1024,URGRange[2] , 300, 2500, 4500);
	}

	
	QuadraticElements = urg_processing.QuadraticApproximation(humanpoints);
	urg_processing.drawQuadratic(QuadraticElements);
	QuadraticE = ofToString(QuadraticElements[3], 3);
	//urg_processing.drawthings(thingspos,6000);
	humanpoints=urg_processing.drawpoints(data, (double)URGRange[0] / URGRange[1],thingspos, 6000);
	Squarepoint=urg_processing.drawSquare(data, (double)URGRange[0] / URGRange[1], thingspos, 6000, QuadraticElements);


	urg_processing.drawLinear(Squarepoint);

	
	EllipseElements=urg_processing.EllipseApproximation(humanpoints);
	urg_processing.drawEllipse(EllipseElements);

	LinearElements = urg_processing.LinearApproximation(humanpoints);
	urg_processing.drawLinear(LinearElements);
	LinearE = ofToString(LinearElements[2], 3);
	
	

	ofSetColor(255, 0, 0);
	drawinformations(8000);
	
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
void ofApp::drawinformations(double range) {
	stringstream ss;
	ss << "framerate: " << ofToString(ofGetFrameRate(), 0);
	ofDrawBitmapString(ss.str(), 10, 20);

	framerate= ofToString(ofGetFrameRate(), 0);
	ofSetCircleResolution(50);
	ofNoFill();
	ofSetColor(0, 0, 0);
	ofSetLineWidth(3);

	for (int i = 1; i < 11; i++)
	{
		ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), (double)1000*i * ofGetHeight() / range);
		ss.str("");
		ss << to_string(1000*i);
		ss << "mm";
		font.drawString(ss.str(), ofGetWidth() / 2 - (double)1000 *i* ofGetHeight() / range, ofGetHeight() - 40);
	}
}
vector<long> ofApp::calibration(int sample)
{
	cout << "calibration start" << endl;
	vector<long> calibrationdata;
	vector<long> data;
	vector<vector<long>>calibrationdatas;
	long max = 200000;
	cout << "collectiong data" << endl;
	for (int i = 0; i <sample; i++)
	{
		long time_stamp = 0;
		if (!urg.get_distance(data, &time_stamp)) {
			cout << "Urg_driver::get_distance(): " << urg.what() << endl;
		}
		data = urg_processing.limitprocessing(data, max, 20);
		//最新データをベクターの最後に追加
		calibrationdatas.push_back(data);

	}
	//100回分の中で一番短い距離でマップを作る
	cout << "conparing data" << endl;
	for (int j = 0; j < calibrationdatas[0].size(); j++)
	{
		long minval = max;
		for (int i = 0; i < calibrationdatas.size(); i++)
		{
			minval = min(minval, calibrationdatas[i][j]);
		}
		calibrationdata.push_back(minval);
	}
	cout << "calibration done" << endl;

	return calibrationdata;
}


