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

using namespace Eigen;
using namespace qrk;
using namespace std;
URG_processsing::URG_processsing()
{

}
vector<long> URG_processsing::limitprocessing(vector<long>data, int maxval, int minval)
{
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] < minval)
		{
			data[i] = maxval;
		}
		if (data[i] > maxval)
		{
			data[i] = maxval;
		}
	}
	return data;
}

void URG_processsing::drawdata(vector<long>data, double step, double range1, double range2)
{
	ofPoint center(ofGetWidth() / 2, (ofGetHeight()*0.8));
	double rangeVal = range2 / (ofGetHeight()*0.8);
	for (int i = 0; i < data.size(); i++)
	{
		ofSetColor(0, 255, 0);
		ofDrawLine(center, ofPoint(ofGetWidth() / 2 + data[i] / rangeVal  * cos((data.size() - i)*(double)step / 180 * M_PI + M_PI), data[i] / rangeVal * sin((data.size() - i)*(double)step / 180 * M_PI + M_PI) + (ofGetHeight()*0.8)));
		if (data[i] < range1)
		{
			ofSetColor(0, 255, 255);
			ofDrawCircle(ofPoint(ofGetWidth() / 2 + data[i] / rangeVal * cos((data.size() - i)*(double)step / 180 * M_PI + M_PI), data[i] / rangeVal * sin((data.size() - i)*(double)step / 180 * M_PI + M_PI) + (ofGetHeight()*0.8)), 1);
		}
	}
}

