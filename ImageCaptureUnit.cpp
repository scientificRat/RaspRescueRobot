//  ImageCaptureUnit.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 25/9/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.

#include "ImageCaptureUnit.hpp"

namespace rr {

    /*
    * Constructor
    */
    ImageCaptureUnit::ImageCaptureUnit(int height, int width) :
            mImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0)), mStop(false) {
        std::cout << "Image capture ready." << std::endl;
    }

    /*
    * Destructor
    */
    ImageCaptureUnit::~ImageCaptureUnit() {
        this->mCamera->release();
        delete this->mCamera;
        this->mCamera = nullptr;
    }

    /*
    * Set image height
    */
    void ImageCaptureUnit::setImageHeight(int height) {
        this->mCamera->set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }

    /*
    * Get image height
    */
    int ImageCaptureUnit::getImageHeight() {
        return this->mCamera->get(CV_CAP_PROP_FRAME_HEIGHT);
    }

    /*
    *Set image width
    */
    void ImageCaptureUnit::setImageWidth(int width) {
        this->mCamera->set(CV_CAP_PROP_FRAME_WIDTH, width);
    }

    /*
    * Get image width
    */
    int ImageCaptureUnit::getImageWidth() {
        return this->mCamera->get(CV_CAP_PROP_FRAME_WIDTH);
    }

    /*
    *set image brightness
    */
    void ImageCaptureUnit::setImageBrightness(int birhtness) {
        this->mCamera->set(CV_CAP_PROP_BRIGHTNESS, birhtness);
    }

    /*
    * Get image brightness
    */
    int ImageCaptureUnit::getImageBrightness() {
        return this->mCamera->get(CV_CAP_PROP_BRIGHTNESS);
    }

    /*
    * Set image contrast
    */
    void ImageCaptureUnit::setImageContrast(int contrast) {
        this->mCamera->set(CV_CAP_PROP_CONTRAST, contrast);
    }

    /*
    * Get image contrast
    */
    int ImageCaptureUnit::getImageContrast() {
        return this->mCamera->get(CV_CAP_PROP_CONTRAST);
    }

    /*
    * Set image saturation
    */
    void ImageCaptureUnit::setImageSaturation(int saturation) {
        this->mCamera->set(CV_CAP_PROP_SATURATION, saturation);
    }

    /*
    * Get image saturation
    */
    int ImageCaptureUnit::getImageSaturation() {
        return this->mCamera->get(CV_CAP_PROP_SATURATION);
    }


    /*
    * Capture video frame
    */
    void ImageCaptureUnit::start() {
        this->mCamera = new raspicam::RaspiCam_Cv();
        init(this->mCamera);

    }


    /*
    * Init camera value
    */
    bool ImageCaptureUnit::init(raspicam::RaspiCam_Cv *camera, int height, int width) {
        bool state = true;
        this->mStop = false;
        //set camera gain
        camera->set(CV_CAP_PROP_GAIN, 100);
        //set camera brightness
        camera->set(CV_CAP_PROP_BRIGHTNESS, 55);
        //set camera contrast
        camera->set(CV_CAP_PROP_CONTRAST, 50);
        //set camera saturation
        camera->set(CV_CAP_PROP_SATURATION, 50);
        //set camera width
        camera->set(CV_CAP_PROP_FRAME_WIDTH, width);
        //set camera height
        camera->set(CV_CAP_PROP_FRAME_HEIGHT, height);
        //set camera format
        camera->set(CV_CAP_PROP_FORMAT, CV_8UC3);

        if (!camera->open()) {
            state = false;
            std::cerr << "Camera open failed." << std::endl;
        } else {
            std::cout << "Raspberry robot's camera opened." << std::endl;
        }

        return state;
    }

    /*
    * Close video frame
    */
    void ImageCaptureUnit::stop() {
        this->mCamera->release();
        this->mStop = true;
        delete this->mCamera;
        this->mCamera = nullptr;
        std::cout << "Raspberry robot's camera closed." << std::endl;
    }

    /*
    * Identify run or stop
    */
    bool ImageCaptureUnit::isStop() {
        return this->mStop;
    }

    /*
    * Get a frame one by one
    */
    cv::Mat &ImageCaptureUnit::getImage() {
        //bool testFlag = true;

        this->mCamera->grab();
        this->mImage.create(this->getImageHeight(), this->getImageWidth(), CV_8UC3);
        this->mCamera->retrieve(this->mImage);

        /*
        //justdebug
        if(testFlag){
            testFlag=false;
            cv::imwrite ("imageTest.jpg",this->mImage );
            std::cout<<"tempImage saved!"<<std::endl;
        }*/
        return this->mImage;
    }


}