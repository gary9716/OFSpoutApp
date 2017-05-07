/*

	Spout OpenFrameworks Receiver example

	Copyright (C) 2015-2017 Lynn Jarvis.

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

*/
#pragma once

#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include "ofMain.h"
#include "..\..\..\SpoutSDK\Spout.h" // Spout SDK
#include "ofxOsc.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		void mousePressed(int x, int y, int button);
		void keyPressed(int key);
		bool bInitialized;		     // Initialization result
		ofFbo* fbo;	     
		ofTexture* shareTex; //it would be allocated with huge size and shared among all apps

		char SenderName[256];	     // Sender name used by a receiver
		int g_Width, g_Height;       // Used for checking sender size change
		
		int paramVal = 0;
		int monitorIndex = -1;
		
		ofApp(int monitorIndex, int value, vector<shared_ptr<ofAppBaseWindow>>& windows, ofFbo* fbo, ofTexture* shareTex, bool usingFormula, bool showDebugInfo, int fontSize) {
			this->monitorIndex = monitorIndex;
			this->paramVal = value;
			this->windows = &windows;
			this->fbo = fbo;
			this->usingFormula = usingFormula;
			this->shareTex = shareTex;
			this->showDebugInfo = showDebugInfo;
			this->fontSize = fontSize;
		}

		bool enableSenderSelector = false;
		bool enableKeyCtrl = false;
		bool usingFormula = true;
		const int defaultFormat = GL_RGB;
		string defaultChannel = "UnitySender1";
		unsigned int overlapPixels = 210;

	private:
		void setAllWindowsForeground(bool showCursor);
		void setAllWindowsBackground(bool showCursor);
		void configSpout();
		void spoutTryToReceiveTex();
		void onOSCMessageReceived(ofxOscMessage &msg);
		void drawTex(ofTexture& tex);
		void processTex(ofTexture& tex);
		void drawFromCenter(const char* msg, float xOffset, float yOffset);

		SpoutReceiver spoutreceiver; // A Spout receiver object
		ofxOscReceiver oscReceiver;
		vector<shared_ptr<ofAppBaseWindow>>* windows;

		int oscPort = 10000;
		unsigned int winWidth = 0;
		unsigned int winHeight = 0;
		
		//debug info
		bool showFPS = true;
		bool showDebugInfo = false;
		bool showMonitorIndex = true;
		ofTrueTypeFont font;
		int fontSize = 32;
		char str[256];
		
};
