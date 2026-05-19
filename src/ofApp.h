/***********************************************************************
ofApp.h - main openframeworks app
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

#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "KinectProjector/KinectProjector.h"
#include "SandSurfaceRenderer/SandSurfaceRenderer.h"
#include "Games/BoidGameController.h"
#include <vector>

class ofApp : public ofBaseApp {

public:
	void setup();
 
	void update();

	void draw();
	void drawProjWindow(ofEventArgs& args);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	std::shared_ptr<ofAppBaseWindow> projWindow;
	bool projectorDisplayDetected = false;

private:
		void updateMainWindowLayout();
		void drawMainWindowChrome() const;
		ofVec2f windowToKinectPreview(float x, float y) const;
		void drawRunModeSidebar();
		void drawRunText(const std::string& text, float x, float y, float scale = 1.0f);
		void drawRunButton(const ofRectangle& rect, const std::string& label, bool active = false);
		void drawRunToggle(const ofRectangle& rect, const std::string& label, bool checked);
		void drawRunSlider(const ofRectangle& rect, const std::string& label, float value, float minValue, float maxValue, const std::string& valueLabel);
		void drawRunSectionTitle(const std::string& label, float x, float y, float width);
	bool handleRunModeClick(float x, float y);
	bool handleRunModeDrag(float x, float y);
	void updateRunSliderFromMouse(float x);
		void addRunButton(const std::string& id, const ofRectangle& rect);
		void addRunSlider(const std::string& id, const ofRectangle& rect, float minValue, float maxValue);
		void drawRunTabs(float x, float y, float width);

	struct RunUiHit
	{
		std::string id;
		ofRectangle rect;
		float minValue = 0.0f;
		float maxValue = 1.0f;
		bool slider = false;
	};

	std::shared_ptr<KinectProjector> kinectProjector;
	SandSurfaceRenderer* sandSurfaceRenderer;
	CBoidGameController boidGameController;
		bool runSettingsOpen = false;
		int runPanelPage = 0;
		int runGameDifficulty = 2;
		std::string activeRunDragId;
		float pendingKeyHeight = 0.0f;
		bool hasPendingKeyHeight = false;
		std::vector<RunUiHit> runUiHits;
		ofTrueTypeFont runFontSmall;
		ofTrueTypeFont runFont;
		ofTrueTypeFont runFontLarge;
		bool runFontsLoaded = false;

	// Main window ROI 
	ofRectangle mainWindowROI;
	ofRectangle previewSectionROI;
	ofRectangle controlsSectionROI;
};
