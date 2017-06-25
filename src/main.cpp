
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
#ifdef TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
#else
	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
#endif
	settings.width = 800;
	settings.height = 600;
	settings.windowMode = OF_WINDOW;
	ofCreateWindow(settings);
	ofRunApp(new ofApp());

}
