//
//  VideoStreamer.h
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 23/10/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//


#ifndef RASPBERRY_ROBOT_VIDEOSTREAMER__
#define RASPBERRY_ROBOT_VIDEOSTREAMER__

#include <unistd.h>
#include <vector>

#include "TCPComponent.h"
#include "ImageProcessUnit.h"

namespace rr {
    class VideoStreamer {
    private:
         ImageProcessUnit* imageProcessUnit = nullptr;
         bool isStop;
         long delayTime; //microsecond intervals
    public:
        
         VideoStreamer();     
           
         void start();

         void stop();

         void setConstract();

         bool isStarted();

         int getImageProperty (int propId);

         bool setImageProperty (int propId,int value);

         void startProcessImage();

         void stopProcessImage();

         void setDelayTime(long delayTime);

    private:
         void sendVideoFrame(std::vector<uchar> &imageData);

    };
}


#endif /* RASPBERRY_ROBOT_VIDEOSTREAMER__*/
