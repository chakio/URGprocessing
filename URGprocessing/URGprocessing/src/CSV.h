#pragma once
#include "ofMain.h"
#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include "urg_processing.h"
#include <iostream>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include <Eigen/Dense>

using namespace std;

class CSV {

public:

	CSV();
	vector<long>CSVprocessing(vector<vector<long>>datas);
	vector<vector<long>>CSVloading(string address, double range);
	vector<long>CSVloadTime(vector<vector<long>>datas);
	vector<long>CSVtoData(vector<long>data);
	vector<vector<long>>OtomoToDatas(vector<vector<long>>datas);
	void drawCSV(double data, double time);
};