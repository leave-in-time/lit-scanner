#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxNetwork.h"
#ifdef TARGET_OPENGLES
#include "wiringPi.h"
#include "ofxCvPiCam.h"
#include "ofxOMXPlayer.h"
#endif

#define INTERVAL 300

class ofApp : public ofBaseApp {
public:
	void setup();
#ifdef TARGET_OPENGLES
	void setupGPIOs();
#endif
	void update();
	void draw();
	void drawTracker(ofRectangle rect, string text, ofColor color);

	#ifdef TARGET_OPENGLES
	ofxCvPiCam cam;
	ofxOMXPlayer omxPlayer;
	#else
	ofVideoGrabber cam;
	ofVideoPlayer omxPlayer;
	#endif
	ofxCv::ObjectFinder finder;
	ofPath path;
	string message;
	ofColor color;
	ofxUDPManager client;
	ofTrueTypeFont font;

	string state;
	uint64_t elapsed;
	bool shouldDisplayMessage;
	const static int RELAY_PIN = 7;
};
