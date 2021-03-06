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
#include "displayApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0, 0, 0);
	ofSetFrameRate(60);

	ofSetWindowTitle("Main App"); // Set the window title to show that it is a Spout Receiver
	bInitialized = false; // Spout receiver initialization

	winWidth = ofGetWidth();
	winHeight = ofGetHeight();
	fbo.allocate(winWidth, winHeight);
	//cout << "winSize w:" << winWidth << ",h:" << winHeight << endl;

	bezManager.setup(10);
	warp = &bezManager.addFbo(&fbo);

	g_Width = winWidth;
	g_Height = winHeight;

	font.loadFont("arial.ttf", fontSize);
	
	setupRestOfWindows();

	//connect osc at last. make sure initialization has been done before receiving commands
	oscReceiver.setup(oscPort);

} // end setup


void ofApp::setupRestOfWindows() {

	auto mainWindow = ofGetMainLoop()->getCurrentWindow();
	windows.push_back(mainWindow);

	//allocate rest of windows dynamically
	for (int i = 1; i < numMonitorsToUse; i++) {
		float resX = monitorResolution[i].x;
		float resY = monitorResolution[i].y;
		int paramVal = correspond[i];
		int monitorIndex = monitorIndices[i];

		auto settings = createWinSetting(resX, resY, monitorIndex, mainWindow);
		auto remainedWindow = ofCreateWindow(*settings);
		windows.push_back(remainedWindow);
		auto remainedApp = make_shared<displayApp>(monitorIndex, paramVal, shareTex, bezManager, usingFormula, showDebugInfo, fontSize);
		ofRunApp(remainedWindow, remainedApp);
	}
	areWindowsSetup = true;

	cout << "succeed allocating all apps" << endl;
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
	else if (addr == "appCtrl") {
		if (message == "exit") {
			std::exit(0);
		}
	}

}

//--------------------------------------------------------------
void ofApp::update() {
	while (oscReceiver.hasWaitingMessages()) {		
		oscReceiver.getNextMessage(&m);
		onOSCMessageReceived(m);
	}
}

enum SpoutShareMode {
	Tex = 0,
	CPU = 1,
	Mem = 2
};

//using GUI app to adjust is better.(SpoutDXmode)
void ofApp::configSpout() {
	spoutreceiver.SetShareMode(Tex);
	spoutreceiver.SetDX9compatible(true);
}

void ofApp::processTex(ofTexture& texture) {
	//check if the texture is allocated correctly, if not, allocate it
	if (!texture.isAllocated() || texture.getWidth() != this->g_Width || texture.getHeight() != this->g_Height) {
		int format = texture.isAllocated() ? texture.getTextureData().glInternalFormat : this->defaultFormat;
		texture.allocate(g_Width, g_Height, format);
		//cout << "allocated, w:" << g_Width << "," << g_Height << endl;
	}
}

void ofApp::drawTex(ofTexture& tex) {
	if (!tex.isAllocated())
		return;

	try {

		fbo.begin();
		
		//draw partial texture to frame buffer and warp it later
		if (usingFormula) {
			unsigned int startX = (winWidth - overlapPixels) * paramVal;
			tex.drawSubsection(0, 0, winWidth, winHeight, startX, 0, winWidth, winHeight);
		}
		else {
			tex.drawSubsection(0, 0, winWidth, winHeight, paramVal, 0, winWidth, winHeight);
		}

		fbo.end();

		warp->draw();
	}
	catch (const char * e) {
		ofLogError("drawTex in ofApp:") << e;
	}
	
}

