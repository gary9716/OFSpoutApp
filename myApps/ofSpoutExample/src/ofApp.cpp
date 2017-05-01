/*

	Spout OpenFrameworks Receiver example

    Visual Studio using the Spout SDK

    Search for SPOUT for additions to a typical Openframeworks application

	Copyright (C) 2017 Lynn Jarvis.

	03.08.15 - Created project
	10.01.17 - Update for Spout 2.006

	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0, 0, 0);
	ofSetFrameRate(60);

	oscReceiver.setup(oscPort);
	
	ofSetWindowTitle("OF Spout Receiver and OSC Receiver"); // Set the window title to show that it is a Spout Receiver
	bInitialized  = false; // Spout receiver initialization
	SenderName[0] = 0; // the name will be filled when the receiver connects to a sender
	
	// Allocate a texture for shared texture transfers
	// An openFrameWorks texture is used so that it can be drawn.
	g_Width = ofGetWidth();
	g_Height = ofGetHeight();

	if (textureFormat == GL_RGB) {
		fboImgType = OF_IMAGE_COLOR;
	}
	else if (textureFormat == GL_RGBA) {
		fboImgType = OF_IMAGE_COLOR_ALPHA;
	}

	ClearFBOMemAndAllocate(g_Width, g_Height);
	
} // end setup


void ofApp::ClearFBOMemAndAllocate(int width, int height) {
	myFbo->clear();
	myFbo->allocate(width, height, textureFormat);
}

void ofApp::onOSCMessageReceived(ofxOscMessage &msg) {
	string addr = msg.getAddress();
	string message = msg.getArgAsString(0);
	//cout << "addr:" << addr << ",msg:" << message << endl;
	if (addr == "winCtrl") {
		if (message == "show") {
			cout << "make all windows foreground" << endl;
			setAllWindowsForeground(false);
		}
		else if (message == "hide") {
			cout << "hide all windows" << endl;
			setAllWindowsBackground(true);
		}
	}

}

ofxOscMessage m;

//--------------------------------------------------------------
void ofApp::update() {
	while (oscReceiver.hasWaitingMessages()) {		
		oscReceiver.getNextMessage(&m);
		onOSCMessageReceived(m);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	ofSetColor(255);
	unsigned int width, height;

	// ====== SPOUT =====
	//
	// INITIALIZE A RECEIVER
	//
	// The receiver will attempt to connect to the name it is sent.
	// Alternatively set the optional bUseActive flag to attempt to connect to the active sender.
	// If the sender name is not initialized it will attempt to find the active sender
	// If the receiver does not find any senders the initialization will fail
	// and "CreateReceiver" can be called repeatedly until a sender is found.
	// "CreateReceiver" will update the passed name, and dimensions.
	if(!bInitialized) {
		// Create the receiver and specify true to attempt to connect to the active sender
		if(spoutreceiver.CreateReceiver(SenderName, width, height, true)) {
			// Is the size of the detected sender different ?
			if(width != g_Width || height != g_Height ) {
				// The sender dimensions have changed so update the global width and height
				g_Width  = width;
				g_Height = height;

				// Update the local texture to receive the new dimensions
				ClearFBOMemAndAllocate(g_Width, g_Height);
				//cout << "tex width:" << g_Width << ",height:" << g_Height << endl;
			}

			bInitialized = true;
			return; // quit for next round
		
		} // receiver was initialized
		else {
			sprintf(str, "No sender detected");
			ofDrawBitmapString(str, 20, 20);
		}
	} // end initialization

	// The receiver has initialized so OK to draw
	if(bInitialized) {

		// Save current global width and height - they will be changed
		// by ReceiveTexture if the sender changes dimensions
		width  = g_Width;
		height = g_Height;

		/*
		myFbo->begin();
		ofClear(255, 255, 255, 0);
		myFbo->end();
		*/
		
		auto associatedTex = myFbo->getTextureReference();
		auto associatedTexData = associatedTex.getTextureData();

		// Try to receive into the local the texture at the current size
		if(spoutreceiver.ReceiveTexture(SenderName, width, height, 
			associatedTexData.textureID, associatedTexData.textureTarget)) {

			//	If the width and height are changed, the local texture has to be resized.
			if(width != g_Width || height != g_Height ) {
				// Update the global width and height
				g_Width  = width;
				g_Height = height;
				
				// Update the local texture to receive the new dimensions
				ClearFBOMemAndAllocate(g_Width, g_Height);
				return; // quit for next round
			}

			//draw partial
			unsigned int winWidth = ofGetWidth();
			unsigned int winHeight = ofGetHeight();
			if (usingFormula) {
				unsigned int startX = (winWidth - overlapPixels) * paramVal;
				associatedTex.drawSubsection(0, 0, winWidth, winHeight, startX, 0, winWidth, winHeight);
			}
			else {
				associatedTex.drawSubsection(0, 0, winWidth, winHeight, paramVal, 0, winWidth, winHeight);
			}

			if (firstTime) {
				firstTime = false;
				setAllWindowsForeground(true);
			}

			// Show what it is receiving
			if(showDebugInfo) {
				sprintf(str, "From : [%s], TexSize:(%d x %d), winSize:(%d x %d)", SenderName, g_Width, g_Height, winWidth, winHeight);
				ofDrawBitmapString(str, 20, 20);
			}

			if (enableSenderSelector) {
				sprintf(str, "RH click select sender");
				ofDrawBitmapString(str, 15, ofGetHeight() - 20);
			}

		}
		else {
			// A texture read failure might happen if the sender
			// is closed. Release the receiver and start again.
			spoutreceiver.ReleaseReceiver();
			bInitialized = false;
			return;
		}
	}

	// Show fps
	if (showFPS) {
		ofSetColor(0, 255, 0);
		sprintf(str, "fps: %3.3d", (int)ofGetFrameRate());
		ofDrawBitmapString(str, 20, 60);
	}
	
	if (showMonitorIndex) {
		ofSetColor(255, 0, 0);
		sprintf(str, "Monitor: %d", monitorIndex);
		ofDrawBitmapString(str, 20, 40);
	}

}

//--------------------------------------------------------------
void ofApp::exit() {

    // ====== SPOUT =====
	if(bInitialized) 
		spoutreceiver.ReleaseReceiver(); // Release the receiver

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	 
	// ====== SPOUT =====
	if(enableSenderSelector && button == 2) { // rh button
		// Open the sender selection panel
		// Spout must have been installed
		spoutreceiver.SelectSenderPanel();
	}
}

void ofApp::keyPressed(int key) {
	if (enableKeyCtrl) {
		if (key == OF_KEY_UP)
			setAllWindowsForeground(false);
		else if (key == OF_KEY_DOWN)
			setAllWindowsBackground(true);
	}
}

void ofApp::setAllWindowsForeground(bool showCursor) {
	for (const auto& win : *windows) {
		HWND win32win = win->getWin32Window();
		ShowWindow(win32win, SW_SHOWNOACTIVATE);
	}

	ShowCursor(showCursor);
}

void ofApp::setAllWindowsBackground(bool showCursor) {
	for (const auto& win : *windows) {
		HWND win32win = win->getWin32Window();
		ShowWindow(win32win, SW_HIDE);
	}

	ShowCursor(showCursor);
}


