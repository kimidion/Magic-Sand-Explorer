#pragma once

#include "DepthCamera.h"
#include "ofxKinect.h"

class KinectV1DepthCamera : public DepthCamera {
public:
	std::string getName() const override;
	bool setup() override;
	bool open() override;
	void update() override;
	void close() override;
	bool isConnected() const override;
	bool isFrameNew() const override;
	unsigned int getWidth() const override;
	unsigned int getHeight() const override;
	const ofShortPixels& getRawDepthPixels() const override;
	const ofPixels& getColorPixels() const override;
	ofMatrix4x4 getWorldMatrix() const override;

private:
	ofxKinect kinect;
};
