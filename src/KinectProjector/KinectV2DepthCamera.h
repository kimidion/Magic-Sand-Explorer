#pragma once

#include "DepthCamera.h"
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/packet_pipeline.h>
#include <memory>

class KinectV2DepthCamera : public DepthCamera {
public:
	~KinectV2DepthCamera() override;

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
	libfreenect2::Freenect2 freenect2;
	libfreenect2::Freenect2Device* device = nullptr;
	std::unique_ptr<libfreenect2::SyncMultiFrameListener> listener;
	bool connected = false;
	bool frameNew = false;
	bool setupComplete = false;
	ofShortPixels depthPixels;
	ofPixels colorPixels;
};
