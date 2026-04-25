#pragma once

#include "ofMain.h"

enum class DepthCameraType {
	Auto,
	KinectV1,
	KinectV2
};

class DepthCamera {
public:
	virtual ~DepthCamera() = default;

	virtual std::string getName() const = 0;
	virtual bool setup() = 0;
	virtual bool open() = 0;
	virtual void update() = 0;
	virtual void close() = 0;
	virtual bool isConnected() const = 0;
	virtual bool isFrameNew() const = 0;
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual const ofShortPixels& getRawDepthPixels() const = 0;
	virtual const ofPixels& getColorPixels() const = 0;
	virtual ofMatrix4x4 getWorldMatrix() const = 0;
};
