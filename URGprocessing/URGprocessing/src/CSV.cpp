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

using namespace std;
using namespace qrk;

std::vector<std::string> split(const std::string &str, char sep)
{
	std::vector<std::string> v;
	std::stringstream ss(str);
	std::string buffer;
	while (std::getline(ss, buffer, sep)) {
		v.push_back(buffer);
	}
	return v;
}

CSV::CSV()
{

}
vector<vector<long>> CSV::CSVloading(string address, double range)
{
	range /= 1000;
	//ファイルの読み込み
	ifstream ifs(address);
	if (!ifs) {
		cout << "入力エラー";
	}

	//csvファイルを1行ずつ読み込む
	string str;
	vector<vector<long>>csvdatas;
	while (ifs &&getline(ifs, str)) {
		//cout << str<<endl;
		vector<std::string>numbers = split(str, ',');
		vector<double>numbersD;
		vector<long>numbersL;

		for (int i = 0; i < numbers.size(); i++)
		{
			if (numbers[i] == "nan")
			{
				numbersD.push_back(range);
			}
			else
			{
				numbersD.push_back(stod(numbers[i]));
			}
			//cout << stod(numbers[i]) << endl;
		}
		for (int i = 0; i < numbers.size(); i++)
		{
			if (numbersD[i] < 0.02)
			{
				numbersD[i] = range;
			}
			if (numbersD[i] > range)
			{
				numbersD[i] = range;
			}

		}
		for (int i = 0; i < numbersD.size(); i++)
		{
			numbersL.push_back(long((double)1000 * numbersD[i]));
		}
		csvdatas.push_back(numbersL);
	}
	return csvdatas;

}
vector<long>CSV::CSVprocessing(vector<vector<long>>datas)
{
	long max = 5600;
	vector<long>data;
	for (int j = 0; j < datas[0].size(); j++)
	{
		long minval = max;
		for (int i = 0; i < datas.size(); i++)
		{
			minval = min(minval, datas[i][j]);
		}
		data.push_back(minval);
	}
	return data;
}

vector<long>CSV::CSVtoData(vector<long>data)
{
	int eracedataNum = 0;
	data.erase(data.begin(), data.begin() + 44);
	cout << data.size() << endl;
	eracedataNum = data.size() - 512;
	eracedataNum /= 2;

	data.erase(data.begin(), data.begin() + eracedataNum);
	data.erase(data.end() - eracedataNum, data.end());

	return data;
}

vector<vector<long>>CSV::OtomoToDatas(vector<vector<long>>datas)
{
	int eracedataNum = 0;
	for (int i = 0; i < datas.size(); i++)
	{
		datas[i].erase(datas[i].begin(), datas[i].begin() + 180);//時間情報の削除
		datas[i].erase(datas[i].end() - 180, datas[i].end());
	}
	return datas;
}

vector<long>CSV::CSVloadTime(vector<vector<long>>datas)
{
	vector<long>times;
	for (int i = 0; i < datas.size(); i++)
	{
		times.push_back(datas[i][0]);
	}
	return times;
}

void CSV::drawCSV(double data, double time)
{
	ofstream write_file;
	write_file.open("Test.csv", ios::app);
	write_file << ofToString(time, 3) + "," + ofToString(data, 3) << endl;
}