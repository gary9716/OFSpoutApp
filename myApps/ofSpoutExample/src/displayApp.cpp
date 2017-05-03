#include "displayApp.h"

//--------------------------------------------------------------
void displayApp::setup() {

	ofBackground(0, 0, 0);
	ofSetWindowTitle("Display only"); // Set the window title to show that it is a Spout Receiver
	winWidth = ofGetWidth();
	winHeight = ofGetHeight();

} // end setup


  //--------------------------------------------------------------
void displayApp::update() {

}

void displayApp::oldProcedure() {
	char str[256];

	if (myFbo == NULL) {
		cout << "fbo is null!?" << endl;
		return;
	}

	if (!myFbo->isAllocated() || !myFbo->isUsingTexture()) {
		sprintf(str, "fbo not allocated yet");
		ofDrawBitmapString(str, 20, 20);
		return;
	}

	drawTex(myFbo->getTextureReference());

	if (showMonitorIndex) {
		ofSetColor(255, 0, 0);
		sprintf(str, "Monitor: %d", monitorIndex);
		ofDrawBitmapString(str, 20, 40);
	}
}

//--------------------------------------------------------------
void displayApp::draw() {
	ofSetColor(255);
	if (shareTex->isAllocated()) {
		drawTex(*shareTex);
	}

}

void displayApp::drawTex(ofTexture& tex) {
	//draw partial
	if (usingFormula) {
		unsigned int startX = (winWidth - overlapPixels) * paramVal;
		tex.drawSubsection(0, 0, winWidth, winHeight, startX, 0, winWidth, winHeight);
	}
	else {
		tex.drawSubsection(0, 0, winWidth, winHeight, paramVal, 0, winWidth, winHeight);
	}
	
	/*
	try {
		
	}
	catch (const char * e) {
		ofLogError("drawTex:") << e;
	}
	*/
	
}

//--------------------------------------------------------------
void displayApp::exit() {

}
