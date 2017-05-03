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
	ofTexture* shareTex;
	
	int g_Width, g_Height;       // Used for checking sender size change
	int paramVal = 0;
	int monitorIndex = -1;
	displayApp(int monitorIndex , int value, ofFbo* sharedFbo, ofTexture* shareTex, bool usingFormula) {
		this->monitorIndex = monitorIndex;
		this->paramVal = value;
		this->myFbo = sharedFbo;
		this->usingFormula = usingFormula;
		this->shareTex = shareTex;
	}

	void drawTex(ofTexture& tex);
	void oldProcedure();

	unsigned int overlapPixels = 210;
	bool showMonitorIndex = true;
	bool usingFormula = true;
	unsigned int winWidth = 0;
	unsigned int winHeight = 0;
};

