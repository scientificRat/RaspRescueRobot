//  ImageProcessUnit.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 25/9/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.

#include "ImageProcessUnit.h"

namespace rr{
    
     /*
     * Consturctor
     */
    ImageProcessUnit::ImageProcessUnit(){
         mImageCaptureUnit  = new ImageCaptureUnit();
    }
    
     /*
     * Deconstructor
         */
     ImageProcessUnit::~ImageProcessUnit(){
         mImageCaptureUnit->stop();
         delete mImageCaptureUnit;
         mImageCaptureUnit  = nullptr;
     }
    
     /*
     * Start processing image 
     */
     void ImageProcessUnit::start(){
         mImageCaptureUnit->start(); 
     }

     /*
     * Stop processing image
     */  
     void ImageProcessUnit::stop(){
         mImageCaptureUnit->stop();
     }
    
     /*
     * Identify image run or stop
     */
     bool ImageProcessUnit::isStop(){
         return mImageCaptureUnit->isStop();
     }

     /*
     * Get encode image 
     */
     std::vector<uchar>& ImageProcessUnit::getEncodeImage(){
         cv::Mat& raw_image =this->mImageCaptureUnit->getImage();
         this->processImage(raw_image);
         cv::imencode(".jpg", raw_image, this->mEncodeImage, std::vector<int>());
         return this->mEncodeImage;
     }
     
     
    void ImageProcessUnit::processImage(cv::Mat& mImage){
    //empty
    }

     /*
     * Get image property
     */
    int ImageProcessUnit::getImageProperty (int propId) {

        switch ( propId ) {
            case CV_CAP_PROP_FRAME_HEIGHT :
                return mImageCaptureUnit->getImageHeight();
            case CV_CAP_PROP_FRAME_WIDTH :
                return mImageCaptureUnit->getImageWidth();
            case CV_CAP_PROP_FPS:
                return 30;
            case CV_CAP_PROP_BRIGHTNESS :
                return mImageCaptureUnit->getImageBrightness();
            case CV_CAP_PROP_CONTRAST :
                return mImageCaptureUnit->getImageContrast(); 
            case CV_CAP_PROP_SATURATION :
                return mImageCaptureUnit->getImageContrast(); 
            default :
                return -1;
        }
    }

     /*
     * Set image property
     */
    bool ImageProcessUnit::setImageProperty (int propId,int value) {

        switch ( propId ) {
            case CV_CAP_PROP_FRAME_HEIGHT :
                 mImageCaptureUnit->setImageHeight(value);
                 break;
            case CV_CAP_PROP_FRAME_WIDTH :
                 mImageCaptureUnit->setImageWidth(value);
                 break;
            case CV_CAP_PROP_BRIGHTNESS :
                 mImageCaptureUnit->setImageBrightness(value);
                 break;
            case CV_CAP_PROP_CONTRAST :
                 mImageCaptureUnit->setImageContrast(value);
                 break; 
            case CV_CAP_PROP_SATURATION :
                 mImageCaptureUnit->setImageContrast(value);
                 break; 
            default :
                return false;
        }
        return true;
    }
}