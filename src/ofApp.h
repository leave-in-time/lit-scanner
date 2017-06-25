#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxNetwork.h"
#ifdef TARGET_OPENGLES
#include "wiringPi.h"
#endif

class ofApp : public ofBaseApp {
public:
	void setup();
#ifdef TARGET_OPENGLES
	void setupGPIOs();
#endif
	void update();
	void draw();
	void drawTracker(ofRectangle rect, string text, ofColor color);

	ofVideoGrabber cam;
	ofxCv::ObjectFinder finder;
	ofPath path;
	string message;
	ofColor color;
	ofxUDPManager client;
	ofTrueTypeFont font;

	string state;
	const static int RELAY_PIN = 2;
};
