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

	// Setup kinectProjector
	kinectProjector = std::make_shared<KinectProjector>(projWindow);
	kinectProjector->setProjectorDisplayDetected(projectorDisplayDetected);
	kinectProjector->setup(true);
	
	// Setup sandSurfaceRenderer
	sandSurfaceRenderer = new SandSurfaceRenderer(kinectProjector, projWindow);
	sandSurfaceRenderer->setup(true);
	
	// Retrieve variables
	ofVec2f kinectRes = kinectProjector->getKinectRes();
	ofVec2f projRes = ofVec2f(projWindow->getWidth(), projWindow->getHeight());
	updateMainWindowLayout();

	boidGameController.setup(kinectProjector);
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

	if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		sandSurfaceRenderer->drawMainWindow(x, y, w, h);//400, 20, 400, 300);
		boidGameController.drawMainWindow(x, y, w, h);
	}

	kinectProjector->drawMainWindow(x, y, w, h);
}

void ofApp::drawProjWindow(ofEventArgs &args) 
{
	if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		sandSurfaceRenderer->drawProjectorWindow();
		boidGameController.drawProjectorWindow();
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
	const float guiReserve = ofClamp(ofGetWidth() * 0.36f, 480.0f, 660.0f);
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
	ofSetColor(24, 27, 30);
	ofDrawRectangle(controlsSectionROI);

	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(70, 78, 84);
	ofDrawRectangle(previewSectionROI);
	ofDrawRectangle(controlsSectionROI);

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
