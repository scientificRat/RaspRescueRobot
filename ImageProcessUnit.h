//  ImageProcess.h
//  rescueRobot
//
//  Created by Wang Han.SCU on 25/9/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.
#ifndef RASPBERRY_ROBOT_IMAGE_PROCESS__
#define RASPBERRY_ROBOT_IMAGE_PROCESS__

#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ImageCaptureUnit.h"

namespace rr {
    class ImageProcessUnit {
    public:
        /*
        * Consturctor
        */
        ImageProcessUnit(bool processCommand = false);

        /*
         * Deconstructor
        */
        ~ImageProcessUnit();

        /*
        * Start processing image
        */
        void start();

        /*
        * Stop processing image
        */
        void stop();

        /*
        * Identify image run or stop
        */
        bool isStop();

        /*
        * Get encode image
        */
        std::vector <uchar> &getEncodeImage();

         /*
         * Get image property
         */
        int getImageProperty (int propId);

         /*
         * Set image property
         */
        bool setImageProperty (int propId,int value);

        void startProcessImage();

        void stopProcessImage();

        bool ProcessState();

    private:
        ImageCaptureUnit *mImageCaptureUnit;

        std::vector <uchar> mEncodeImage;

        bool processCommand;

        /*
        * Encode image (from mat to vecotr<uchar>)
         */
        void encodeImage(cv::Mat &mImage);

        /*
        * Process image
        */
        void processImage(cv::Mat &mImage);

    };
}
#endif /*RASPBERRY_ROBOT_IMAGE_PROCESS__*/