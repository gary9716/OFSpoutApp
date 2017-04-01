#pragma once

#include "ofMain.h"
#include "..\..\..\SpoutSDK\Spout.h" // Spout SDK

class displayApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	bool bInitialized;		     // Initialization result
	ofFbo* myFbo;	     // Texture used for texture share transfers
	int g_Width, g_Height;       // Used for checking sender size change
	int partIndex = 0;
	int monitorIndex = -1;
	displayApp(int monitorIndex , int partIndex, ofFbo* sharedFbo) {
		this->monitorIndex = monitorIndex;
		this->partIndex = partIndex;
		this->myFbo = sharedFbo;
	}

	unsigned int overlapPixels = 210;
	bool showMonitorIndex = true;

};

