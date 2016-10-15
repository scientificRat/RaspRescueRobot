//  ImageCaptureUnit.h
//  rescueRobot
//
//  Created by Wang Han.SCU on 25/9/16.
//  Copyright ? 2016 robotcloud. SCU. All rights reserved.

#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>

#ifndef RASPBERRY_ROBOT_IMAGE_CAPTURE__
#define RASPBERRY_ROBOT_IMAGE_CAPTURE__

namespace rr{
	 class ImageCaptureUnit{
		 raspicam::RaspiCam_Cv *mCamera = nullptr;
		 public:
			 /*
			 * Constructor
			 */
			 ImageCaptureUnit(int height = 240 ,int width = 320);
			
			 /*
			 * Destructor
			 */
			 ~ImageCaptureUnit();

			 /*
			 * Set image height
			 */
			 void setImageHeight(int height);
			 
			 /*
			 * Get image height
			 */
			 int getImageHeight();
			 
			 /*
			 *Set image width
			 */
			 void setImageWidth(int width);
			 
			 /*
			 * Get image width
			 */
			 int getImageWidth();
			 
			 /*
			 *set image brightness
			 */
			 void setImageBrightness(int birhtness);
			 
			 /*
			 * Get image birhtness
			 */ 
			 int getImageBrightness();
			 
			 /*
			 * Set image contrast
			 */
			 void setImageContrast(int contrast);
			 
			 /*
			 * Get image contrast
			 */ 
			 int getImageContrast();
			 
			 /*
			 * Set image saturation
			 */
			 void setImageSaturation(int saturation); 
			 
			 /*
			 * Get image saturation
			 */ 
			 int getImageSaturation();
			 
			 /*
			 * Capture video
			 */
			 void start();
			
			 /*
			 * Init camera value
			 */
			 bool init(raspicam::RaspiCam_Cv* camera,int height = 240, int width =320 );
			 
			 /*
			 * Stop capturing video
			 */
			 void stop();
			 
			 /*
			 * Identify camera run or stop
			 */
			 bool isStop();
			 
			 /*
			 * Get a frame one by one
			 */
			 cv::Mat& getImage();
			 
		 private:
			 cv::Mat mImage;			 
			 bool mStop;
	};
}
#endif