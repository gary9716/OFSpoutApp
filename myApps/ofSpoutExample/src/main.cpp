#include "ofMain.h"
#include "ofApp.h"

#include <iostream>
#include <string>
#include <algorithm>

/*
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

//--------------------------------------------------------------
// to change options for console window (Visual Studio)
//
// Properties > Linker > System > Subsystem
//    for console : Windows (/SUBSYSTEM:CONSOLE)
//
//    for Window : Windows (/SUBSYSTEM:WINDOWS)
//
// Click APPLY and OK. Then make changes to Main as below
//--------------------------------------------------------------

void DumpDevice(const DISPLAY_DEVICE& dd, size_t nSpaceCount)
{
	printf("%*sDevice Name: %s\n", nSpaceCount, "", dd.DeviceName);
	printf("%*sDevice String: %s\n", nSpaceCount, "", dd.DeviceString);
	printf("%*sState Flags: %x\n", nSpaceCount, "", dd.StateFlags);
	printf("%*sDeviceID: %s\n", nSpaceCount, "", dd.DeviceID);
	printf("%*sDeviceKey: ...%s\n\n", nSpaceCount, "", dd.DeviceKey + 42);
}


int outputMonitorInfo() {
	ofAppGLFWWindow ofGLFWWin;
	ofGLFWWin.listMonitors();
	
	int count = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	cout << "num monitors:" << count << endl;
	
	//using native windows api for outputing monitor info
	/*
	DISPLAY_DEVICE dd;

	dd.cb = sizeof(DISPLAY_DEVICE);

	DWORD deviceNum = 0;
	while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) {
		DumpDevice(dd, 0);
		DISPLAY_DEVICE newdd = { 0 };
		newdd.cb = sizeof(DISPLAY_DEVICE);
		DWORD monitorNum = 0;
		while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
		{
			DumpDevice(newdd, 4);
			monitorNum++;
		}
		puts("");
		deviceNum++;
	}
	*/
	
	return count;
}


string outputWorkingDir() {

	ofFilePath filePath;
	string workingDirPath = filePath.getCurrentWorkingDirectory();
	cout << "current working dir:" << workingDirPath << endl;
	return workingDirPath;
}

bool usingParams = false;
void trim(string &str) {
	std::remove(str.begin(), str.end(), ' ');
	std::remove(str.begin(), str.end(), '\n');
	std::remove(str.begin(), str.end(), '	');
	
	return;
}

void PauseAndThenLeave() {
	system("pause");
	exit(255);
}

ofGLFWWindowSettings* createWinSetting(int width, int height, int monitorIndex = 0, shared_ptr<ofAppBaseWindow> sharedWin = nullptr) {
	ofGLFWWindowSettings* settings = new ofGLFWWindowSettings();
	settings->windowMode = OF_GAME_MODE;
	settings->monitor = monitorIndex;
	settings->width = width;
	settings->height = height;
	settings->setPosition(ofVec2f(0, 0));
	settings->shareContextWith = sharedWin;
	return settings;
}

int debugFlagIndex = 2;
bool isDebugging = true;
int debugMsgSize = 40;

// for displaying console ( debugging use )
//========================================================================
int main(int argc,      // Number of strings in array argv  
	char *argv[],   // Array of command-line argument strings  
	char *envp[])  // Array of environment variable strings   
{ // Properties > Linker > System > Subsystem, set the field to "Windows (/SUBSYSTEM:CONSOLE)"

// for hiding console
//========================================================================
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) { // Properties > Linker > System > Subsystem, set the field to "Windows (/SUBSYSTEM:WINDOWS)"
	
	int count;
	// Display each command-line argument.  
	cout << "\nCommand-line arguments:\n";
	for (count = 0; count < argc; count++)
		cout << "  argv[" << count << "]   "
		<< argv[count] << "\n";

	if (argc > 1)
		isDebugging = stoi(argv[debugFlagIndex]) == 1;
	else
		isDebugging = false;

	string workDirPath = outputWorkingDir();
	string paramFileName = "params.txt";
	string fullFilePath = workDirPath + "\\..\\" + paramFileName;
	//cout << "full:" << fullFilePath << endl;
	ifstream myfile(fullFilePath);
	
	//init and declare some local vars
	string line;
	vector<int> monitorIndices;
	vector<int> correspond;
	bool usingFormula = true;
	vector<ofVec2f> monitorResolution;
	int numParams = 0;
	bool pauseAndLeave = false;
	
	
	if (myfile.is_open())
	{
		getline(myfile, line);
		auto params = ofSplitString(line, ",", true, true);
		for (string param : params) {
			monitorIndices.push_back(stoi(param));
		}
		cout << "number elements:" << monitorIndices.size() << ",monitorIndices:" << endl;
		for (int parsedIndex : monitorIndices) {
			cout << parsedIndex << " ";
		}
		cout << endl;
		


		getline(myfile, line);
		params = ofSplitString(line, ",", true, true);
		for (string param : params) {
			correspond.push_back(stoi(param));
		}
		cout << "number elements:" << correspond.size() << ",corresponding parts:" << endl;
		for (int parsedIndex : correspond) {
			cout << parsedIndex << " ";
		}
		cout << endl;
		


		getline(myfile, line);
		trim(line);
		usingFormula = stoi(line) == 1;
		cout << "usingFormula:" << usingFormula << endl;



		getline(myfile, line);
		trim(line);
		pauseAndLeave = stoi(line) == 1;



		numParams = monitorIndices.size();
		if (correspond.size() == monitorIndices.size() && numParams > 0)
			usingParams = true;

		if (usingParams) {
			for (int i = 0; i < numParams; i++) {
				getline(myfile, line);
				auto params = ofSplitString(line, ",", true, true);
				ofVec2f mSize(stoi(params[0]), stoi(params[1]));
				cout << "m_i:" << i << ",w:" << mSize.x << ",h:" << mSize.y << endl;
				monitorResolution.push_back(mSize);
			}
		}
		
		myfile.close();
	}
	else {
		cout << "failed to open file" << endl;
	}

	int numMonitors = outputMonitorInfo();
	if (numMonitors == 0)
		return -1;
	
	if (usingParams) {
		cout << "using params" << endl;
	}
	else {
		cout << "not using params" << endl;
	}

	if (pauseAndLeave)
		PauseAndThenLeave();


	//if not using parameters, setup with all available(available from API) monitors
	if (!usingParams) {
		monitorIndices.clear();
		correspond.clear();
		monitorResolution.clear();

		for (int i = 0; i < numMonitors; i++) {
			monitorIndices.push_back(i);
			correspond.push_back(i);
			monitorResolution.push_back(ofPoint(1920, 1200));
		}
	}

	auto settings = createWinSetting(monitorResolution[0].x, monitorResolution[0].y, monitorIndices[0]);
	int numMonitorsToUse = usingParams ? numParams : numMonitors;
	
	//create first window and rest of windows would share the same context with the first one
	auto mainWindow = ofCreateWindow(*settings);
	auto mainApp = make_shared<ofApp>(settings->monitor, correspond[0], usingFormula, isDebugging, debugMsgSize, numMonitorsToUse, monitorResolution, correspond, monitorIndices);
	ofRunApp(mainWindow, mainApp);
	
	/*
	int winIndex = 0;
	MONITORINFO monInfo;
	for (const auto& win : windows) {
		HWND win32win = win->getWin32Window();
		HMONITOR hMon = MonitorFromWindow(win32win, MONITOR_DEFAULTTONEAREST);
		GetMonitorInfo(hMon, &monInfo);
		winIndex++;
	}
	*/
	
	ofRunMainLoop();

}