vector<vector<double>> URG_processsing::findthings1(vector<long>data, int length)
{
	bool find = false;//見つけたフラグ
	double findpos[4] = { 0,0,0,0 };//物体が存在するであろう場所の平均値0:個数、1:そこまでの距離,2:x,3:y
	double findposnum = 0;//いくつのレーザーを遮ったか
	double thinglength = 0;//物体の横幅
	vector<vector<double>> thingposes;
	vector<double> thingpos;
	thingposes.clear();
	for (int i = 0; i < 4; i++)
	{
		thingpos.push_back(0);
	}
	for (int i = 0; i < data.size(); i++)
	{

		if (data[i] < length)
		{
			find = true;
			findpos[0] += i;
			findposnum++;
		}
		else
		{
			if (find)
			{
				findpos[0] = ((int)findpos[0] / findposnum);//真ん中
				findpos[1] = data[findpos[0]];
				thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*data[findpos[0] + (int)findposnum / 2] * 2;
				findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
				findpos[2] = findpos[1] * cos(findpos[0]);
				findpos[3] = findpos[1] * sin(findpos[0]);
				thingpos[0] = findpos[2];
				thingpos[1] = findpos[3];
				thingpos[2] = thinglength;
				thingposes.push_back(thingpos);

			}
			find = false;
			findpos[0] = 0;
			findposnum = 0;
		}

	}
	if (find)
	{
		findpos[0] = (findpos[0] / findposnum);//真ん中
		findpos[1] = data[(int)findpos[0]];
		thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*data[findpos[0] + (int)findposnum / 2] * 2;
		findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
		findpos[2] = findpos[1] * cos(findpos[0]);
		findpos[3] = findpos[1] * sin(findpos[0]);
		thingpos[0] = findpos[2];
		thingpos[1] = findpos[3];
		thingpos[2] = thinglength;
		thingposes.push_back(thingpos);

	}


	ofNoFill();
	ofSetColor(0, 255, 0);
	ofSetLineWidth(3);
	ofDrawCircle(ofPoint(ofGetWidth() / 2, (ofGetHeight()*0.8), 0), length / 2);


	return thingposes;
}
vector<vector<double>> URG_processsing::findthings2(vector<long>data, int length)
{
	bool find = false;//見つけたフラグ
	double findpos[4] = { 0,0,0,0 };//物体が存在するであろう場所の平均値0:個数、1:そこまでの距離,2:x,3:y
	double findposnum = 0;//いくつのレーザーを遮ったか
	double thinglength = 0;//物体の横幅
	vector<vector<double>> thingposes;
	vector<double> thingpos;
	thingposes.clear();
	for (int i = 0; i < 4; i++)
	{
		thingpos.push_back(0);
	}
	for (int i = 0; i < data.size(); i++)
	{

		if (data[i] < 500)
		{
			find = true;
			findpos[0] += i;
			findposnum++;
		}
		else
		{
			if (find)
			{
				findpos[0] = (findpos[0] / findposnum);//真ん中
				findpos[1] = data[(int)findpos[0]];
				thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1] * 2;
				findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
				findpos[2] = findpos[1] * cos(findpos[0]);
				findpos[3] = findpos[1] * sin(findpos[0]);
				thingpos[0] = findpos[2];
				thingpos[1] = findpos[3];
				thingpos[2] = thinglength;
				if (thinglength > (double)length*0.80)
				{
					if (thinglength < (double)length*1.20)
					{
						thingposes.push_back(thingpos);
					}
				}

			}
			find = false;
			findpos[0] = 0;
			findposnum = 0;
		}

	}
	if (find)
	{
		findpos[0] = (findpos[0] / findposnum);//真ん中
		findpos[1] = data[(int)findpos[0]];
		thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1] * 2;
		findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
		findpos[2] = findpos[1] * cos(findpos[0]);
		findpos[3] = findpos[1] * sin(findpos[0]);
		thingpos[0] = findpos[2];
		thingpos[1] = findpos[3];
		thingpos[2] = thinglength;
		if (thinglength > (double)length - 20)
		{
			if (thinglength < (double)length + 20)
			{
				thingposes.push_back(thingpos);
			}
		}

	}


	ofNoFill();
	ofSetColor(0, 255, 0);
	ofSetLineWidth(3);
	ofDrawCircle(ofPoint(ofGetWidth() / 2, (ofGetHeight()*0.8), 0), length / 2);


	return thingposes;
}
vector<vector<double>> URG_processsing::findthings3(vector<long>data, vector<long>calibration)
{
	bool find = false;//見つけたフラグ
	double findpos[4] = { 0,0,0,0 };//物体が存在するであろう場所の平均値0:個数、1:そこまでの距離,2:x,3:y
	double findposnum = 0;//いくつのレーザーを遮ったか
	double thinglength = 0;//物体の横幅
	double maxlength = 0;//物体を遮ってるレーザーのうちで一番長いもの
	double avelength = 0;
	vector<vector<double>> thingposes;
	vector<double> thingpos;
	thingposes.clear();
	for (int i = 0; i < 4; i++)
	{
		thingpos.push_back(0);
	}
	for (int i = 0; i < data.size(); i++)
	{

		if (data[i] < calibration[i])
		{
			find = true;
			findpos[0] += i;
			findposnum++;
			maxlength = max(maxlength, (double)data[i]);
			avelength += (double)data[i];

		}
		else
		{
			if (find)
			{
				avelength /= findposnum;
				findpos[0] = ((int)findpos[0] / findposnum);//真ん中
				findpos[1] = data[findpos[0]];
				thinglength = (findpos[1] + (tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*0.35 / 180 * M_PI / 2) * 2;
				findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
				findpos[2] = findpos[1] * cos(findpos[0]);
				findpos[3] = findpos[1] * sin(findpos[0]);
				thingpos[0] = findpos[2];
				thingpos[1] = findpos[3];
				thingpos[2] = thinglength;
				thingposes.push_back(thingpos);

			}
			find = false;
			findpos[0] = 0;
			findposnum = 0;
			maxlength = 0;
		}

	}
	if (find)
	{
		avelength /= findposnum;
		findpos[0] = (findpos[0] / findposnum);//真ん中
		findpos[1] = data[(int)findpos[0]];
		thinglength = (findpos[1] + (tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*0.35 / 180 * M_PI / 2) * 2;
		findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
		findpos[2] = findpos[1] * cos(findpos[0]);
		findpos[3] = findpos[1] * sin(findpos[0]);
		thingpos[0] = findpos[2];
		thingpos[1] = findpos[3];
		thingpos[2] = thinglength;
		thingposes.push_back(thingpos);

	}
	return thingposes;
}
vector<vector<double>> URG_processsing::findthings4(vector<long>data, vector<long>calibration, int length)
{
	bool find = false;//見つけたフラグ
	double findpos[4] = { 0,0,0,0 };//物体が存在するであろう場所の平均値0:個数、1:そこまでの距離,2:x,3:y
	double findposnum = 0;//いくつのレーザーを遮ったか
	double thinglength = 0;//物体の横幅
	vector<vector<double>> thingposes;
	vector<double> thingpos;
	thingposes.clear();
	for (int i = 0; i < 4; i++)
	{
		thingpos.push_back(0);
	}
	for (int i = 0; i < data.size(); i++)
	{

		if (data[i] < calibration[i])
		{
			find = true;
			findpos[0] += i;
			findposnum++;
		}
		else
		{
			if (find)
			{
				findpos[0] = ((int)findpos[0] / findposnum);//真ん中
				findpos[1] = data[findpos[0]];
				thinglength = (findpos[1] + (tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*0.35 / 180 * M_PI / 2) * 2;
				findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
				findpos[2] = (findpos[1] + thinglength) * cos(findpos[0]);
				findpos[3] = (findpos[1] + thinglength) * sin(findpos[0]);
				thingpos[0] = findpos[2];
				thingpos[1] = findpos[3];
				thingpos[2] = thinglength;
				thingposes.push_back(thingpos);
				/*if (thinglength > (double)length - 10)
				{
				if (thinglength < (double)length + 10)
				{
				thingposes.push_back(thingpos);
				}
				}*/

			}
			find = false;
			findpos[0] = 0;
			findposnum = 0;
		}

	}
	if (find)
	{
		findpos[0] = (findpos[0] / findposnum);//真ん中
		findpos[1] = data[(int)findpos[0]];
		thinglength = (findpos[1] + (tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*0.35 / 180 * M_PI / 2) * 2;
		findpos[0] *= 0.35 / 180 * M_PI;//物体の真ん中の角度(ラジアン)
		findpos[2] = (findpos[1] + thinglength) * cos(findpos[0]);
		findpos[3] = (findpos[1] + thinglength) * sin(findpos[0]);
		thingpos[0] = findpos[2];
		thingpos[1] = findpos[3];
		thingpos[2] = thinglength;
		thingposes.push_back(thingpos);
		/*if (thinglength > (double)length - 10)
		{
		if (thinglength < (double)length + 10)
		{
		thingposes.push_back(thingpos);
		}
		}*/

	}
	return thingposes;
}
vector<vector<double>> URG_processsing::findthings5(vector<long>data, int length, double step, double range, double Width, double Xwidth, double Ywidth)
{
	bool find = false;//見つけたフラグ
	bool findout = false;//物体の終点フラグ
	double findpos[6] = { 0,0,0,0,0,0 };//物体が存在するであろう場所の平均値0:個数、1:そこまでの距離,2:x,3:y,4:始まりのi,5:終わりのi
	double findposnum = 0;//いくつのレーザーを遮ったか
	double thinglength = 0;//物体の横幅
	vector<vector<double>> thingposes;
	vector<double> thingpos;
	thingposes.clear();
	for (int i = 0; i < 6; i++)
	{
		thingpos.push_back(0);
	}
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] < range)//レーザーが短いかどうか
		{
			if (find)//データの長さの連続性の確認
			{
				if (abs(data[i] - data[i - 1]) < length)
				{
					findpos[0] += i;
					findposnum++;
				}
				else
				{
					findout = true;
					findpos[5] =findpos[4]+ findposnum-1;
				}
			}
			else
			{
				findpos[0] = i;
				findposnum=1;
				find = true;
				findpos[4] = i;
			}
		}
		else
		{
			if (find)
			{
				findout = true;
				findpos[5] = findpos[4] + findposnum - 1;
			}
		}
		if (findout)//物体の終点を見つけた後の処理
		{
			findpos[0] = ((int)findpos[0] / findposnum);//真ん中
			findpos[1] = data[findpos[0]];
			thinglength = (findpos[1] + (tan(findposnum* (double)step / 180 * M_PI / 2)*findpos[1]))*tan(findposnum* (double)step / 180 * M_PI / 2) * 2;
			findpos[0] *= (double)step*(M_PI / 180);//物体の真ん中の角度(ラジアン)
			findpos[2] = (findpos[1] + thinglength / 2 * 0) * cos(findpos[0]);
			findpos[3] = (findpos[1] + thinglength / 2 * 0) * sin(findpos[0]);
			thingpos[0] = findpos[2];
			thingpos[1] = findpos[3];
			thingpos[2] = thinglength;
			thingpos[3] = findpos[4];
			thingpos[4] = findpos[5];
			thingposes.push_back(thingpos);

			find = false;
			findpos[0] = 0;
			findposnum = 0;
			findout = false;
			thingpos[3] = 0;
			thingpos[4]=0;
		}
	}
	if (find)//最後まで物体の終点が来なかった場合に終点の処理をする
	{
		findpos[0] = (findpos[0] / findposnum);//真ん中
		findpos[1] = data[(int)findpos[0]];
		thinglength = (findpos[1] + (tan(findposnum* (double)step / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*(double)step / 180 * M_PI / 2) * 2;
		findpos[0] *= (double)step*(M_PI / 180);//物体の真ん中の角度(ラジアン)
		findpos[2] = (findpos[1] + thinglength / 2 * 0) * cos(findpos[0]);
		findpos[3] = (findpos[1] + thinglength / 2 * 0) * sin(findpos[0]);
		thingpos[0] = findpos[2];
		thingpos[1] = findpos[3];
		thingpos[2] = thinglength;
		thingpos[3] = findpos[4];
		thingpos[4] = data.size()-1;
		thingposes.push_back(thingpos);

		findpos[0] = 0;
		findposnum = 0;
		thingpos[3] = 0;
		thingpos[4] = 0;
	}
	
	bool eraseflag = true;
	for (int i = 1; i < thingposes.size()+1; i++)
	{
		if (eraseflag )
		{
			i -= 1;
		}
		else
		{
			eraseflag = true;
		}

		if (abs(thingposes[i][0]) < Xwidth)
		{
			if (thingposes[i][1] < Ywidth)
			{
				if (thingposes[i][2] > Width)
				{
					eraseflag = false;//条件に合った物体
				}
			}
		}
		if (eraseflag)
		{
			thingposes.erase(thingposes.begin() + i);
		}
	}
	return thingposes;
}

void URG_processsing::drawthings(vector<vector<double>>thingposes, double range)
{
	double rangeVal = range / (ofGetHeight()*0.8);
	ofNoFill();
	for (int i = 0; i < thingposes.size(); i++)
	{
		ofSetColor(255, 0, 0);	
		ofDrawCircle(ofPoint((double)thingposes[i][0] / rangeVal + ofGetWidth() / 2, (double)-thingposes[i][1] / rangeVal + (ofGetHeight()*0.8)), (double)thingposes[i][2] / rangeVal / 2);
	}

}

vector<vector<double>> URG_processsing::drawpoints(vector<long>data, double step, vector<vector<double>>thingposes, double range)
{
	vector<vector<double>>humanpoints;

	double rangeVal = range / (ofGetHeight()*0.8);
	ofFill();
	ofSetColor(255, 0, 0);

	for (int j = 0; j < (thingposes[0][4] - thingposes[0][3]); j++)
	{
		vector<double>humanpoint;
		int k = j + thingposes[0][3];
		ofDrawCircle(ofPoint(ofGetWidth() / 2 + data[k] / rangeVal  * cos((data.size() - k)*(double)step / 180 * M_PI + M_PI), data[k] / rangeVal * sin((data.size() - k)*(double)step / 180 * M_PI + M_PI) + ((ofGetHeight()*0.8))), 2);
		humanpoint.push_back(ofGetWidth() / 2 + data[k] / rangeVal  * cos((data.size() - k)*(double)step / 180 * M_PI + M_PI));//x
		humanpoint.push_back(data[k] / rangeVal * sin((data.size() - k)*(double)step / 180 * M_PI + M_PI) + ofGetHeight()*0.8);
		humanpoints.push_back(humanpoint);
	}
	//ofDrawCircle(ofPoint((double)thingposes[i][0] / rangeVal + ofGetWidth() / 2, (double)-thingposes[i][1] / rangeVal + ((ofGetHeight()*0.8)*0.8)), (double)thingposes[i][2] / rangeVal / 2);
	
	
	
	return humanpoints;
}
vector<ofPoint> URG_processsing::drawSquare(vector<long>data, double step, vector<vector<double>>thingposes, double range)
{
	vector<vector<double>>humanpoints;

	double rangeVal = range / ((ofGetHeight()*0.8));
	ofFill();
	ofSetColor(0, 0, 0);
	vector<ofPoint> point;
	for (int i = 0; i < 4; i++)
	{
		point.push_back(ofPoint(0,0));
	}
	ofVec2f vec[4];
	point[0] = ofPoint(ofGetWidth() / 2 + data[thingposes[0][3]] / rangeVal  * cos((data.size() - thingposes[0][3])*(double)step / 180 * M_PI + M_PI), data[thingposes[0][3]] / rangeVal * sin((data.size() - thingposes[0][3])*(double)step / 180 * M_PI + M_PI) + (ofGetHeight()*0.8));
	point[1] = ofPoint(ofGetWidth() / 2 + data[thingposes[0][3] + (int)(thingposes[0][4] - thingposes[0][3]) / 2] / rangeVal  * cos((data.size() - thingposes[0][3] - (int)(thingposes[0][4] - thingposes[0][3]) / 2)*(double)step / 180 * M_PI + M_PI), data[thingposes[0][3] + (int)(thingposes[0][4] - thingposes[0][3]) / 2] / rangeVal * sin((data.size() - thingposes[0][3] - (int)(thingposes[0][4] - thingposes[0][3]) / 2)*(double)step / 180 * M_PI + M_PI) + (ofGetHeight()*0.8));
	point[2] = ofPoint(ofGetWidth() / 2 + data[thingposes[0][4]] / rangeVal  * cos((data.size() - thingposes[0][4])*(double)step / 180 * M_PI + M_PI), data[thingposes[0][4]] / rangeVal * sin((data.size() - thingposes[0][4])*(double)step / 180 * M_PI + M_PI) + (ofGetHeight()*0.8));
	vec[0] = point[2] - point[1];
	point[3] = vec[0] + point[0];
	for (int i = 0; i < 4; i++)
	{
		ofDrawCircle(point[i], 5);
	}
	vec[2] = point[2] - point[0];
	vec[3] = point[3] - point[1];

	vector<ofPoint> anspoint;
	for (int i = 0; i < 2; i++)
	{
		anspoint.push_back(ofPoint(0, 0));
	}

	if (vec[2].length() > vec[3].length())
	{
		//ofDrawLine(point[0], point[2]);
		anspoint[0] = point[0];
		anspoint[1] = point[2];
	}
	else
	{
		//ofDrawLine(point[1], point[3]);
		anspoint[0] = point[1];
		anspoint[1] = point[3];
	}
	

	return anspoint;
}

vector<ofPoint> URG_processsing::drawSquare(vector<long>data, double step, vector<vector<double>>thingposes, double range, vector<double>QuadraticElements)
{
	vector<vector<double>>humanpoints;

	double rangeVal = range / (ofGetHeight()*0.8);
	ofFill();
	ofSetColor(0, 0, 0);
	vector<ofPoint> point;
	for (int i = 0; i < 4; i++)
	{
		point.push_back(ofPoint(0, 0));
	}
	ofVec2f vec[4];

	long minval = 2000000;
	int minpos;
	for (int j = 0; j < (thingposes[0][4] - thingposes[0][3]); j++)
	{
		if (minval > min(minval, data[thingposes[0][3]+j]))
		{
			minval = min(minval, data[thingposes[0][3]+j]);
			minpos = j;
		}
	}
	//cout << minpos << endl;



	double x = ofGetWidth() / 2 + data[thingposes[0][3]] / rangeVal  * cos((data.size() - thingposes[0][3])*(double)step / 180 * M_PI + M_PI);
	double y = QuadraticElements[0] * (double)x*(double)x + QuadraticElements[1] * (double)x + QuadraticElements[2];
	point[0] = ofPoint(x, y);

	x = -1 * QuadraticElements[1] / (2 * QuadraticElements[0]);
	//x = ofGetWidth() / 2 + data[thingposes[0][3]+minpos] / rangeVal  * cos((data.size() - thingposes[0][3]-minpos )*(double)step / 180 * M_PI + M_PI);
	y = QuadraticElements[0] * (double)x*(double)x + QuadraticElements[1] * (double)x + QuadraticElements[2];
	point[1] = ofPoint(x, y);
	
	
	x = ofGetWidth() / 2 + data[thingposes[0][4]] / rangeVal  * cos((data.size() - thingposes[0][4])*(double)step / 180 * M_PI + M_PI), data[thingposes[0][4]] / rangeVal * sin((data.size() - thingposes[0][4])*(double)step / 180 * M_PI + M_PI) + (ofGetHeight()*0.8);
	y = QuadraticElements[0] * (double)x*(double)x + QuadraticElements[1] * (double)x + QuadraticElements[2];
	point[2] = ofPoint(x, y);
	vec[0] = point[2] - point[1];
	point[3] = vec[0] + point[0];
	for (int i = 0; i < 4; i++)
	{
		ofDrawCircle(point[i], 5);
	}
	vec[2] = point[2] - point[0];
	vec[3] = point[3] - point[1];

	vector<ofPoint> anspoint;
	for (int i = 0; i < 2; i++)
	{
		anspoint.push_back(ofPoint(0, 0));
	}

	if (vec[2].length()*0.7 > vec[3].length())
	{
		//ofDrawLine(point[0], point[2]);
		anspoint[0] = point[0];
		anspoint[1] = point[2];
	}
	else
	{
		//ofDrawLine(point[1], point[3]);
		anspoint[0] = point[1];
		anspoint[1] = point[3];
	}


	return anspoint;
}
vector<long> URG_processsing::lowpassfilter(vector<long>data, vector<vector<long>>datas)
{
	//平均用のメモリの確保
	vector<long>ave;
	for (int i = 0; i < data.size(); i++)
	{
		ave.push_back(0);
	}

	//平均を求めるため各配列の足し算(最新データは平均に使わない)
	for (int i = 0; i < datas.size(); i++)
	{
		for (int j = 0; j < datas[i].size(); j++)
		{
			ave[j] += datas[i][j];
		}
	}
	//平均の計算
	for (int i = 0; i < ave.size(); i++)
	{
		ave[i] /= datas.size();
	}
	//最新データを加味
	if (datas.size() > 1)
	{
		for (int i = 0; i < ave.size(); i++)
		{
			ave[i] += 9 * datas[datas.size() - 1][i];
			ave[i] /= 10;//平均と最新の比1:1
		}
	}
	return ave;
}
vector<double> URG_processsing::EllipseApproximation(vector<vector<double>> humanpoints)
{
	vector<vector<vector<double>>> elements;
	vector<double>ele;
	for (int i = 0; i < 6; i++)
	{
		ele.push_back(0);
	}

	MatrixXd Matrix(5, 5), MatrixInverse(5, 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Matrix(i, j) = 0;
		}
	}
	VectorXd subMatrix(5);
	for (int i = 0; i < 5; i++)
	{
		subMatrix(i) = 0;
	}

	VectorXd ansMatrix(5);
	vector<double> EllipseElemnt;
	for (int i = 0; i < 5; i++)
	{
		EllipseElemnt.push_back(0);
	}

	for (int i = 0; i<humanpoints.size(); i++)
	{
		vector<vector<double>> element;
		double Xi = humanpoints[i][0];
		double Yi = humanpoints[i][1];
		ele[0] = pow(Xi, 2)*pow(Yi, 2);
		ele[1] = pow(Xi, 1)*pow(Yi, 3);
		ele[2] = pow(Xi, 2)*pow(Yi, 1);
		ele[3] = pow(Xi, 1)*pow(Yi, 2);
		ele[4] = pow(Xi, 1)*pow(Yi, 1);
		ele[5] = -pow(Xi, 3)*pow(Yi, 1);
		element.push_back(ele);

		ele[0] = pow(Xi, 1)*pow(Yi, 3);
		ele[1] = pow(Xi, 0)*pow(Yi, 4);
		ele[2] = pow(Xi, 1)*pow(Yi, 2);
		ele[3] = pow(Xi, 0)*pow(Yi, 3);
		ele[4] = pow(Xi, 0)*pow(Yi, 2);
		ele[5] = -pow(Xi, 2)*pow(Yi, 2);
		element.push_back(ele);

		ele[0] = pow(Xi, 2)*pow(Yi, 1);
		ele[1] = pow(Xi, 1)*pow(Yi, 2);
		ele[2] = pow(Xi, 2)*pow(Yi, 0);
		ele[3] = pow(Xi, 1)*pow(Yi, 1);
		ele[4] = pow(Xi, 1)*pow(Yi, 0);
		ele[5] = -pow(Xi, 3)*pow(Yi, 0);
		element.push_back(ele);

		ele[0] = pow(Xi, 1)*pow(Yi, 2);
		ele[1] = pow(Xi, 0)*pow(Yi, 3);
		ele[2] = pow(Xi, 1)*pow(Yi, 1);
		ele[3] = pow(Xi, 0)*pow(Yi, 2);
		ele[4] = pow(Xi, 0)*pow(Yi, 1);
		ele[5] = -pow(Xi, 2)*pow(Yi, 1);
		element.push_back(ele);

		ele[0] = pow(Xi, 1)*pow(Yi, 1);
		ele[1] = pow(Xi, 0)*pow(Yi, 2);
		ele[2] = pow(Xi, 1)*pow(Yi, 0);
		ele[3] = pow(Xi, 0)*pow(Yi, 1);
		ele[4] = pow(Xi, 0)*pow(Yi, 0);
		ele[5] = -pow(Xi, 2)*pow(Yi, 0);
		element.push_back(ele);

		elements.push_back(element);
	}
	for (int e = 0; e < elements.size(); e++)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Matrix(i, j) += elements[e][i][j];
			}
			subMatrix(i) += elements[e][i][5];
		}
	}
	MatrixInverse = Matrix.inverse();
	ansMatrix = MatrixInverse*subMatrix;


	EllipseElemnt[0] = (ansMatrix(0)*ansMatrix(3) - 2 * ansMatrix(1)*ansMatrix(2)) / (4 * ansMatrix(1) - ansMatrix(0)*ansMatrix(0));
	EllipseElemnt[1] = (ansMatrix(0)*ansMatrix(2) - 2 * ansMatrix(3)) / (4 * ansMatrix(1) - ansMatrix(0)*ansMatrix(0));
	EllipseElemnt[2] = atan(ansMatrix(0) / (1 - ansMatrix(1)) / 2);
	EllipseElemnt[3] = sqrt(pow(EllipseElemnt[0] * cos(EllipseElemnt[2]) + EllipseElemnt[1] * sin(EllipseElemnt[2]), 2)

		- ansMatrix[4] * pow(cos(EllipseElemnt[2]), 2)

		- ((pow(EllipseElemnt[0] * sin(EllipseElemnt[2]) - EllipseElemnt[1] * cos(EllipseElemnt[2]), 2) - ansMatrix[4] * pow(sin(EllipseElemnt[2]), 2))
			*(pow(sin(EllipseElemnt[2]), 2) - ansMatrix[1] * pow(cos(EllipseElemnt[2]), 2))
			/ (pow(cos(EllipseElemnt[2]), 2) - ansMatrix[1] * pow(sin(EllipseElemnt[2]), 2))));

	EllipseElemnt[4] = sqrt(pow(EllipseElemnt[0] * sin(EllipseElemnt[2]) - EllipseElemnt[1] * cos(EllipseElemnt[2]), 2)

		- ansMatrix[4] * pow(sin(EllipseElemnt[2]), 2)

		- (pow(EllipseElemnt[0] * cos(EllipseElemnt[2]) + EllipseElemnt[1] * sin(EllipseElemnt[2]), 2) - ansMatrix[4] * pow(cos(EllipseElemnt[2]), 2))
		*(pow(cos(EllipseElemnt[2]), 2) - ansMatrix[1] * pow(sin(EllipseElemnt[2]), 2))
		/ (pow(sin(EllipseElemnt[2]), 2) - ansMatrix[1] * pow(cos(EllipseElemnt[2]), 2)));

	return EllipseElemnt;
}

