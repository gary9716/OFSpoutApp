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
	ofFbo* fbo;	     
	ofTexture* shareTex; //it would be allocated with huge size and shared among all apps
	
	int g_Width, g_Height;       // Used for checking sender size change
	int paramVal = 0;
	int monitorIndex = -1;
	displayApp(int monitorIndex , int value, ofFbo* sharedFbo, ofTexture* shareTex, bool usingFormula, bool showDebugInfo, int fontSize) {
		this->monitorIndex = monitorIndex;
		this->paramVal = value;
		this->fbo = sharedFbo;
		this->usingFormula = usingFormula;
		this->shareTex = shareTex;
		this->showDebugInfo = showDebugInfo;
		this->fontSize = fontSize;
	}

	void drawTex(ofTexture& tex);
	void drawFromCenter(const char* msg, float xOffset, float yOffset);

	unsigned int overlapPixels = 210;
	bool usingFormula = true;
	unsigned int winWidth = 0;
	unsigned int winHeight = 0;
	char str[256];

	bool showDebugInfo = false;
	bool showMonitorIndex = true;
	ofTrueTypeFont font;
	int fontSize = 32;
};

