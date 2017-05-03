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

		SpoutReceiver spoutreceiver; // A Spout receiver object
		bool bInitialized;		     // Initialization result
		ofFbo* myFbo;	     // Texture used for texture share transfers
		ofTexture* shareTex;

		char SenderName[256];	     // Sender name used by a receiver
		int g_Width, g_Height;       // Used for checking sender size change
		
		int paramVal = 0;
		int monitorIndex = -1;
		vector<shared_ptr<ofAppBaseWindow>>* windows;

		ofApp(int monitorIndex, int value, vector<shared_ptr<ofAppBaseWindow>>& windows, ofFbo* fbo, ofTexture* shareTex, bool usingFormula) {
			this->monitorIndex = monitorIndex;
			this->paramVal = value;
			this->windows = &windows;
			this->myFbo = fbo;
			this->usingFormula = usingFormula;
			this->shareTex = shareTex;
		}

		unsigned int overlapPixels = 210;

		bool firstTime = true;

		//debug info
		bool showFPS = true;
		bool showDebugInfo = false;
		bool showMonitorIndex = true;
		bool enableSenderSelector = false;
		bool enableKeyCtrl = false;
		const int defaultFormat = GL_RGB;
		string defaultChannel = "UnitySender1";
		ofImageType fboImgType = OF_IMAGE_COLOR;
		void onOSCMessageReceived(ofxOscMessage &msg);
		void drawTex(ofTexture& tex);
		void allocateTex(ofTexture& tex);
		bool usingFormula = true;

	private:
		void setAllWindowsForeground(bool showCursor);
		void setAllWindowsBackground(bool showCursor);
		void ClearFBOMemAndAllocate(int width, int height);
		void configSpout();
		bool initReceiver(string channelName);
		bool initReceiver();
		bool receive(ofTexture &); // will automatically allocate the texture
		void oldReceiveTexProcedure();

		string channelName;
		ofxOscReceiver oscReceiver;
		bool recordingSrcSet = false;
		int oscPort = 10000;
		unsigned int winWidth = 0;
		unsigned int winHeight = 0;
		
		bool isRecording = false;
};
