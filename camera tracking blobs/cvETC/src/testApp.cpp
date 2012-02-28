#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	spaghMovie.loadMovie("whiteMovie__.mov");
	//spaghMovie.setSpeed(0.5);
	spaghMovie.play();
	#ifdef _USE_LIVE_VIDEO
    vidGrabber.setDeviceID(10);
    vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("green2.mov");
        vidPlayer.play();
	//vidPlayer.loadMovie("spagh.mov");
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
	//finalImg.allocate(320,240);
	
	movieWidth = 384.0;
	movieHeight = 108.0;
	movieX = 0;
	movieY = 0;
	


	bLearnBakground = true;
	threshold = 45;
}

//--------------------------------------------------------------
void testApp::update(){

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;
		//if (bLearnBakground == true){
		//	grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
		//	bLearnBakground = false;
		//}

		// take the abs value of the difference between background and incoming and then threshold:
		//grayDiff.absDiff(grayBg, grayImage);
		grayImage.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
	
		
		contourFinder.findContours(grayImage, 20, (340*240)/3, 10, true);	// find holes
			
	}
	
	

}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableAlphaBlending(); 
	ofBackground(100,100,100,255);


	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	//colorImg.draw(20,20);
	//grayImage.draw(360,20);
	//grayBg.draw(20,280);
	//ofSoundStopAll();
	
	//setVolume(3);
	//grayImage.draw(360,280, 640,480);
	
	
	unsigned char * pixels = grayImage.getPixels();
	
	unsigned char * otherPixels = spaghMovie.getPixels();
	
	unsigned char * finalpixels = new unsigned char[320 * 240 * 4];//finalImg.getPixels();
	
//	unsigned char * vidPixels = vidPlayer.getPixels();
	
	unsigned char * vidPixels = vidGrabber.getPixels();
	
	spaghMovie.idleMovie();
	
	int vidAlphaVal = 128;

	for (int i = 0; i < 320; i++){
		for (int j = 0; j < 240; j++){
			
			int indexRsrc = (j*320 + i)*3;
			int indexGsrc = indexRsrc+1;
			int indexBsrc = indexRsrc+2;
			
			int indexRdst = (j*320 + i)*4;
			int indexGdst = indexRdst+1;
			int indexBdst = indexRdst+2;
			int indexAdst = indexRdst+3;
			
			//pixels[indexG] = 255-pixels[indexG];
			
			if(pixels[indexGsrc/3] < 225) {
			//if(i > 50 && i < 100) {
				finalpixels[indexRdst] = otherPixels[indexRsrc];
				finalpixels[indexGdst] = otherPixels[indexGsrc];
				finalpixels[indexBdst] = otherPixels[indexBsrc];
				finalpixels[indexAdst] = vidAlphaVal;
				//up the blue value if you want by multiplying by like 1.5 or whatever.
				//since its an unsigned char if it goes outside the boundaries that's ok...
				//try it
			}
			else {
				finalpixels[indexRdst] = vidPixels[indexRsrc];
				finalpixels[indexGdst] = vidPixels[indexGsrc];
				finalpixels[indexBdst] = vidPixels[indexBsrc];
				finalpixels[indexAdst] = vidAlphaVal;
			}
			
			//unsigned char r = pixels[(j * 320 + i)*3];
//			float val = 1 - ((float)r / 255.0f);
//			ofSetColor(0, 0, 0);
//			
//			cout << val;
			//if(val>0.5){
				
				//unsigned char * pixelsSpagh = spaghMovie.getPixels();
				
		//spaghMovie.draw(r);
			//}
			//ofCircle(300 + i,20+j,5*val);
			//each pixel is an integer value, 
			//rgand b are unsigned chars , 
		}
		
	}
	
	grayImage.setFromPixels(pixels,320,240);
	grayImage.draw(360,280, 640,480);
	
	finalImg.allocate(320,240, OF_IMAGE_COLOR_ALPHA);  
	finalImg.setFromPixels(finalpixels,320,240,OF_IMAGE_COLOR_ALPHA);
	//finalImg.draw(360,0,320,240);
	finalImg.draw(200,0,movieWidth,movieHeight);
	
	//spaghMovie.draw(640,0);


	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(20,20,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(20,20);
    }

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/a)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case 'a':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
