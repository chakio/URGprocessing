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
	ofPoint center(ofGetWidth() / 2, ofGetHeight());
	double rangeVal = range2 / ofGetHeight();
	for (int i = 0; i < data.size(); i++)
	{
		ofSetColor(0, 255, 0);
		ofDrawLine(center, ofPoint(ofGetWidth() / 2 + data[i] / rangeVal  * cos((data.size() - i)*(double)step / 180 * M_PI + M_PI), data[i] / rangeVal * sin((data.size() - i)*(double)step / 180 * M_PI + M_PI) + ofGetHeight()));
		if (data[i] < range1)
		{
			ofSetColor(0, 255, 255);
			ofDrawCircle(ofPoint(ofGetWidth() / 2 + data[i] / rangeVal * cos((data.size() - i)*(double)step / 180 * M_PI + M_PI), data[i] / rangeVal * sin((data.size() - i)*(double)step / 180 * M_PI + M_PI) + ofGetHeight()), 1);
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
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), length / 2);


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
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), length / 2);


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
	double rangeVal = range / ofGetHeight();
	ofNoFill();
	for (int i = 0; i < thingposes.size(); i++)
	{
		ofSetColor(255, 0, 0);	
		ofDrawCircle(ofPoint((double)thingposes[i][0] / rangeVal + ofGetWidth() / 2, (double)-thingposes[i][1] / rangeVal + ofGetHeight()), (double)thingposes[i][2] / rangeVal / 2);
	}

}

vector<vector<double>> URG_processsing::drawpoints(vector<long>data, double step, vector<vector<double>>thingposes, double range)
{
	vector<vector<double>>humanpoints;

	double rangeVal = range / ofGetHeight();
	ofFill();
	ofSetColor(255, 0, 0);

	for (int j = 0; j < (thingposes[0][4] - thingposes[0][3]); j++)
	{
		vector<double>humanpoint;
		int k = j + thingposes[0][3];
		ofDrawCircle(ofPoint(ofGetWidth() / 2 + data[k] / rangeVal  * cos((data.size() - k)*(double)step / 180 * M_PI + M_PI), data[k] / rangeVal * sin((data.size() - k)*(double)step / 180 * M_PI + M_PI) + ofGetHeight()), 2);
		humanpoint.push_back(ofGetWidth() / 2 + data[k] / rangeVal  * cos((data.size() - k)*(double)step / 180 * M_PI + M_PI));//x
		humanpoint.push_back(data[k] / rangeVal * sin((data.size() - k)*(double)step / 180 * M_PI + M_PI) + ofGetHeight());
		humanpoints.push_back(humanpoint);
	}
	//ofDrawCircle(ofPoint((double)thingposes[i][0] / rangeVal + ofGetWidth() / 2, (double)-thingposes[i][1] / rangeVal + ofGetHeight()), (double)thingposes[i][2] / rangeVal / 2);


	return humanpoints;
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
	EllipseElemnt[2] = atan(ansMatrix(0) / (1 - ansMatrix(1))) / 2;
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
	double LineElements[4] = { 0,0,0,0 };

	for (int e = 0; e < elements.size(); e++)
	{
		for (int i = 0; i < 4; i++)
		{
			LineElements[i] += elements[e][i];
		}
	}

	vector<double> LineElement;
	for (int i = 0; i < 2; i++)
	{
		LineElement.push_back(0);
	}
	LineElement[0] = (elements.size()*LineElements[3] - LineElements[0] * LineElements[1])
		/ (elements.size()*LineElements[2] - LineElements[0] * LineElements[0]);
	LineElement[1] = (LineElements[2] * LineElements[1] - LineElements[3] * LineElements[0])
		/ (elements.size()*LineElements[2] - LineElements[0] * LineElements[0]);
	return LineElement;
}

void  URG_processsing::drawLinear(vector<double>LinearElement)
{
	ofSetColor(0, 0, 255);
	for (int x = 0; x < ofGetWidth(); x++)
	{
		double y = LinearElement[0] * (double)x + LinearElement[1];
		ofDrawCircle(ofPoint(x, y), 1);
	}
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
	vector<double> QuadraticElemnt;
	for (int i = 0; i < 3; i++)
	{
		QuadraticElemnt.push_back(0);
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


	QuadraticElemnt[0] = ansMatrix(0);
	QuadraticElemnt[1] = ansMatrix(1);
	QuadraticElemnt[2] = ansMatrix(2);
	
	return QuadraticElemnt;
}

void URG_processsing::drawQuadratic(vector<double>QuadraticElement)
{
	ofSetColor(255, 255, 0);
	for (int x = 0; x < ofGetWidth(); x++)
	{
		double y = QuadraticElement[0] * (double)x*(double)x + QuadraticElement[1] * (double)x+ QuadraticElement[2];
		ofDrawCircle(ofPoint(x, y), 1);
	}
}