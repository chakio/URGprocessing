#include "ofApp.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 

//using namespace qrk;
using namespace std;

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
			//csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs21.csv", URGRange[2]);//その場回転
			csvdatas = csv.CSVloading("C:\\Users\\kawasaki\\Source\\Repos\\URGprocessing\\URGprocessing\\URGprocessing\\bin\\data\\Lrs31.csv", URGRange[2]);//大まわり
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
		urg_processing.drawdata(data,(double)URGRange[0]/URGRange[1], URGRange[2],8000);
		thingspos = urg_processing.findthings5(data, 100, (double)URGRange[0] / URGRange[1], URGRange[2],100,2500,7000);//大きさ、x座標、y座標
	}
	else
	{
		ofSetColor(0, 255, 0);
		urg_processing.drawdata(data, (double)URGRange[0] / URGRange[1], URGRange[2], URGRange[2]);
		thingspos = urg_processing.findthings5(data, 70, (double)360/1024,URGRange[2] , 100, 2500, 5000);
	}

	/*ofSetColor(255, 0, 0);
	if (thingspos.size() > 0)
	{
		for (int i = 0; i < thingspos.size(); i++)
		{
			cout << thingspos[i][2] << endl;
		}
	}*/
	urg_processing.drawthings(thingspos,8000);
	ofSetColor(255, 0, 0);
	drawinformations(8000);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
	{
		if (csvdatas.size()-1 == step)
		{
			step = 0;
		}
		else
		{
			step++;
		}
	}
	else if (key == 'b')
	{
		if (step == 0)
		{
			step=csvdatas.size() ;
		}
		else
		{
			step--;
		}
	}
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

URG_processsing::URG_processsing()
{

}
vector<long> URG_processsing::limitprocessing(vector<long>data,int maxval,int minval)
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
		ofDrawLine(center, ofPoint(ofGetWidth() / 2 + data[i]/rangeVal  * cos((data.size() - i)*(double)step / 180 * M_PI  + M_PI), data[i]/ rangeVal * sin((data.size() - i)*(double)step / 180 * M_PI + M_PI) + ofGetHeight()));
		if (data[i] < range1)
		{
			ofSetColor(0, 255, 255);
			ofDrawCircle(ofPoint(ofGetWidth() / 2 + data[i] / rangeVal * cos((data.size() - i)*(double)step / 180 * M_PI  + M_PI), data[i] / rangeVal * sin((data.size() - i)*(double)step / 180 * M_PI + M_PI) + ofGetHeight()), 2);
		}
	}
}

