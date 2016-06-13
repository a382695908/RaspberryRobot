#ifndef RASPI_VISION__
#define RASPI_VISION__

#include <iostream>
#include <cstdint>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/time.h>

#include "Controler.h"
#include "ImageProcess.h"

extern std::ostream& log_;

namespace rv{
	class RaspiVision{
	public:
		const static int DIFF_THRESHOLD = 10;
		cv::Mat dupImage();
		void stop()
		{
			mStop = true;
		}
		bool isStop() const
		{
			return mStop;
		}
		void setImageSize(std::uint16_t height, std::uint16_t width)
		{
			mCamera.set(CV_CAP_PROP_FRAME_WIDTH, width);
			mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, height);
			mImage.create(height, width, CV_8UC3);
		}

		//set image brightness
		void setImageBrightness(std::uint16_t birhtness)
		{
			mCamera.set(CV_CAP_PROP_BRIGHTNESS, birhtness);
		}

		//set image contrast
		void setImageContrast(std::uint16_t contrast)
		{
			mCamera.set(CV_CAP_PROP_CONTRAST, contrast );
		}

		//set image saturation
		void setImageSaturation(std::uint16_t saturation)
		{
			mCamera.set( CV_CAP_PROP_SATURATION, saturation);
		}

		int getImageBrightness()
		{
			return mCamera.get(CV_CAP_PROP_BRIGHTNESS);
		}

		int getImageContrast()
		{
			return mCamera.get(CV_CAP_PROP_CONTRAST);
		}

		int getImageSaturation()
		{
			return mCamera.get(CV_CAP_PROP_SATURATION);
		}
		int getImageWidth()
		{
			return mCamera.get(CV_CAP_PROP_FRAME_WIDTH);
		}

		int getImageHeight()
		{
			return mCamera.get(CV_CAP_PROP_FRAME_HEIGHT);
		}

		static RaspiVision& GetInstance()
		{
			if (sRaspiVision == nullptr){
				sRaspiVision = new RaspiVision(Controler::GetInstance(), 240, 320);
				log_ << "RapiVision initial complete." << std::endl;
			}
			return *sRaspiVision;
		}

		static void Release()
		{
			sRaspiVision->stop();
			delete sRaspiVision;
			sRaspiVision = nullptr;
			log_ << "RapiVision release complete." << std::endl;
		}

		~RaspiVision()
		{
			mCamera.release();
		}

	private:
		//Single instance
		RaspiVision(Controler& controler, std::uint16_t height, std::uint16_t width) :
		mControler(controler),
		mImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0)), mStop(false)
		{

			mCamera.set(CV_CAP_PROP_GAIN,100);
			//mCamera.set(RASPI_CAM_ISO,800);
			//set camera brightness
			mCamera.set( CV_CAP_PROP_BRIGHTNESS, 55 );
			//set camera contrast
			mCamera.set( CV_CAP_PROP_CONTRAST, 50 );
			//set camera saturation
			mCamera.set( CV_CAP_PROP_SATURATION, 50 );

			mCamera.set(CV_CAP_PROP_FRAME_WIDTH, width);
			mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, height);
			//mCamera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );

			if (!mCamera.open()){
				std::cerr << "Camera open failed." << std::endl;
			}
			log_ << "RapiVision camera opened." << std::endl;

			mRunnable = std::move(std::thread(msCallback, this));
			mRunnable.detach();
		}

		raspicam::RaspiCam_Cv mCamera;
		std::thread mRunnable;
		std::condition_variable mCV;
		std::mutex mMutex;
		cv::Mat mImage;
		Controler& mControler;
		bool mStop;

		//Process the picture captured from camera to image
		void mProcessImage();

		//get current time, using for debug
		long getCurrentTime();

		//This that is that this
		static void msCallback(RaspiVision* that);

		static RaspiVision* sRaspiVision;
	};
}
#endif
