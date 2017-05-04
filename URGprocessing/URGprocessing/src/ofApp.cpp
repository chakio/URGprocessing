#include "ofApp.h"


//using namespace qrk;
//using namespace std;


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
	urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(+90), 0);//360digree/1024ŒÂ 180“x•ª‚Å513ŒÂ
#endif
	
	urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
	
}
void ofApp::setup(){
	ofSetFrameRate(15);
	font.loadFont("Meiryo.ttf", 20);
	
}

//--------------------------------------------------------------
void ofApp::update() {
	long time_stamp = 0;
	if (!urg.get_distance(data, &time_stamp)) {
		cout << "Urg_driver::get_distance(): " << urg.what() << endl;
	}
	data=urg_processing.limitprocessing(data, 200000, 20);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetColor(0, 0, 255);
	double Avedata = 0;
	ofPoint center(ofGetWidth() / 2, ofGetHeight());
	urg_processing.drawdata(data);


 	ofSetColor(255, 0, 0);
	Ddata = data;
	data1 = data;

	for (int i = 1; i < data.size(); i++)
	{
		data1[i] = data1[i - 1] + (data[i] - data[i - 1])/2;
		Avedata += data1[i];
	}
	Avedata /= data.size();
	double pos = 0,posnum=0;
	for (int i = 0; i < data1.size(); i++)
	{
		if (data1[i] < Avedata)
		{
			pos += i;
			posnum++;
		}
	}
	pos /= posnum;
	for (int i = 1; i < data.size(); i++)
	{
		Ddata[i-1] = abs(data1[i - 1] - data1[i]);
	}
	
	ofSetColor(0, 0, 255);
	for (int i = 0; i < data.size(); i++)
	{

		ofDrawLine(center, ofPoint(ofGetWidth() / 2 + Ddata[i] * 2 * cos((data.size()-i)*0.35 / 180 * M_PI + M_PI), Ddata[i] * 2 * sin((data.size() - i)*0.35 / 180 * M_PI + M_PI) + ofGetHeight()));
	}
	if ((int)pos < 0)
	{
		pos=0;
	}
	if ((int)pos > data.size()-1)
	{
		pos = data.size()-1;
	}
	ofSetColor(255, 0, 0);
	ofDrawCircle(ofPoint(ofGetWidth() / 2 +data[(int)pos] * 2 * cos((data.size()-pos)*0.35 / 180 * M_PI+M_PI), data[(int)pos] * 2 * sin((data.size()-pos)*0.35 / 180 * M_PI + M_PI) + ofGetHeight()),30);


	drawinformations();
	
	stringstream pp;
	pp << "position " << ofToString(pos*0.35);
	ofDrawBitmapString(pp.str(), 10, 35);
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
	ofDrawCircle(ofPoint(ofGetWidth() / 2, ofGetHeight(), 0), 150);
	ss.str("");
	ss << "300mm";
	font.drawString(ss.str(), ofGetWidth() / 2 - 100, ofGetHeight() - 40);
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