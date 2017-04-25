#include "ofMain.h"
#include "ofApp.h"
#include "displayApp.h"


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
const int texWidth = 15390;
const int texHeight = 1200;

// for displaying console ( debugging use )
//========================================================================
int main() { // Properties > Linker > System > Subsystem, set the field to "Windows (/SUBSYSTEM:CONSOLE)"

// for hiding console
//========================================================================
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) { // Properties > Linker > System > Subsystem, set the field to "Windows (/SUBSYSTEM:WINDOWS)"
	
	string workDirPath = outputWorkingDir();
	string paramFileName = "params.txt";
	string fullFilePath = workDirPath + "\\..\\" + paramFileName;
	//cout << "full:" << fullFilePath << endl;
	ifstream myfile(fullFilePath);
	string line;
	vector<int> monitorIndices;
	vector<int> correspond;
	if (myfile.is_open())
	{
		getline(myfile, line);
		auto params = ofSplitString(line, ",");
		for (string param : params) {
			monitorIndices.push_back(stoi(param));
		}

		getline(myfile, line);
		params = ofSplitString(line, ",");
		for (string param : params) {
			correspond.push_back(stoi(param));
		}
		myfile.close();
	}
	else {
		cout << "failed to open file" << endl;
	}

	int numMonitors = outputMonitorInfo();
	
	if (numMonitors == 0)
		return -1;
	

	int numParams = monitorIndices.capacity();
	if (correspond.capacity() == monitorIndices.capacity() && numParams > 0)
		usingParams = true;

	ofGLFWWindowSettings settings;
	settings.windowMode = OF_GAME_MODE;
	settings.width = 1920;
	settings.height = 1200;
	settings.setPosition(ofVec2f(0, 0));
	
	vector<shared_ptr<ofAppBaseWindow>> windows;
	int partIndex = 0;

	if (usingParams) {
		settings.monitor = monitorIndices[0]; //the index in monitors
		partIndex = correspond[0];
	}
	else {
		settings.monitor = 0; //the index in monitors
		partIndex = settings.monitor;
	}

	//create first window and rest of windows would share the same context with first one
	auto mainWindow = ofCreateWindow(settings);
	
	ofFbo* fbo = new ofFbo();
	auto mainApp = make_shared<ofApp>(settings.monitor, partIndex, windows, fbo);
	windows.push_back(mainWindow);
	settings.shareContextWith = mainWindow;
	ofRunApp(mainWindow, mainApp);
	
	if (usingParams) {
		cout << "using params" << endl;
		for (int i = 1; i < numParams; i++) {
			settings.monitor = monitorIndices[i]; //the index in monitors
			partIndex = correspond[i];

			auto remainedWindow = ofCreateWindow(settings);
			auto remainedApp = make_shared<displayApp>(settings.monitor, partIndex, fbo);
			windows.push_back(remainedWindow);
			ofRunApp(remainedWindow, remainedApp);
		}
	}
	else {
		cout << "not using params" << endl;
		for (int i = 1; i < numMonitors; i++) {
			settings.monitor = i; //the index in monitors
			partIndex = settings.monitor;

			auto remainedWindow = ofCreateWindow(settings);
			auto remainedApp = make_shared<displayApp>(settings.monitor, partIndex, fbo);
			windows.push_back(remainedWindow);
			ofRunApp(remainedWindow, remainedApp);
		}
	}
	
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
