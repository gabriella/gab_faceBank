#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker srcTracker;
	ofxFaceTracker camTracker;
   // ofxFacetracker objectMesh;
	
	ofPolyline mouth;
	ofPolyline leftEye;
	
	ofImage srcImg;
	int imageCounter;
	vector<ofVec2f> srcPoints;
	
	int posX, posY;
	
	
	
};
