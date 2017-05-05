#include "ofApp.h"


//using namespace qrk;
using namespace std;


ofApp::ofApp(int argc, char *argv[])
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
void ofApp::setup(){
	ofSetFrameRate(15);
	font.loadFont("Meiryo.ttf", 20);
	thingspos.clear();
	datas.clear();
}

//--------------------------------------------------------------
void ofApp::update() {
	long time_stamp = 0;
	if (!urg.get_distance(data, &time_stamp)) {
		cout << "Urg_driver::get_distance(): " << urg.what() << endl;
	}
	data=urg_processing.limitprocessing(data, 200000, 20);
	
	//10個以上はためないようにする
	if (datas.size() > 3)
	{
		datas.erase(datas.begin());
	}
	//最新データをベクターの最後に追加
	datas.push_back(data);

	data = urg_processing.lowpassfilter(data, datas);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetColor(0, 0, 255);
	double Avedata = 0;
	ofPoint center(ofGetWidth() / 2, ofGetHeight());
	urg_processing.drawdata(data);
	
	//cout << thingspos.size() << endl;
	thingspos = urg_processing.findthings(data, 300);
	urg_processing.drawthings(thingspos);
 	ofSetColor(255, 0, 0);
	drawinformations();

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
void ofApp::drawinformations() {
	stringstream ss;
	ss << "framerate: " << ofToString(ofGetFrameRate(), 0);
	ofDrawBitmapString(ss.str(), 10, 20);

	ofSetCircleResolution(50);
	ofNoFill();
	ofSetColor(0, 0, 0);
	ofSetLineWidth(3);
	
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), 250);
	ss.str("");
	ss << "500mm" ;
	font.drawString(ss.str(), ofGetWidth() / 2-240, ofGetHeight()-40);
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), 500);
	ss.str("");
	ss << "1000mm";
	font.drawString(ss.str(), ofGetWidth() / 2 - 470, ofGetHeight() - 40);
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), 750);
	ss.str("");
	ss << "1500mm";
	font.drawString(ss.str(), ofGetWidth() / 2 - 720, ofGetHeight() - 40);

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

void URG_processsing::drawdata(vector<long>data)
{
	ofPoint center(ofGetWidth() / 2, ofGetHeight());
	for (int i = 0; i < data.size(); i++)
	{
		ofDrawLine(center, ofPoint(ofGetWidth() / 2 + data[i]/2  * cos((data.size() - i)*0.35 / 180 * M_PI + M_PI), data[i]/2 * sin((data.size() - i)*0.35 / 180 * M_PI + M_PI) + ofGetHeight()));

	}
}

vector<vector<double>> URG_processsing::findthings(vector<long>data,int length)
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
				findpos[0] =(findpos[0]/ findposnum);//真ん中
				findpos[1] = data[(int)findpos[0]]/2;
				thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1];
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
		findpos[1] = data[(int)findpos[0]] / 2;
		thinglength = tan(findposnum*0.35 / 180 * M_PI / 2)*findpos[1];
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

void URG_processsing::drawthings(vector<vector<double>>thingposes)
{
	ofSetColor(255, 0, 0);
	ofFill();
	for (int i = 0; i < thingposes.size(); i++)
	{
		ofDrawCircle(ofPoint(thingposes[i][0]+ofGetWidth()/2, -thingposes[i][1]+ofGetHeight()), thingposes[i][2] );

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