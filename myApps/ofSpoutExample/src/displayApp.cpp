#include "displayApp.h"

//--------------------------------------------------------------
void displayApp::setup() {

	ofBackground(0, 0, 0);

	ofSetWindowTitle("Display only"); // Set the window title to show that it is a Spout Receiver
	
} // end setup


  //--------------------------------------------------------------
void displayApp::update() {

}

//--------------------------------------------------------------
void displayApp::draw() {
	char str[256];
	ofSetColor(255);
	
	if (myFbo == NULL) {
		cout << "fbo is null!?" << endl;
		return;
	}

	
	if (!myFbo->isAllocated() || !myFbo->isUsingTexture()) {
	//	sprintf(str, "fbo not allocated yet");
	//	ofDrawBitmapString(str, 20, 20);
		return;
	}

	unsigned int winWidth = ofGetWidth();
	unsigned int winHeight = ofGetHeight();
	
	if (usingFormula) {
		unsigned int startX = (winWidth - overlapPixels) * paramVal;
		myFbo->getTextureReference().drawSubsection(0, 0, winWidth, winHeight, startX, 0, winWidth, winHeight);
	}
	else {
		myFbo->getTextureReference().drawSubsection(0, 0, winWidth, winHeight, paramVal, 0, winWidth, winHeight);
	}


	if (showMonitorIndex) {
		ofSetColor(255, 0, 0);
		sprintf(str, "Monitor: %d", monitorIndex);
		ofDrawBitmapString(str, 20, 40);
	}

}

//--------------------------------------------------------------
void displayApp::exit() {

}
