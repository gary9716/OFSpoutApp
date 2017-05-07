#include "displayApp.h"

//--------------------------------------------------------------
void displayApp::setup() {

	ofBackground(0, 0, 0);
	ofSetWindowTitle("Display only");
	winWidth = ofGetWidth();
	winHeight = ofGetHeight();

	font.loadFont("arial.ttf", fontSize);

} // end setup

//--------------------------------------------------------------
void displayApp::update() {

}

//--------------------------------------------------------------
void displayApp::draw() {
	ofSetColor(255);
	drawTex(*shareTex);

	if (showDebugInfo && showMonitorIndex) {
		ofSetColor(255, 0, 0);
		sprintf(str, "ParamIndex %d", monitorIndex);
		drawFromCenter(str, 0, 100);
	}

}

void displayApp::drawTex(ofTexture& tex) {
	if (!tex.isAllocated())
		return;

	try {
		//draw partial
		if (usingFormula) {
			unsigned int startX = (winWidth - overlapPixels) * paramVal;
			tex.drawSubsection(0, 0, winWidth, winHeight, startX, 0, winWidth, winHeight);
		}
		else {
			tex.drawSubsection(0, 0, winWidth, winHeight, paramVal, 0, winWidth, winHeight);
		}
	}
	catch (const char * e) {
		ofLogError("drawTex in displayApp:") << e;
	}
	
}

void displayApp::drawFromCenter(const char* msg, float xOffset = 0, float yOffset = 0) {

	float msgH = font.stringHeight(msg);
	float msgW = font.stringWidth(msg);

	font.drawString(msg, (winWidth - msgW) / 2 - +xOffset, (winHeight - msgH) / 2 - +yOffset);

}

//--------------------------------------------------------------
void displayApp::exit() {

}