void ofApp::spoutTryToReceiveTex() {
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

	if (!bInitialized) {
		SenderName[0] = 0; // the name will be filled when the receiver connects to a sender

		// Create the receiver and specify true to attempt to connect to the active sender
		if (spoutreceiver.CreateReceiver(SenderName, width, height, true)) {
			// Is the size of the detected sender different ?
			if (width != g_Width || height != g_Height) {
				// The sender dimensions have changed so update the global width and height
				g_Width = width;
				g_Height = height;
			}
			
			processTex(shareTex);
			bInitialized = true;

			return; // quit for next round

		} // receiver was initialized
		else {
			if (shareTex.isAllocated()) {
				//flush it to black
				shareTex.clear();
			}

			drawFromCenter("No source detected", 0, 0);
		}
	} // end initialization

	  // The receiver has initialized so OK to draw
	if (bInitialized) {

		// Save current global width and height - they will be changed
		// by ReceiveTexture if the sender changes dimensions
		width = g_Width;
		height = g_Height;

		auto associatedTexData = shareTex.getTextureData();

		// Try to receive into the local the texture at the current size
		if (spoutreceiver.ReceiveTexture(SenderName, width, height,
			associatedTexData.textureID, associatedTexData.textureTarget)) {

			//	If the width and height are changed, the local texture has to be resized.
			if (width != g_Width || height != g_Height) {
				// Update the global width and height
				g_Width = width;
				g_Height = height;

				// Update the local texture to receive the new dimensions
				processTex(shareTex);
				return; // do the rest of work in next frame
			}

			drawTex(shareTex);

			// Show what it is receiving
			/*
			if (showDebugInfo) {
				sprintf(str, "From : [%s], TexSize:(%d x %d), winSize:(%d x %d)", SenderName, g_Width, g_Height, winWidth, winHeight);
				ofDrawBitmapString(str, 20, 20);
			}
			*/
		}
		else {
			// A texture read failure might happen if the sender
			// is closed. Release the receiver and start again.
			spoutreceiver.ReleaseReceiver();
			bInitialized = false;
			return;
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
	ofClear(0);
	
	spoutTryToReceiveTex();

	// Show fps
	if (showDebugInfo && showFPS) {
		ofSetColor(0, 255, 0);
		sprintf(str, "FPS: %3.3d", (int)ofGetFrameRate());
		drawFromCenter(str, 0, 200);
	}
	
	if (showDebugInfo && showMonitorIndex) {
		ofSetColor(255, 0, 0);
		sprintf(str, "ParamIndex %d", monitorIndex);
		drawFromCenter(str, 0, 100);
	}

}

//--------------------------------------------------------------
void ofApp::exit() {

    // ====== SPOUT =====
	if(bInitialized) 
		spoutreceiver.ReleaseReceiver(); // Release the receiver

}


void ofApp::setAllWindowsForeground(bool showCursor) {
	if (!areWindowsSetup)
		return;
	for (const auto& win : windows) {
		HWND win32win = win->getWin32Window();
		ShowWindow(win32win, SW_SHOWNOACTIVATE);
	}

	ShowCursor(showCursor);
}

void ofApp::setAllWindowsBackground(bool showCursor) {
	if (!areWindowsSetup)
		return;
	for (const auto& win : windows) {
		HWND win32win = win->getWin32Window();
		ShowWindow(win32win, SW_HIDE);
	}

	ShowCursor(showCursor);
}

void ofApp::drawFromCenter(const char* msg, float xOffset = 0, float yOffset = 0) {

	float msgH = font.stringHeight(msg);
	float msgW = font.stringWidth(msg);

	font.drawString(msg, (winWidth - msgW) / 2 + xOffset, (winHeight - msgH) / 2 + yOffset);

}

void ofApp::loadWarpSetting() {
	bezManager.loadSettings();
}

unique_ptr<ofGLFWWindowSettings> ofApp::createWinSetting(int width, int height, int monitorIndex = 0, shared_ptr<ofAppBaseWindow> sharedWin = nullptr) {
	unique_ptr<ofGLFWWindowSettings> settings(new ofGLFWWindowSettings());
	settings->windowMode = OF_GAME_MODE;
	settings->monitor = monitorIndex;
	settings->width = width;
	settings->height = height;
	settings->setPosition(ofVec2f(0, 0));
	settings->shareContextWith = sharedWin;
	return settings;
}


//Input Event --------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//for displaying app, we dont need adjustment feature
	//bezManager.mousePressed(x, y, button);
}

void ofApp::mouseDragged(int x, int y, int button) {
	
	// send drag event
	//for displaying app, we dont need adjustment feature
	//bezManager.mouseDragged(x, y, button);
}

void ofApp::keyPressed(int key) {
	if (enableKeyCtrl) {
		if (key == OF_KEY_UP)
			setAllWindowsForeground(false);
		else if (key == OF_KEY_DOWN)
			setAllWindowsBackground(true);
	}

	/* for convenience, I would implement this part via OSC
	// send key event
	bezManager.keyPressed(key);

	// show / hide guide
	if (key == OF_KEY_RETURN) {
		bezManager.toggleGuideVisible();
	}
	// save settings
	if (key == 's') {
		bezManager.saveSettings();
	}
	// load settings
	if (key == 'l') {
		bezManager.loadSettings();
	}
	
	*/
}