vector<double> URG_processsing::EllipseApproximation2(vector<vector<double>> humanpoints)
{
	vector<vector<vector<double>>> elements;
	vector<double>ele;
	for (int i = 0; i < 6; i++)
	{
		ele.push_back(0);
	}

	MatrixXd Matrix(5, 5), MatrixInverse(5, 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Matrix(i, j) = 0;
		}
	}
	VectorXd subMatrix(5);
	for (int i = 0; i < 5; i++)
	{
		subMatrix(i) = 0;
	}

	VectorXd ansMatrix(5);
	vector<double> EllipseElemnt;
	for (int i = 0; i < 5; i++)
	{
		EllipseElemnt.push_back(0);
	}

	for (int i = 0; i<humanpoints.size(); i++)
	{
		vector<vector<double>> element;
		double Xi = humanpoints[i][0];
		double Yi = humanpoints[i][1];
		ele[0] = pow(Xi, 2)*pow(Yi, 2);
		ele[1] = pow(Xi, 1)*pow(Yi, 3);
		ele[2] = pow(Xi, 2)*pow(Yi, 1);
		ele[3] = pow(Xi, 1)*pow(Yi, 2);
		ele[4] = pow(Xi, 1)*pow(Yi, 1);
		ele[5] = -pow(Xi, 3)*pow(Yi, 1);
		element.push_back(ele);

		ele[0] = pow(Xi, 1)*pow(Yi, 3);
		ele[1] = pow(Xi, 0)*pow(Yi, 4);
		ele[2] = pow(Xi, 1)*pow(Yi, 2);
		ele[3] = pow(Xi, 0)*pow(Yi, 3);
		ele[4] = pow(Xi, 0)*pow(Yi, 2);
		ele[5] = -pow(Xi, 2)*pow(Yi, 2);
		element.push_back(ele);

		ele[0] = pow(Xi, 2)*pow(Yi, 1);
		ele[1] = pow(Xi, 1)*pow(Yi, 2);
		ele[2] = pow(Xi, 2)*pow(Yi, 0);
		ele[3] = pow(Xi, 1)*pow(Yi, 1);
		ele[4] = pow(Xi, 1)*pow(Yi, 0);
		ele[5] = -pow(Xi, 3)*pow(Yi, 0);
		element.push_back(ele);

		ele[0] = pow(Xi, 1)*pow(Yi, 2);
		ele[1] = pow(Xi, 0)*pow(Yi, 3);
		ele[2] = pow(Xi, 1)*pow(Yi, 1);
		ele[3] = pow(Xi, 0)*pow(Yi, 2);
		ele[4] = pow(Xi, 0)*pow(Yi, 1);
		ele[5] = -pow(Xi, 2)*pow(Yi, 1);
		element.push_back(ele);

		ele[0] = pow(Xi, 1)*pow(Yi, 1);
		ele[1] = pow(Xi, 0)*pow(Yi, 2);
		ele[2] = pow(Xi, 1)*pow(Yi, 0);
		ele[3] = pow(Xi, 0)*pow(Yi, 1);
		ele[4] = pow(Xi, 0)*pow(Yi, 0);
		ele[5] = -pow(Xi, 2)*pow(Yi, 0);
		element.push_back(ele);

		elements.push_back(element);
	}
	for (int e = 0; e < elements.size(); e++)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Matrix(i, j) += elements[e][i][j];
			}
			subMatrix(i) += elements[e][i][5];
		}
	}
	MatrixInverse = Matrix.inverse();
	ansMatrix = MatrixInverse*subMatrix;

	EllipseElemnt[2] = atan(ansMatrix(0) / (1 - ansMatrix(1)) / 2);


	EllipseElemnt[0] = (ansMatrix(0)*ansMatrix(3) + 2 * ansMatrix(1)*ansMatrix(2)) / (-4 * ansMatrix(1) + ansMatrix(0)*ansMatrix(0));
	EllipseElemnt[1] = (ansMatrix(0)*ansMatrix(2) - 2 * ansMatrix(3)) / (4 * ansMatrix(1) - ansMatrix(0)*ansMatrix(0));
	
	EllipseElemnt[3] = sqrt(pow(EllipseElemnt[0] * cos(EllipseElemnt[2]) + EllipseElemnt[1] * sin(EllipseElemnt[2]), 2)

		- ansMatrix[4] * pow(cos(EllipseElemnt[2]), 2)

		- ((pow(EllipseElemnt[0] * sin(EllipseElemnt[2]) - EllipseElemnt[1] * cos(EllipseElemnt[2]), 2) - ansMatrix[4] * pow(sin(EllipseElemnt[2]), 2))
			*(pow(sin(EllipseElemnt[2]), 2) - ansMatrix[1] * pow(cos(EllipseElemnt[2]), 2))
			/ (pow(cos(EllipseElemnt[2]), 2) - ansMatrix[1] * pow(sin(EllipseElemnt[2]), 2))));

	EllipseElemnt[4] = sqrt((1 + 3 * sin(EllipseElemnt[2] * sin(EllipseElemnt[2]) / 4))*(pow(EllipseElemnt[0], 2) + EllipseElemnt[1] * ansMatrix[1] - EllipseElemnt[0] * EllipseElemnt[1] * ansMatrix[0] - ansMatrix[4]));
	EllipseElemnt[3]= EllipseElemnt[4]*2;
	/*EllipseElemnt[0] = (ansMatrix[1] + 3 * ansMatrix[1] * cos(EllipseElemnt[2])*cos(EllipseElemnt[2]) + 3 * ansMatrix[2])
						/ (10 - 18 * sin(EllipseElemnt[2])*sin(EllipseElemnt[2])*cos(EllipseElemnt[2])*cos(EllipseElemnt[2]));

	EllipseElemnt[1] = (ansMatrix[1] + 2 * EllipseElemnt[0] + 6 * EllipseElemnt[0] * sin(EllipseElemnt[2])*sin(EllipseElemnt[2])) / 6;
		
	EllipseElemnt[3] = sqrt(EllipseElemnt[0] * EllipseElemnt[0] * (1 + 3 * sin(EllipseElemnt[2])*sin(EllipseElemnt[2]))
						+ EllipseElemnt[1] * EllipseElemnt[1] * (1 + 3 * cos(EllipseElemnt[2])*cos(EllipseElemnt[2]))
						- 6 * EllipseElemnt[0] * EllipseElemnt[1] + sin(EllipseElemnt[2])*cos(EllipseElemnt[2]));

	EllipseElemnt[4] = EllipseElemnt[3] / 2;*/

	

	//cout << EllipseElemnt[2] << endl;
	
	return EllipseElemnt;
}

