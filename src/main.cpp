/***********************************************************************
Main.cpp
Copyright (c) 2016-2017 Thomas Wolf and Rasmus R. Paulsen (people.compute.dtu.dk/rapa)

This file is part of the Magic Sand.

The Magic Sand is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Magic Sand is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Augmented Reality Sandbox; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***********************************************************************/


#include "ofMain.h"
#include "ofApp.h"
#include "GLFW/glfw3.h"

const std::string MagicSandVersion = "2.0.0.0";

bool setWindowDimensions(ofGLFWWindowSettings& settings, int windowsNum) {
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	cout << "Number of screens found: " << count << endl;
	if (count > windowsNum) {
		int xM; int yM;
		glfwGetMonitorPos(monitors[windowsNum], &xM, &yM); // We take the first monitor
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[windowsNum]);

        cout << "Monitor " << windowsNum << " size: " << desktopMode->width << "x" << desktopMode->height << endl;
		
		if (windowsNum == 0)
		{
			// Keep the control window large enough for readable setup controls.
			const int width = std::min(desktopMode->width - 20, std::max(1900, static_cast<int>(desktopMode->width * 0.98f)));
			const int height = std::min(desktopMode->height - 40, std::max(1100, static_cast<int>(desktopMode->height * 0.96f)));
			settings.setSize(width, height);
			settings.setPosition(ofVec2f(xM + (desktopMode->width - width) / 2, yM + (desktopMode->height - height) / 2));
		}
		else
		{
			// Projector window full screen
			settings.setSize(desktopMode->width, desktopMode->height);
			settings.setPosition(ofVec2f(xM, yM));
		}

		return true;
	}
	else {
		if (windowsNum == 0)
		{
			settings.setSize(1280, 900); // Visible fallback for the control interface
			settings.setPosition(ofVec2f(40, 40));
		}
		else
		{
			settings.setSize(1280, 800); // Projector fallback
			settings.setPosition(ofVec2f(80, 80));
		}
		return false;
	}

}

//========================================================================
int main() {
	ofGLFWWindowSettings settings;
//	setFirstWindowDimensions(settings);
	//settings.width = 1200;
 //	settings.height = 600;
	setWindowDimensions(settings, 0);
	settings.windowMode = OF_WINDOW;
	settings.resizable = true;
	settings.decorated = true;
	settings.title = "Magic-Sand " + MagicSandVersion;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
	bool projectorDisplayDetected = setWindowDimensions(settings, 1);
	settings.monitor = 1;
	settings.windowMode = projectorDisplayDetected ? OF_FULLSCREEN : OF_WINDOW;
	settings.resizable = false;
	settings.decorated = false;
	settings.shareContextWith = mainWindow;
	shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);
	secondWindow->setVerticalSync(true);

	shared_ptr<ofApp> mainApp(new ofApp);
	ofAddListener(secondWindow->events().draw, mainApp.get(), &ofApp::drawProjWindow);
	mainApp->projWindow = secondWindow;
	mainApp->projectorDisplayDetected = projectorDisplayDetected;
		
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
