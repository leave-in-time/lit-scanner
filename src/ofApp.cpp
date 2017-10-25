#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
using namespace std;

static const ofColor red = ofColor::fromHex(0xff0000);
static const ofColor green = ofColor::fromHex(0x00ff00);
static const ofColor white = ofColor::fromHex(0xffffff);

Mat frame;
string videoPath = ofToDataPath("/home/pi/idle.mp4", true);

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofHideCursor();
	// camera and opencv settings
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	#ifdef TARGET_OPENGLES
	cam.setup(800, 600, true);
	#else
	cam.setup(800, 600);
	#endif
	// video settings
	omxPlayer.loadMovie(videoPath);
	omxPlayer.setPaused(true);
	// udp settings
	client.Create();
	client.Bind(8888);
	client.SetNonBlocking(true);
	// misc
	state = "off";
	shouldDisplayMessage = true;
	ofTrueTypeFont::setGlobalDpi(72);
	font.load("VT323-Regular.ttf", 60, true, true);
	font.setLineHeight(68.0);
	font.setLetterSpacing(1.035);
	// rpi stuff
	#ifdef TARGET_OPENGLES
	setupGPIOs();
	#endif
}

//--------------------------------------------------------------
#ifdef TARGET_OPENGLES
void ofApp::setupGPIOs(){
	// setup wiring pi
	if(wiringPiSetup() == -1){
		ofLogNotice(__func__) << "Error on wiringPi setup";
	}
	// relay pin
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, HIGH);
}
#endif

//--------------------------------------------------------------
void ofApp::update() {
	if (state != "idle" && state != "off") {
		#ifdef TARGET_OPENGLES
		frame = cam.grab();
		if (!frame.empty()) finder.update(frame);
		#else
		cam.update();
		if (cam.isFrameNew()) finder.update(cam);
		#endif
	}
	#ifndef TARGET_OPENGLES
	if (state == "idle") omxPlayer.update();
	#endif
	char data[10];
	client.Receive(data, 10);
	string msg = data;
	if (msg == "0") {
		state = "off";
		omxPlayer.setPaused(true);
		#ifdef TARGET_OPENGLES
		digitalWrite(RELAY_PIN, HIGH);
		#endif
		ofLog(OF_LOG_NOTICE, "Relay on");
	}
	else if (msg == "1") {
		state = "idle";
		omxPlayer.loadMovie(videoPath);
		omxPlayer.setPaused(false);
		#ifdef TARGET_OPENGLES
		digitalWrite(RELAY_PIN, HIGH);
		#endif
		ofLog(OF_LOG_NOTICE, "Relay on");
	}
	else if (msg == "2") {
		state = "refused";
		omxPlayer.setPaused(true);
		#ifdef TARGET_OPENGLES
		digitalWrite(RELAY_PIN, HIGH);
		#endif
		ofLog(OF_LOG_NOTICE, "Relay on");
	}
	else if (msg == "3") {
		state = "ok";
		omxPlayer.setPaused(true);
		ofLog(OF_LOG_NOTICE, "Relay off");
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (state == "off") {
		ofClear(0);
	}
	else if (state == "idle") {
		omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	else {
		#ifdef TARGET_OPENGLES
		if (!frame.empty()) drawMat(frame, 0, 0);
		#else
		cam.draw(0, 0);
		#endif
		if (finder.size() > 0) {
			ofRectangle current = finder.getObjectSmoothed(0);
			current.scaleFromCenter(1.3);
			if (state == "refused") drawTracker(current, "IDENTITÉ NON VALIDÉE", red);
			else if (state == "ok") {
				#ifdef TARGET_OPENGLES
				digitalWrite(RELAY_PIN, LOW);
				#endif
				drawTracker(current, "ACCÈS AUTORISÉ", green);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawTracker(ofRectangle rect, string text, ofColor color) {
	float scale = .85 * rect.width / font.stringWidth(text);
	ofSetColor(color);
	ofPushMatrix();
		ofTranslate(rect.x, rect.y -10);
		ofScale(scale, scale);
		if (state == "refused") {
			int currentTime = ofGetElapsedTimeMillis();
			if (currentTime - elapsed > INTERVAL) {
				shouldDisplayMessage = !shouldDisplayMessage;
				elapsed = currentTime;
			}
			if (shouldDisplayMessage) {
				font.drawString(text, 0, 0);
			}
		}
		else {
			font.drawString(text, 0, 0);
		}
	ofPopMatrix();
	ofPushMatrix();
		ofTranslate(rect.x + rect.width + 10, rect.y + rect.height / 2 );
		ofScale(scale / 3, scale / 3);
		string data = to_string(rect.getCenter().x);
		data += ", ";
		data += to_string(rect.getCenter().y);
		data += "\n// ";
		data += to_string(rect.width);
		data += " // ";
		data += to_string(rect.height);
		font.drawString(data, 0, 0);
	ofPopMatrix();
	path.clear();
	path.setStrokeWidth(10);
	path.setFilled(false);
	path.setStrokeColor(color);
	path.rectangle(rect);
	path.draw();
	ofSetColor(white);
}