void  URG_processsing::drawEllipse(vector<double>EllipseElement)
{
	if (EllipseElement[3] > 0 && EllipseElement[4] > 0)
	{
		for (int i = 0; i < 360; i++)
		{
			ofSetColor(255, 255, 255);
			double t = (double)i / 180 * M_PI;
			double X = EllipseElement[3] * cos(t)*cos(EllipseElement[2]) - EllipseElement[4] * sin(t)*sin(EllipseElement[2]) + EllipseElement[0];
			double Y = EllipseElement[3] * cos(t)*sin(EllipseElement[2]) - EllipseElement[4] * sin(t)*cos(EllipseElement[2]) + EllipseElement[1];
			ofCircle(ofPoint(X, Y), 2);
		}
	}
}
vector<double> URG_processsing::LinearApproximation(vector<vector<double>> humanpoints)
{
	vector<vector<double>> elements;
	vector<double>element;
	for (int i = 0; i < 4; i++)
	{
		element.push_back(0);
	}

	for (int i = 0; i<humanpoints.size(); i++)
	{
		double Xi = humanpoints[i][0];
		double Yi = humanpoints[i][1];
		element[0] = pow(Xi, 1)*pow(Yi, 0);
		element[1] = pow(Xi, 0)*pow(Yi, 1);
		element[2] = pow(Xi, 2)*pow(Yi, 0);
		element[3] = pow(Xi, 1)*pow(Yi, 1);
		elements.push_back(element);
	}
	double LineElements[5] = { 0,0,0,0 };

	for (int e = 0; e < elements.size(); e++)
	{
		for (int i = 0; i < 4; i++)
		{
			LineElements[i] += elements[e][i];
		}
	}

	vector<double> LineElement;
	for (int i = 0; i < 3; i++)
	{
		LineElement.push_back(0);
	}
	LineElement[0] = (elements.size()*LineElements[3] - LineElements[0] * LineElements[1])
		/ (elements.size()*LineElements[2] - LineElements[0] * LineElements[0]);
	LineElement[1] = (LineElements[2] * LineElements[1] - LineElements[3] * LineElements[0])
		/ (elements.size()*LineElements[2] - LineElements[0] * LineElements[0]);

	for (int i = 0; i<humanpoints.size(); i++)
	{
		double Xi = humanpoints[i][0];
		double Yi = humanpoints[i][1];
		LineElement[2] += abs(Yi - LineElement[0] * Xi - LineElement[1]);
	}

	return LineElement;
}