vector<vector<double>> URG_processsing::findthings1(vector<long>data,int length)
{
	bool find=false;//見つけたフラグ
	double findpos[4] = { 0,0,0,0 };//物体が存在するであろう場所の平均値0:個数、1:そこまでの距離,2:x,3:y
	double findposnum=0;//いくつのレーザーを遮ったか
	double thinglength=0;//物体の横幅
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
				findpos[0] =((int)findpos[0]/ findposnum);//真ん中
				findpos[1] = data[findpos[0]];
				thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*data[findpos[0]+(int)findposnum/2]*2;
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
		findpos[1] = data[(int)findpos[0]] ;
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
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), length/2);
	

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
		if (thinglength > (double)length-20)
		{
			if (thinglength < (double)length+20)
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
		thinglength = (findpos[1]+(tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*0.35 / 180 * M_PI / 2)*2;
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
		findpos[2] = (findpos[1]+thinglength) * cos(findpos[0]);
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
		if (data[i] < range)//レーザーが短いかどうか
		{
			if (find)//データの長さの連続性の確認
			{
				if (abs(data[i] - data[i - 1]) < length)
				{
					findpos[0] += i;
					findposnum++;
					find = true;
				}
				else
				{
					find = true;
					findout = true;
				}
			}
			else
			{
				findpos[0] += i;
				findposnum++;
				find = true;
			}
		}
		else
		{
			if (find)
			{
				findout = true;
			}
		}
		if (findout)//物体の終点を見つけた後の処理
		{
			findpos[0] = ((int)findpos[0] / findposnum);//真ん中
			findpos[1] = data[findpos[0]];
			thinglength = (findpos[1] + (tan(findposnum* (double)step / 180 * M_PI / 2)*findpos[1]))*tan(findposnum* (double)step / 180 * M_PI / 2) * 2;
			findpos[0] *= (double)step*(M_PI/180);//物体の真ん中の角度(ラジアン)
			findpos[2] = (findpos[1] + thinglength / 2*0) * cos(findpos[0]);
			findpos[3] = (findpos[1] + thinglength / 2*0) * sin(findpos[0]);
			thingpos[0] = findpos[2];
			thingpos[1] = findpos[3];
			thingpos[2] = thinglength;
			thingposes.push_back(thingpos);
			
			find = false;
			findpos[0] = 0;
			findposnum = 0;
			findout = false;
		}
	}
	if (find)//最後まで物体の終点が来なかった場合に終点の処理をする
	{
		findpos[0] = (findpos[0] / findposnum);//真ん中
		findpos[1] = data[(int)findpos[0]];
		thinglength = (findpos[1] + (tan(findposnum* (double)step / 180 * M_PI / 2)*findpos[1]))*tan(findposnum*(double)step / 180 * M_PI / 2) * 2;
		findpos[0] *= (double)step*(M_PI / 180);//物体の真ん中の角度(ラジアン)
		findpos[2] = (findpos[1] + thinglength/2*0) * cos(findpos[0]);
		findpos[3] = (findpos[1] + thinglength/2*0) * sin(findpos[0]);
		thingpos[0] = findpos[2];
		thingpos[1] = findpos[3];
		thingpos[2] = thinglength;
		thingposes.push_back(thingpos);
		
	}
	
	for (int i = 0; i < thingposes.size(); i++)
	{
		bool eraseflag = true;
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
			i -= 1;
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
		if (thingposes[i][0] > -2500)
		{
			if (thingposes[i][0] < 2500)
			{
				if (thingposes[i][1] < 5000)
				{
					if (thingposes[i][2] > 90)
					{
						ofSetColor(255, 0, 0);
					}
					else
					{
						ofSetColor(255, 255, 255);
					}
				}
				else
				{
					ofSetColor(0, 0, 255);
				}
			}
			else
			{
				ofSetColor(0, 0, 255);
			}
		}
		else
		{
			ofSetColor(0, 0, 255);
		}
		ofDrawCircle(ofPoint((double)thingposes[i][0] / rangeVal + ofGetWidth() / 2, (double)-thingposes[i][1] / rangeVal + ofGetHeight()), (double)thingposes[i][2] / rangeVal / 2);
	}

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
		ave[i] /= datas.size() ;
	}
	//最新データを加味
	if (datas.size() > 1)
	{
		for (int i = 0; i < ave.size(); i++)
		{
			ave[i] += 9*datas[datas.size()-1][i];
			ave[i] /= 10;//平均と最新の比1:1
		}
	}
	return ave;
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
	long max=5600;
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
	data.erase(data.begin(), data.begin() +  44);
	cout << data.size() << endl;
	eracedataNum = data.size() - 512;
	eracedataNum /=2;
	
	data.erase(data.begin(), data.begin() + eracedataNum);
	data.erase(data.end() - eracedataNum, data.end());

	return data;
}

vector<vector<long>>CSV::OtomoToDatas(vector<vector<long>>datas)
{
	int eracedataNum = 0;
	for (int i = 0; i < datas.size(); i++)
	{
		datas[i].erase(datas[i].begin(), datas[i].begin()+180);//時間情報の削除
		datas[i].erase(datas[i].end() -180, datas[i].end());
	}
	return datas;
}