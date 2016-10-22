//  ImageProcessUnit.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 25/9/16.
//  Copyright © 2016 robotcloud. SCU. All rights reserved.

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
     
    //  /*
    //  * encode image (from mat to vecotr<uchar>)
    //  */
    //  void ImageProcessUnit::encodeImage(cv::Mat& mImage){        
    //   this->processImage(mImage);
    //   cv::imencode(".jpg", mImage, this->mEncodeImage, std::vector<int>());
    //  }
     
     void ImageProcessUnit::processImage(cv::Mat& mImage){
         //empty
     }
}