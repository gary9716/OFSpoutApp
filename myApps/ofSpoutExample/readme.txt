OPENFRAMEWORKS EXAMPLES

The "ofSpoutExample" folder and contents has to be copied to the Openframeworks apps\myApps folder.

The source files of the Spout SDK are required and will be compiled into the application. These are not set up as an addon, so they must be copied to a "SpoutSDK" folder within the Openframeworks folder

C:\OPENFRAMEWORKS
    addons
    examples
    apps
      SpoutSDK <- create this folder (under "apps") and copy the Spout SDK source files into it 
      myApps
        ofSpoutExample <- copy the entire example folder here (under "apps\myApps")
         ofSpoutExample.sln
         ofSpoutExample.vcxproj
         ofSpoutExample.vcxproj.filters
         src <- the required source files for different examples will be in here
         bin <- the required dlls and data are in here

Open the solution file with Visual Studio 2012 or later, set to "Release" and build "ofSpoutExample".

The "src" folder contains files for example applications.
The folders "src\ofSpoutReceiver" and "src\ofSpoutSender" contain the application source files for a receiver and sender examples respectively.
Copy the required ones to the "src" folder. The other source files are common to both.

Openframeworks examples have been tested with Visual Studio 2012 and Openframeworks 0.8.4. 

if someone has some spout linking problems, make sure there is a filter called SpoutSDK under the ofSpoutExampleProject in the Solution Explorer panel.
if not, please follow the steps below:
1. right click the ofSpoutExample project
2. select Add > New Filter
3. name the filter SpoutSDK
4. Drag the SpoutSDK folder in the File Explorer onto this filter
5. do Rebuild Solution 

if someone has some linking problems caused by "av" related keywords, please follow the steps below:
1. select Project > ofSpoutExample Properties...
2. select Configuration Properties > Linker > Input
3. add "$(OF_ROOT)\addons\ofxMovieExporter\libs\libav\lib\vs2010\*.lib" to Additional Dependencies field
4. click confirm and do Rebuild Solution
BTW, one should check there is an addon called ofxMovieExporter in your openframeworks directory.
if not, it can be downloaded from https://github.com/gary9716/ofxMovieExporter-VS2015

if the program is successfully executed, but the screen black out
please go to [Spout2_Root]\SPOUTDXMODE\ and 
use the program SpoutDXMode to config share mode to texture mode.