void  URG_processsing::drawLinear(vector<double>LinearElement)
{
	ofSetColor(0, 0, 255);

	ofPoint oldpoint = ofPoint(0, LinearElement[1]);
	for (int x = 1; x < ofGetWidth(); x++)
	{
		double y = LinearElement[0] * (double)x + LinearElement[1];
		ofDrawLine(ofPoint(x, y), oldpoint);
		oldpoint = ofPoint(x, y);
	}
	
}
double  URG_processsing::drawLinear(vector<ofPoint>Element,vector<double>humandirects)
{
	ofSetColor(255, 0, 0);
	double A = (Element[1].y - Element[0].y) / (Element[1].x - Element[0].x);
	double B = Element[1].y - A*Element[1].x;
	ofPoint oldpoint=ofPoint(0,B);
	for (int x = 1; x < ofGetWidth(); x++)
	{
		double y = A* (double)x + B;
		ofDrawLine(ofPoint(x, y), oldpoint);
		oldpoint = ofPoint(x, y);
	}
	double direct = atan(A) / M_PI * 180;

	double directpos=0;

	if (humandirects[humandirects.size() - 1] > 150)
	{
		directpos = 5;
	}
	else if (humandirects[humandirects.size() - 1]>120)
	{
		directpos = 4;
	}
	else if (humandirects[humandirects.size() - 1]> 90)
	{
		directpos = 3;
	}
	else if (humandirects[humandirects.size() - 1] > 60)
	{
		directpos = 2;
	}
	else if(humandirects[humandirects.size() - 1]>30)
	{
		directpos = 1;
	}
	else if (humandirects[humandirects.size() - 1]> 0)
	{
		directpos = 0;
	}
	else if (humandirects[humandirects.size() - 1]> -30)
	{
		directpos = 11;
	}
	else if (humandirects[humandirects.size() - 1] > -60)
	{
		directpos = 10;
	}
	else if (humandirects[humandirects.size() - 1] > -90)
	{
		directpos = 9;
	}
	else if (humandirects[humandirects.size() - 1]> -120)
	{
		directpos = 8;
	}
	else if (humandirects[humandirects.size() - 1] > -150)
	{
		directpos = 7;
	}
	else if (humandirects[humandirects.size() - 1] > -180)
	{
		directpos = 6;
	}



	if (direct> 60)
	{
		if (directpos == 3 || directpos == 2 || directpos == 1)
		{
			//そのまま
		}
		else
		{
			direct -= 180;
		}
	}
	else if (direct>30)
	{
		if (directpos == 2 || directpos == 1 || directpos == 0)
		{
			//そのまま
		}
		else
		{
			direct -= 180;
		}
	}
	else if (direct> 0)
	{
		if (directpos == 1 || directpos == 0 || directpos == 11)
		{
			//そのまま
		}
		else
		{
			direct -= 180;
		}
	}
	else if (direct> -30)
	{
		if (directpos == 0 || directpos == 11 || directpos == 10)
		{
			//そのまま
		}
		else
		{
			direct += 180;
		}
	}
	else if (direct> -60)
	{
		if (directpos == 11 || directpos == 10 || directpos == 9)
		{
			//そのまま
		}
		else
		{
			direct += 180;
		}
	}
	else if (direct > -90)
	{
		if (directpos == 10 || directpos == 9 || directpos == 8)
		{
			//そのまま
		}
		else
		{
			direct += 180;
		}
	}
	cout << directpos << endl;

	
	return direct ;

}

