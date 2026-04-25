#include "KinectV1DepthCamera.h"

std::string KinectV1DepthCamera::getName() const
{
	return "Kinect v1";
}

bool KinectV1DepthCamera::setup()
{
	kinect.init();
	kinect.setRegistration(true);
	kinect.setUseTexture(false);
	return true;
}

bool KinectV1DepthCamera::open()
{
	return kinect.open();
}

void KinectV1DepthCamera::update()
{
	kinect.update();
}

void KinectV1DepthCamera::close()
{
	kinect.close();
}

bool KinectV1DepthCamera::isConnected() const
{
	return kinect.isConnected();
}

bool KinectV1DepthCamera::isFrameNew() const
{
	return kinect.isFrameNew();
}

unsigned int KinectV1DepthCamera::getWidth() const
{
	return static_cast<unsigned int>(kinect.getWidth());
}

unsigned int KinectV1DepthCamera::getHeight() const
{
	return static_cast<unsigned int>(kinect.getHeight());
}

const ofShortPixels& KinectV1DepthCamera::getRawDepthPixels() const
{
	return kinect.getRawDepthPixels();
}

const ofPixels& KinectV1DepthCamera::getColorPixels() const
{
	return kinect.getPixels();
}

ofMatrix4x4 KinectV1DepthCamera::getWorldMatrix() const
{
	auto mat = ofMatrix4x4();
	if (kinect.isConnected()) {
		ofVec3f a = kinect.getWorldCoordinateAt(0, 0, 1);
		ofVec3f b = kinect.getWorldCoordinateAt(1, 1, 1);
		mat = ofMatrix4x4(b.x - a.x, 0, 0, a.x,
			0, b.y - a.y, 0, a.y,
			0, 0, 0, 1,
			0, 0, 0, 1);
	}
	return mat;
}
