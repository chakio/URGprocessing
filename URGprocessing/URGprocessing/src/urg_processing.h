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
	void drawdata(vector<long>data, double step, double range1, double range2);
	vector<vector<double>> findthings1(vector<long>data, int length);//半径length内の物体を検知
	vector<vector<double>> findthings2(vector<long>data, int length);//大きさがlengthくらいのものを検知
	vector<vector<double>> findthings3(vector<long>data, vector<long>calibration);//マップをもとに検出
	vector<vector<double>> findthings4(vector<long>data, vector<long>calibration, int length);//マップをもとに,大きさがlengthくらいのものを検知
	vector<vector<double>> findthings5(vector<long>data, int length, double step, double range, double Width, double Xwidth, double Ywidth);//基準なしで大きさがlengthくらいのものを検知
	void drawthings(vector<vector<double>>, double range);
	vector<vector<double>> drawpoints(vector<long>data, double step, vector<vector<double>>thingposes, double range);
	vector<ofPoint> drawSquare(vector<long>data, double step, vector<vector<double>>thingposes, double range);
	vector<ofPoint> drawSquare(vector<long>data, double step, vector<vector<double>>thingposes, double range, vector<double>QuadraticElements);
	vector<long> lowpassfilter(vector<long>data, vector<vector<long>>datas);
	vector<double> EllipseApproximation(vector<vector<double>> humanpoints);
	vector<double> EllipseApproximation2(vector<vector<double>> humanpoints);
	void drawEllipse(vector<double>EllipseElements);
	vector<double> LinearApproximation(vector<vector<double>> humanpoints);
	void drawLinear(vector<double>EllipseElements);
	void drawLinear(vector<ofPoint>EllipseElements);
	vector<double> QuadraticApproximation(vector<vector<double>> humanpoints);
	void drawQuadratic(vector<double>QuadraticElements);
};