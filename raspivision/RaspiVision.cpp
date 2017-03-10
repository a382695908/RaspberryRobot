//
//  StreamOutputer.cpp
//  raspivision
//
//  Created by WangHan.SCU on 5/1/16.
//  Copyright (C) 2016 SCU. All rights reserved.
//

#include "RaspiVision.h"

std::ostream& log_ = std::cout;

namespace rv{
    cv::Mat RaspiVision::dupImage()
    {
        std::unique_lock<std::mutex> lck(mMutex);
        mCV.wait(lck);

        return mImage;
    }

    long RaspiVision::getCurrentTime()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

    void RaspiVision::mProcessImage()
    {
        //long start = getCurrentTime();
        //rv::sqrtBinary(mImage);
        //long end = getCurrentTime();
        //std::cout<<"Frame :" << 1/((float)(end-start)/(float)1000)<<std::endl;
    }

    void RaspiVision::msCallback(RaspiVision* that)
    {
        log_ << "RapiVision thread started." << std::endl;

        while (!that->mStop){
            //Capture picture
            that->mCamera.grab();
            that->mMutex.lock();
            that->mCamera.retrieve(that->mImage);

            //Process the image
            that->mProcessImage();
            that->mCV.notify_one();
            that->mMutex.unlock();

            //Send image to controler
            that->mControler.recieve(that->mImage);
        }

        log_ << "RapiVision thread ended." << std::endl;
    }

    RaspiVision* RaspiVision::sRaspiVision = nullptr;
}