vector<double> URG_processsing::QuadraticApproximation(vector<vector<double>> humanpoints)
{
	vector<vector<vector<double>>> elements;
	vector<double>ele;
	for (int i = 0; i < 4; i++)
	{
		ele.push_back(0);
	}

	MatrixXd Matrix(3, 3), MatrixInverse(3, 3);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix(i, j) = 0;
		}
	}
	VectorXd subMatrix(3);
	for (int i = 0; i < 3; i++)
	{
		subMatrix(i) = 0;
	}

	VectorXd ansMatrix(3);
	vector<double> QuadraticElement;
	for (int i = 0; i < 4; i++)
	{
		QuadraticElement.push_back(0);
	}

	for (int i = 0; i<humanpoints.size(); i++)
	{
		vector<vector<double>> element;
		double Xi = humanpoints[i][0];
		double Yi = humanpoints[i][1];
		ele[0] = pow(Xi, 4)*pow(Yi, 0);
		ele[1] = pow(Xi, 3)*pow(Yi, 0);
		ele[2] = pow(Xi, 2)*pow(Yi, 0);
		ele[3] = pow(Xi, 2)*pow(Yi, 1);
		element.push_back(ele);

		ele[0] = pow(Xi, 3)*pow(Yi, 0);
		ele[1] = pow(Xi, 2)*pow(Yi, 0);
		ele[2] = pow(Xi, 1)*pow(Yi, 0);
		ele[3] = pow(Xi, 1)*pow(Yi, 1);
		element.push_back(ele);

		ele[0] = pow(Xi, 2)*pow(Yi, 0);
		ele[1] = pow(Xi, 1)*pow(Yi, 0);
		ele[2] = pow(Xi, 0)*pow(Yi, 0);
		ele[3] = pow(Xi, 0)*pow(Yi, 1);
		element.push_back(ele);
		elements.push_back(element);
	}
	for (int e = 0; e < elements.size(); e++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Matrix(i, j) += elements[e][i][j];
			}
			subMatrix(i) += elements[e][i][3];
		}
	}
	MatrixInverse = Matrix.inverse();
	ansMatrix = MatrixInverse*subMatrix;


	QuadraticElement[0] = ansMatrix(0);
	QuadraticElement[1] = ansMatrix(1);
	QuadraticElement[2] = ansMatrix(2);
	

	for (int i = 0; i<humanpoints.size(); i++)
	{
		double Xi = humanpoints[i][0];
		double Yi = humanpoints[i][1];
		QuadraticElement[3] += abs(Yi - QuadraticElement[0] * Xi *Xi - QuadraticElement[1] * Xi - QuadraticElement[2]);
	}


	return QuadraticElement;
}

void URG_processsing::drawQuadratic(vector<double>QuadraticElement)
{
	ofSetColor(255, 255, 0);
	
	

	ofPoint oldpoint = ofPoint(0, QuadraticElement[2]);
	for (int x = 1; x < ofGetWidth(); x++)
	{
		double y = QuadraticElement[0] * (double)x*(double)x + QuadraticElement[1] * (double)x + QuadraticElement[2];
		ofDrawLine(ofPoint(x, y), oldpoint);
		oldpoint = ofPoint(x, y);
	}
}