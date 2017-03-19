#include "ofMain.h"
#include "ofApp.h"

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

int outputMonitorInfo() {
	ofAppGLFWWindow ofGLFWWin;
	ofGLFWWin.listMonitors();
	
	int count = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	cout << "num monitors:" << count << endl;

	return count;
}


string outputWorkingDir() {

	ofFilePath filePath;
	string workingDirPath = filePath.getCurrentWorkingDirectory();
	cout << "current working dir:" << workingDirPath << endl;
	return workingDirPath;
}

bool usingParams = false;

// for default console
//========================================================================
int main() {

// for window without console
//========================================================================
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	

	string workDirPath = outputWorkingDir();
	ifstream myfile("params.txt");
	string line;
	vector<int> correspond;
	if (myfile.is_open())
	{
		getline(myfile, line);
		auto params = ofSplitString(line, ",");
		for (string param : params) {
			correspond.push_back(stoi(param));
		}

		myfile.close();
	}


	int numMonitors = outputMonitorInfo();
	if (correspond.capacity() == numMonitors)
		usingParams = true;

	ofGLFWWindowSettings settings;
	settings.windowMode = OF_GAME_MODE;
	settings.width = 1920;
	settings.height = 1200;
	settings.setPosition(ofVec2f(0, 0));
	
	vector<shared_ptr<ofAppBaseWindow>> windows;

	for (int i = 0; i < numMonitors; i++) {
		
		settings.monitor = i; //the index in monitors
		
		auto mainWindow = ofCreateWindow(settings);
		
		int partIndex = i;
		if (usingParams)
			partIndex = correspond[i];

		auto mainApp = make_shared<ofApp>(partIndex, windows);

		windows.push_back(mainWindow);
	
		ofRunApp(mainWindow, mainApp);
		
	}
	
	for (const auto& win : windows) {
		HWND win32win = win->getWin32Window();
		win->hideCursor();
		ShowWindow(win32win, SW_SHOWNOACTIVATE);
	}

	ofRunMainLoop();

}
