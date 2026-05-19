/***********************************************************************
ofApp.cpp - main openframeworks app
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

#include "ofApp.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

void ofApp::setup() {
	// OF basics
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_WARNING);
	ofSetLogLevel("ofFbo", OF_LOG_ERROR);
	ofSetLogLevel("ofShader", OF_LOG_ERROR);
	ofSetLogLevel("ofxKinect", OF_LOG_WARNING);
	runFontsLoaded =
		runFontSmall.load("ofxbraitsch/fonts/Roboto-Regular.ttf", 14, true, true) &&
		runFont.load("ofxbraitsch/fonts/Roboto-Regular.ttf", 18, true, true) &&
		runFontLarge.load("ofxbraitsch/fonts/Roboto-Regular.ttf", 28, true, true);

	// Setup kinectProjector
	kinectProjector = std::make_shared<KinectProjector>(projWindow);
	kinectProjector->setProjectorDisplayDetected(projectorDisplayDetected);
	kinectProjector->setup(true);
	
	// Setup sandSurfaceRenderer
	sandSurfaceRenderer = new SandSurfaceRenderer(kinectProjector, projWindow);
	sandSurfaceRenderer->setup(true);
	sandSurfaceRenderer->setLegacyGuiVisible(false);
	
	// Retrieve variables
	ofVec2f kinectRes = kinectProjector->getKinectRes();
	ofVec2f projRes = ofVec2f(projWindow->getWidth(), projWindow->getHeight());
	updateMainWindowLayout();

	boidGameController.setup(kinectProjector);
	boidGameController.setLegacyGuiVisible(false);
	boidGameController.setProjectorRes(projRes);
	boidGameController.setKinectRes(kinectRes);
	ofRectangle kinectROI = kinectProjector->getKinectROI();
	boidGameController.setKinectROI(kinectROI);

}


void ofApp::update() {
	updateMainWindowLayout();

    // Call kinectProjector->update() first during the update function()
	kinectProjector->update();
   	sandSurfaceRenderer->update();
    
	ofRectangle kinectROI = kinectProjector->getKinectROI();
	boidGameController.setKinectROI(kinectROI);

	boidGameController.update();
}


void ofApp::draw()
{
	drawMainWindowChrome();

	float x = mainWindowROI.x;
	float y = mainWindowROI.y;
	float w = mainWindowROI.width;
	float h = mainWindowROI.height;

	const bool running = kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING;
	if (running)
	{
		sandSurfaceRenderer->drawMainWindow(x, y, w, h);//400, 20, 400, 300);
		boidGameController.drawMainWindow(x, y, w, h);
		drawRunModeSidebar();
		return;
	}

	kinectProjector->drawMainWindow(x, y, w, h);
}

void ofApp::drawProjWindow(ofEventArgs &args) 
{
	if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		sandSurfaceRenderer->drawProjectorWindow();
		boidGameController.drawProjectorWindow();
		return;
	}
	kinectProjector->drawProjectorWindow();
}

void ofApp::keyPressed(int key) 
{
	if (key == 'c')
	{
		kinectProjector->SaveKinectColorImage();
	}
	else if (key == 'd')
	{
		kinectProjector->SaveFilteredDepthImage();
	}
	else if (key == ' ')
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_SETUP)
		{
			// Try to start the application
			kinectProjector->startApplication();
		}
	}
	else if (key == OF_KEY_RETURN || key == 10 || key == 13)
	{
		kinectProjector->advanceCalibrationPrompt();
	}
	else if (key == OF_KEY_ESC)
	{
		kinectProjector->cancelCalibration("Calibration cancelled. Adjust camera/projector, then try again.");
	}
	else if (key == 'v')
	{
		kinectProjector->cycleCameraFeedRotation();
	}
	else if (key == 'f' || key == 'r')
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartGame(2);
		}
	}
	else if (key == '1') // Absolute beginner
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartGame(0);
		}
	}
	else if (key == '2') 
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartGame(1);
		}
	}
	else if (key == '3')
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartGame(2);
		}
	}
	else if (key == '4')
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartGame(3);
		}
	}
	else if (key == 'm')
	{
		if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartSeekMotherGame();
		}
	}
}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {
	if (handleRunModeDrag(x, y))
	{
		return;
	}
	if (kinectProjector && kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		return;
	}

	if (kinectProjector->handleWorkflowPreviewMouseDragged(x, y, button))
	{
		return;
	}

	// We assume that we only use this during ROI annotation
	ofVec2f previewPoint = windowToKinectPreview(x, y);
	kinectProjector->mouseDragged(previewPoint.x, previewPoint.y, button);
}

void ofApp::mousePressed(int x, int y, int button) 
{
	if (handleRunModeClick(x, y))
	{
		return;
	}
	if (kinectProjector && kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		return;
	}

	if (kinectProjector->handleWorkflowClick(x, y))
	{
		return;
	}

	if (kinectProjector->handleCalibrationClick(x, y))
	{
		return;
	}

	if (kinectProjector->handleWorkflowPreviewMousePressed(x, y, button))
	{
		return;
	}

	if (mainWindowROI.inside((float)x, (float)y))
	{
		ofVec2f previewPoint = windowToKinectPreview(x, y);
		kinectProjector->mousePressed(previewPoint.x, previewPoint.y, button);
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	if (activeRunDragId == "key-height")
	{
		updateRunSliderFromMouse(x);
		if (hasPendingKeyHeight)
		{
			sandSurfaceRenderer->setSelectedColorHeight(pendingKeyHeight);
			hasPendingKeyHeight = false;
		}
	}
	activeRunDragId.clear();
	if (kinectProjector && kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		return;
	}

	if (kinectProjector->handleWorkflowPreviewMouseReleased(x, y, button))
	{
		return;
	}

	// We assume that we only use this during ROI annotation
	ofVec2f previewPoint = windowToKinectPreview(x, y);
	kinectProjector->mouseReleased(previewPoint.x, previewPoint.y, button);

}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

void ofApp::windowResized(int w, int h) {
	updateMainWindowLayout();

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::updateMainWindowLayout()
{
	if (!kinectProjector)
	{
		return;
	}

	ofVec2f kinectRes = kinectProjector->getKinectRes();
	if (kinectRes.x <= 0 || kinectRes.y <= 0)
	{
		return;
	}

	const float margin = ofClamp(ofGetWidth() * 0.025f, 16.0f, 32.0f);
	const float sectionGap = ofClamp(ofGetWidth() * 0.018f, 14.0f, 24.0f);
	const float innerPad = ofClamp(ofGetWidth() * 0.014f, 12.0f, 20.0f);
	const bool running = kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING;
	const float guiReserve = running
		? (runSettingsOpen ? ofClamp(ofGetWidth() * 0.44f, 820.0f, 1040.0f) : 180.0f)
		: ofClamp(ofGetWidth() * 0.36f, 480.0f, 660.0f);
	const bool twoColumn = ofGetWidth() >= 980;

	if (twoColumn)
	{
		previewSectionROI.set(
			margin,
			margin,
			ofGetWidth() - guiReserve - margin * 2.0f - sectionGap,
			ofGetHeight() - margin * 2.0f);
		controlsSectionROI.set(
			previewSectionROI.getMaxX() + sectionGap,
			margin,
			ofGetWidth() - previewSectionROI.getMaxX() - sectionGap - margin,
			previewSectionROI.height);
	}
	else
	{
		const float previewMaxH = std::max(180.0f, ofGetHeight() - 220.0f);
		const float previewH = ofClamp(ofGetHeight() * 0.54f, 180.0f, previewMaxH);
		previewSectionROI.set(margin, margin, ofGetWidth() - margin * 2.0f, previewH);
		controlsSectionROI.set(
			margin,
			previewSectionROI.getMaxY() + sectionGap,
			previewSectionROI.width,
			ofGetHeight() - previewSectionROI.getMaxY() - sectionGap - margin);
	}

	float availableX = previewSectionROI.x + innerPad;
	float availableY = previewSectionROI.y + innerPad;
	float availableW = previewSectionROI.width - innerPad * 2.0f;
	float availableH = previewSectionROI.height - innerPad * 2.0f;

	availableW = std::max(160.0f, availableW);
	availableH = std::max(140.0f, availableH);

	const float scale = std::min(availableW / kinectRes.x, availableH / kinectRes.y);
	const float previewW = kinectRes.x * scale;
	const float previewH = kinectRes.y * scale;

	mainWindowROI.set(
		availableX + (availableW - previewW) * 0.5f,
		availableY + (availableH - previewH) * 0.5f,
		previewW,
		previewH);
}

void ofApp::drawMainWindowChrome() const
{
	ofPushStyle();
	ofBackground(16, 18, 20);
	ofFill();
	ofSetColor(28, 31, 34);
	ofDrawRectangle(previewSectionROI);
	if (kinectProjector && (kinectProjector->GetApplicationState() != KinectProjector::APPLICATION_STATE_RUNNING || runSettingsOpen))
	{
		ofSetColor(24, 27, 30);
		ofDrawRectangle(controlsSectionROI);
	}

	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(70, 78, 84);
	ofDrawRectangle(previewSectionROI);
	if (kinectProjector && (kinectProjector->GetApplicationState() != KinectProjector::APPLICATION_STATE_RUNNING || runSettingsOpen))
	{
		ofDrawRectangle(controlsSectionROI);
	}

	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(92, 106, 118);
	ofDrawRectangle(mainWindowROI);
	ofPopStyle();
}

ofVec2f ofApp::windowToKinectPreview(float x, float y) const
{
	ofVec2f kinectRes = kinectProjector ? kinectProjector->getKinectRes() : ofVec2f(0, 0);
	if (mainWindowROI.width <= 0 || mainWindowROI.height <= 0 || kinectRes.x <= 0 || kinectRes.y <= 0)
	{
		return ofVec2f(0, 0);
	}

	return ofVec2f(
		ofMap(x, mainWindowROI.x, mainWindowROI.getMaxX(), 0, kinectRes.x, true),
		ofMap(y, mainWindowROI.y, mainWindowROI.getMaxY(), 0, kinectRes.y, true));
}

void ofApp::addRunButton(const std::string& id, const ofRectangle& rect)
{
	RunUiHit hit;
	hit.id = id;
	hit.rect = rect;
	runUiHits.push_back(hit);
}

void ofApp::addRunSlider(const std::string& id, const ofRectangle& rect, float minValue, float maxValue)
{
	RunUiHit hit;
	hit.id = id;
	hit.rect = rect;
	hit.minValue = minValue;
	hit.maxValue = maxValue;
	hit.slider = true;
	runUiHits.push_back(hit);
}

void ofApp::drawRunSectionTitle(const std::string& label, float x, float y, float width)
{
	ofSetColor(235, 241, 244);
	drawRunText(label, x, y, 2.0f);
	ofSetColor(74, 84, 90);
	ofDrawLine(x, y + 18.0f, x + width, y + 18.0f);
}

void ofApp::drawRunText(const std::string& text, float x, float y, float scale)
{
	if (!runFontsLoaded)
	{
		ofPushMatrix();
		ofTranslate(x, y);
		ofScale(scale, scale);
		ofDrawBitmapString(text, 0, 0);
		ofPopMatrix();
		return;
	}

	if (scale >= 2.7f)
	{
		runFontLarge.drawString(text, x, y);
	}
	else if (scale >= 1.9f)
	{
		runFont.drawString(text, x, y);
	}
	else
	{
		runFontSmall.drawString(text, x, y);
	}
}

void ofApp::drawRunButton(const ofRectangle& rect, const std::string& label, bool active)
{
	ofFill();
	ofSetColor(active ? ofColor(68, 145, 116) : ofColor(50, 58, 63));
	ofDrawRectRounded(rect, 8.0f);
	ofNoFill();
	ofSetColor(active ? ofColor(116, 220, 176) : ofColor(88, 101, 108));
	ofDrawRectRounded(rect, 8.0f);
	ofFill();
	ofSetColor(240, 244, 246);
	drawRunText(label, rect.x + 16.0f, rect.y + rect.height * 0.5f + 7.0f, 2.0f);
}

void ofApp::drawRunToggle(const ofRectangle& rect, const std::string& label, bool checked)
{
	drawRunButton(rect, label + (checked ? " on" : " off"), checked);
}

void ofApp::drawRunSlider(const ofRectangle& rect, const std::string& label, float value, float minValue, float maxValue, const std::string& valueLabel)
{
	ofSetColor(218, 226, 230);
	drawRunText(label, rect.x, rect.y - 12.0f, 1.0f);
	ofSetColor(154, 166, 172);
	drawRunText(valueLabel, rect.getMaxX() - 72.0f, rect.y - 12.0f, 1.0f);
	ofFill();
	ofSetColor(46, 53, 58);
	ofDrawRectRounded(rect, 8.0f);
	const float t = ofMap(value, minValue, maxValue, 0.0f, 1.0f, true);
	ofSetColor(72, 154, 197);
	ofDrawRectRounded(ofRectangle(rect.x, rect.y, rect.width * t, rect.height), 8.0f);
	ofSetColor(235, 244, 248);
	ofDrawCircle(rect.x + rect.width * t, rect.y + rect.height * 0.5f, 14.0f);
}

void ofApp::drawRunTabs(float x, float y, float width)
{
	const float gap = 16.0f;
	const float tabW = (width - gap) * 0.5f;
	ofRectangle colorTab(x, y, tabW, 52.0f);
	ofRectangle gameTab(x + tabW + gap, y, tabW, 52.0f);
	addRunButton("run-tab-color", colorTab);
	addRunButton("run-tab-game", gameTab);
	drawRunButton(colorTab, "Color", runPanelPage == 0);
	drawRunButton(gameTab, "Game", runPanelPage == 1);
}

void ofApp::drawRunModeSidebar()
{
	runUiHits.clear();
	const float margin = 32.0f;
	const float panelX = controlsSectionROI.x;
	const float panelY = controlsSectionROI.y;
	const float panelW = controlsSectionROI.width;

	ofPushStyle();
	if (!runSettingsOpen)
	{
		ofRectangle tab(panelX + 16.0f, panelY + 22.0f, panelW - 32.0f, 82.0f);
		addRunButton("toggle-settings", tab);
		drawRunButton(tab, "Settings");
		ofPopStyle();
		return;
	}

	ofFill();
	ofSetColor(24, 27, 30, 255);
	ofDrawRectangle(controlsSectionROI);
	ofNoFill();
	ofSetColor(81, 93, 100);
	ofDrawRectangle(controlsSectionROI);

	float x = panelX + margin;
	float y = panelY + 38.0f;
	float w = panelW - margin * 2.0f;
	ofSetColor(250, 252, 252);
	drawRunText("Run Sandbox", x, y, 2.8f);
	ofRectangle closeRect(panelX + panelW - 144.0f, panelY + 18.0f, 112.0f, 52.0f);
	addRunButton("toggle-settings", closeRect);
	drawRunButton(closeRect, "Close");

	y += 54.0f;
	drawRunTabs(x, y, w);
	y += 74.0f;
	const float halfW = (w - 18.0f) * 0.5f;

	if (runPanelPage == 0)
	{
	drawRunSectionTitle("Color Mapping", x, y, w);
	y += 40.0f;
	ofSetColor(154, 166, 172);
	drawRunText("Map", x, y, 2.1f);
	ofSetColor(238, 244, 246);
	drawRunText(sandSurfaceRenderer->getColorMapFile(), x + 104.0f, y, 2.1f);
	y += 28.0f;

	ofRectangle prevRect(x, y, halfW, 52.0f);
	ofRectangle nextRect(x + halfW + 18.0f, y, halfW, 52.0f);
	addRunButton("color-prev", prevRect);
	addRunButton("color-next", nextRect);
	drawRunButton(prevRect, "Previous");
	drawRunButton(nextRect, "Next");
	y += 82.0f;

	std::ostringstream scaleText;
	scaleText << std::fixed << std::setprecision(2) << sandSurfaceRenderer->getHeightMapScale();
	ofRectangle scaleRect(x, y, w, 32.0f);
	addRunSlider("scale", scaleRect, 0.1f, 8.0f);
	drawRunSlider(scaleRect, "Scale", sandSurfaceRenderer->getHeightMapScale(), 0.1f, 8.0f, scaleText.str());
	y += 62.0f;

	std::ostringstream offsetText;
	offsetText << std::fixed << std::setprecision(2) << sandSurfaceRenderer->getHeightMapOffset();
	ofRectangle offsetRect(x, y, w, 32.0f);
	addRunSlider("offset", offsetRect, -2.0f, 2.0f);
	drawRunSlider(offsetRect, "Offset", sandSurfaceRenderer->getHeightMapOffset(), -2.0f, 2.0f, offsetText.str());
	y += 62.0f;

	ofRectangle contourRect(x, y, halfW, 52.0f);
	ofRectangle resetRect(x + halfW + 18.0f, y, halfW, 52.0f);
	addRunButton("contours", contourRect);
	addRunButton("reset-map", resetRect);
	drawRunToggle(contourRect, "Contours", sandSurfaceRenderer->getDrawContourLines());
	drawRunButton(resetRect, "Reset");
	y += 72.0f;

	drawRunSectionTitle("Palette Keys", x, y, w);
	y += 40.0f;
	std::vector<ColorMap::HeightMapKey> keys = sandSurfaceRenderer->getHeightMapKeys();
	if (!keys.empty())
	{
			const float swatchGap = 8.0f;
			const float swatchW = (w - swatchGap * (keys.size() - 1)) / keys.size();
			const int selected = sandSurfaceRenderer->getSelectedColorIndex();
		for (int i = 0; i < keys.size(); ++i)
		{
			int keyIndex = keys.size() - 1 - i;
			ofRectangle swatch(x + i * (swatchW + swatchGap), y, swatchW, 38.0f);
			addRunButton("key-" + ofToString(i), swatch);
			ofSetColor(keys[keyIndex].color);
			ofDrawRectRounded(swatch, 4.0f);
			ofNoFill();
			ofSetLineWidth(i == selected ? 3.0f : 1.0f);
			ofSetColor(i == selected ? ofColor(255, 255, 255) : ofColor(72, 82, 88));
			ofDrawRectRounded(swatch, 4.0f);
			ofFill();
		}
			y += 64.0f;

		int selectedKeyIndex = keys.size() - 1 - ofClamp(selected, 0, static_cast<int>(keys.size()) - 1);
		float keyHeight = (hasPendingKeyHeight && activeRunDragId == "key-height") ? pendingKeyHeight : keys[selectedKeyIndex].height;
		float minHeight = (selectedKeyIndex > 0) ? keys[selectedKeyIndex - 1].height : keyHeight - 100.0f;
		float maxHeight = (selectedKeyIndex < keys.size() - 1) ? keys[selectedKeyIndex + 1].height : keyHeight + 100.0f;
		std::ostringstream heightText;
		heightText << std::fixed << std::setprecision(0) << keyHeight;
		ofRectangle keyHeightRect(x, y, w, 32.0f);
		addRunSlider("key-height", keyHeightRect, minHeight, maxHeight);
		drawRunSlider(keyHeightRect, "Height", keyHeight, minHeight, maxHeight, heightText.str());
		y += 60.0f;

		ofRectangle addRect(x, y, (w - 36.0f) / 3.0f, 52.0f);
		ofRectangle removeRect(addRect.getMaxX() + 18.0f, y, addRect.width, 52.0f);
		ofRectangle moveRect(removeRect.getMaxX() + 18.0f, y, addRect.width, 52.0f);
		addRunButton("key-add", addRect);
		addRunButton("key-remove", removeRect);
		addRunButton("key-move", moveRect);
		drawRunButton(addRect, "Add");
		drawRunButton(removeRect, "Remove");
		drawRunButton(moveRect, "Move");
		y += 82.0f;
	}
	}
	else
	{

	drawRunSectionTitle("Animal Game", x, y, w);
	y += 40.0f;
	for (int i = 0; i < 4; ++i)
	{
		float bw = (w - 42.0f) / 4.0f;
		ofRectangle diffRect(x + i * (bw + 14.0f), y, bw, 68.0f);
		addRunButton("difficulty-" + ofToString(i), diffRect);
		drawRunButton(diffRect, ofToString(i + 1), i == runGameDifficulty);
	}
	y += 82.0f;
	ofRectangle startRect(x, y, w, 76.0f);
	addRunButton("start-game", startRect);
	drawRunButton(startRect, "Start animal game", !boidGameController.isIdle());
	y += 92.0f;
	ofRectangle seekRect(x, y, halfW, 72.0f);
	ofRectangle clearRect(x + halfW + 18.0f, y, halfW, 72.0f);
	addRunButton("seek-mother", seekRect);
	addRunButton("clear-animals", clearRect);
	drawRunButton(seekRect, "Seek mother");
	drawRunButton(clearRect, "Clear");
	}
	ofPopStyle();
}

bool ofApp::handleRunModeClick(float x, float y)
{
	if (!kinectProjector || kinectProjector->GetApplicationState() != KinectProjector::APPLICATION_STATE_RUNNING)
	{
		return false;
	}
	for (const RunUiHit& hit : runUiHits)
	{
		if (!hit.rect.inside(x, y))
		{
			continue;
		}
			if (hit.slider)
			{
				activeRunDragId = hit.id;
				updateRunSliderFromMouse(x);
				return true;
			}
			if (hit.id == "toggle-settings")
			{
				runSettingsOpen = !runSettingsOpen;
			}
			else if (hit.id == "run-tab-color")
			{
				runPanelPage = 0;
			}
			else if (hit.id == "run-tab-game")
			{
				runPanelPage = 1;
			}
			else if (hit.id == "color-prev")
			{
				sandSurfaceRenderer->selectNextColorMap(-1, false);
			}
			else if (hit.id == "color-next")
			{
				sandSurfaceRenderer->selectNextColorMap(1, false);
		}
		else if (hit.id == "contours")
		{
			sandSurfaceRenderer->setDrawContourLines(!sandSurfaceRenderer->getDrawContourLines());
		}
		else if (hit.id == "reset-map")
		{
			sandSurfaceRenderer->resetColorMap();
			sandSurfaceRenderer->resetHeightMapTransform();
		}
		else if (hit.id.rfind("key-", 0) == 0 && hit.id != "key-add" && hit.id != "key-remove" && hit.id != "key-move" && hit.id != "key-height")
		{
			sandSurfaceRenderer->selectColorKey(ofToInt(hit.id.substr(4)));
		}
		else if (hit.id == "key-add")
		{
			sandSurfaceRenderer->insertColorAfterSelected();
		}
		else if (hit.id == "key-remove")
		{
			sandSurfaceRenderer->removeSelectedColor();
		}
		else if (hit.id == "key-move")
		{
			sandSurfaceRenderer->moveSelectedColor(1);
		}
		else if (hit.id.rfind("difficulty-", 0) == 0)
		{
			runGameDifficulty = ofClamp(ofToInt(hit.id.substr(11)), 0, 3);
		}
		else if (hit.id == "start-game")
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartGame(runGameDifficulty);
		}
		else if (hit.id == "seek-mother")
		{
			boidGameController.setDebug(kinectProjector->getDumpDebugFiles());
			boidGameController.StartSeekMotherGame();
		}
		else if (hit.id == "clear-animals")
		{
			boidGameController.clearAnimals();
		}
		return true;
	}
	return runSettingsOpen && controlsSectionROI.inside(x, y);
}

bool ofApp::handleRunModeDrag(float x, float y)
{
	if (activeRunDragId.empty())
	{
		return false;
	}
	updateRunSliderFromMouse(x);
	return true;
}

void ofApp::updateRunSliderFromMouse(float x)
{
	for (const RunUiHit& hit : runUiHits)
	{
		if (hit.id != activeRunDragId)
		{
			continue;
		}
		const float value = ofMap(x, hit.rect.x, hit.rect.getMaxX(), hit.minValue, hit.maxValue, true);
		if (hit.id == "scale")
		{
			sandSurfaceRenderer->setHeightMapScale(value);
		}
		else if (hit.id == "offset")
		{
			sandSurfaceRenderer->setHeightMapOffset(value);
		}
		else if (hit.id == "key-height")
		{
			pendingKeyHeight = value;
			hasPendingKeyHeight = true;
		}
		return;
	}
}
