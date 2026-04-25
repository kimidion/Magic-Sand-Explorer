#include "KinectV2DepthCamera.h"
#include <algorithm>
#include <cmath>
#include <limits>

KinectV2DepthCamera::~KinectV2DepthCamera()
{
	close();
}

std::string KinectV2DepthCamera::getName() const
{
	return "Kinect v2";
}

bool KinectV2DepthCamera::setup()
{
	depthPixels.allocate(getWidth(), getHeight(), 1);
	depthPixels.set(0);
	colorPixels.allocate(getWidth(), getHeight(), OF_PIXELS_RGB);
	colorPixels.set(0);
	setupComplete = true;
	return true;
}

bool KinectV2DepthCamera::open()
{
	if (!setupComplete)
	{
		setup();
	}

	if (connected)
	{
		return true;
	}

	if (freenect2.enumerateDevices() == 0)
	{
		ofLogWarning("KinectV2DepthCamera") << "No Kinect v2 devices found";
		return false;
	}

	listener = std::make_unique<libfreenect2::SyncMultiFrameListener>(
		libfreenect2::Frame::Color | libfreenect2::Frame::Depth);

	device = freenect2.openDefaultDevice(new libfreenect2::CpuPacketPipeline());
	if (device == nullptr)
	{
		ofLogWarning("KinectV2DepthCamera") << "Could not open Kinect v2 device";
		listener.reset();
		return false;
	}

	device->setColorFrameListener(listener.get());
	device->setIrAndDepthFrameListener(listener.get());

	connected = device->startStreams(true, true);
	if (!connected)
	{
		ofLogWarning("KinectV2DepthCamera") << "Could not start Kinect v2 streams";
		close();
		return false;
	}

	ofLogVerbose("KinectV2DepthCamera") << "Opened Kinect v2 serial " << device->getSerialNumber()
										<< " firmware " << device->getFirmwareVersion();
	return true;
}

void KinectV2DepthCamera::update()
{
	frameNew = false;
	if (!connected || !listener)
	{
		return;
	}

	libfreenect2::FrameMap frames;
	if (!listener->waitForNewFrame(frames, 10))
	{
		return;
	}

	auto depthFrameIt = frames.find(libfreenect2::Frame::Depth);
	if (depthFrameIt != frames.end())
	{
		libfreenect2::Frame* depthFrame = depthFrameIt->second;
		const float* depthData = reinterpret_cast<const float*>(depthFrame->data);
		unsigned short* out = depthPixels.getData();
		const size_t pixelCount = depthFrame->width * depthFrame->height;

		for (size_t i = 0; i < pixelCount; ++i)
		{
			float depth = depthData[i];
			if (!std::isfinite(depth) || depth <= 0)
			{
				out[i] = 0;
			}
			else
			{
				depth = std::min(depth, static_cast<float>(std::numeric_limits<unsigned short>::max()));
				out[i] = static_cast<unsigned short>(depth);
			}
		}
		frameNew = true;
	}

	auto colorFrameIt = frames.find(libfreenect2::Frame::Color);
	if (colorFrameIt != frames.end())
	{
		libfreenect2::Frame* colorFrame = colorFrameIt->second;
		const unsigned char* colorData = colorFrame->data;
		unsigned char* out = colorPixels.getData();

		for (unsigned int y = 0; y < getHeight(); ++y)
		{
			unsigned int srcY = static_cast<unsigned int>((static_cast<float>(y) / getHeight()) * colorFrame->height);
			srcY = std::min(srcY, static_cast<unsigned int>(colorFrame->height - 1));
			for (unsigned int x = 0; x < getWidth(); ++x)
			{
				unsigned int srcX = static_cast<unsigned int>((static_cast<float>(x) / getWidth()) * colorFrame->width);
				srcX = std::min(srcX, static_cast<unsigned int>(colorFrame->width - 1));
				const size_t srcIdx = (srcY * colorFrame->width + srcX) * colorFrame->bytes_per_pixel;
				const size_t dstIdx = (y * getWidth() + x) * 3;

				if (colorFrame->format == libfreenect2::Frame::BGRX)
				{
					out[dstIdx] = colorData[srcIdx + 2];
					out[dstIdx + 1] = colorData[srcIdx + 1];
					out[dstIdx + 2] = colorData[srcIdx];
				}
				else
				{
					out[dstIdx] = colorData[srcIdx];
					out[dstIdx + 1] = colorData[srcIdx + 1];
					out[dstIdx + 2] = colorData[srcIdx + 2];
				}
			}
		}
	}

	listener->release(frames);
}

void KinectV2DepthCamera::close()
{
	if (device != nullptr)
	{
		if (connected)
		{
			device->stop();
		}
		device->close();
		device = nullptr;
	}
	listener.reset();
	connected = false;
	frameNew = false;
}

bool KinectV2DepthCamera::isConnected() const
{
	return connected;
}

bool KinectV2DepthCamera::isFrameNew() const
{
	return frameNew;
}

unsigned int KinectV2DepthCamera::getWidth() const
{
	return 512;
}

unsigned int KinectV2DepthCamera::getHeight() const
{
	return 424;
}

const ofShortPixels& KinectV2DepthCamera::getRawDepthPixels() const
{
	return depthPixels;
}

const ofPixels& KinectV2DepthCamera::getColorPixels() const
{
	return colorPixels;
}

ofMatrix4x4 KinectV2DepthCamera::getWorldMatrix() const
{
	auto mat = ofMatrix4x4();
	if (device != nullptr)
	{
		auto params = device->getIrCameraParams();
		mat = ofMatrix4x4(
			1.0f / params.fx, 0, 0, -params.cx / params.fx,
			0, 1.0f / params.fy, 0, -params.cy / params.fy,
			0, 0, 0, 1,
			0, 0, 0, 1);
	}
	return mat;
}
