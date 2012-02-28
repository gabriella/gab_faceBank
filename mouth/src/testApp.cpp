#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    cam.setDeviceID(10);
	cam.initGrabber(640, 480);
	ofEnableAlphaBlending();
	
	camTracker.setup();
	
	imageCounter =0;
	posX=ofGetWindowWidth()/2;
	posY=ofGetWindowHeight()/2;;
	
	//face tracker on source
	srcTracker.setup();
	srcTracker.setRescale(.5);
	
	srcTracker.setup();
    srcTracker.setIterations(25);
    srcTracker.setAttempts(2);
    srcTracker.setClamp(4);
    srcTracker.setTolerance(.01);
    
	srcImg.loadImage("/Users/administrator/Documents/of_preRelease_v007_osx/SpatialMedia/week2/mouth/bin/data/face.jpeg");
    srcTracker.update(toCv(srcImg));
    srcPoints = srcTracker.getImagePoints();

}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		camTracker.update(toCv(cam));
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	srcImg.draw(0,0, 100,100); //draw srcimg thumbnail
	
	if(camTracker.getFound()) {
        ofMesh objectMesh = camTracker.getObjectMesh();

		ofMesh camMesh = camTracker.getImageMesh();
				
		ofMesh imageMesh = camTracker.getImageMesh();
        
        
		//move the mesh away
		cam.getTextureReference().bind();
		ofPushMatrix();
        
		ofTranslate(100,100);
		//imageMesh.draw();
		ofPopMatrix();
		cam.getTextureReference().unbind();
		
		//cam.getTextureReference().unbind();
        
		/*
		ofMesh imageMesh = tracker.getImageMesh();
		leftEye = tracker.getImageFeature(ofxFaceTracker::EYE_LEFT);
		mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
		 */
		
		cam.getTextureReference().bind();
        ofPushMatrix();
        ofSetupScreenOrtho(640,480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
        ofTranslate(posX, posY);
        ofScale(3,3);
        
        
        
        objectMesh.draw();
        ofPopMatrix();
        
        //camMesh.draw();

		cam.getTextureReference().unbind();

        camMesh.clearTexCoords();
		camMesh.addTexCoords(srcPoints);
        
        srcImg.getTextureReference().bind();
        ofSetupScreenOrtho(640,480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);

        camMesh.draw();
        srcImg.getTextureReference().unbind();
        
        
	    mouth.setClosed(true);
        ofPushStyle();
        ofFill();
        ofSetColor(60,0,0);
        ofBeginShape();
        for(int i =0; i<mouth.size(); i++){
            ofVertex(mouth[i]);
        }
        ofEndShape(true);
        ofPopStyle();  
		mouth.draw();
	
	
	if (posY < 400 ) {
		posY ++;}
	else {
		posY = 400; 
	}
        if(posX>50){
            posX--;
        }
        else{posX = 50;}
	}
	
	/* distort the mouth 
	ofSetLineWidth(1);
	tracker.draw();
	
	ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
	ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
	ofPolyline faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
	
	ofSetLineWidth(2);
	ofSetColor(ofColor::red);
	leftEye.draw();
	ofSetColor(ofColor::green);
	rightEye.draw();
	ofSetColor(ofColor::blue);
	faceOutline.draw();
	
	 if(tracker.getFound()) {
	 ofSetColor(255);
	 bool inside = faceOutline.inside(mouseX, mouseY);
	 ofDrawBitmapString(inside ? "inside" : "outside", 10, 40);
	 }

	*/

}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		camTracker.reset();
		posX =0;
		posY =0;
	}
	
	if (key== ' ') {
		srcImg.loadImage(ofToString(imageCounter-1) + ".png");
		cout << imageCounter;
		srcTracker.update(toCv(srcImg));
	    srcPoints = srcTracker.getImagePoints();
		
	}
	
	if (key == 's') {
		srcImg.grabScreen(200,0,(ofGetWindowWidth()-200),ofGetWindowHeight());
		srcImg.saveImage(ofToString(imageCounter) + ".png");
		imageCounter ++;
	}
	
	
	
}


