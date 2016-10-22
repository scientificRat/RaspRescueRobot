//
//  VideoStreamer.h
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//


#ifndef RASPBERRY_ROBOT_VIDEOSTREAMER__
#define RASPBERRY_ROBOT_VIDEOSTREAMER__


#include <vector>

#include "TCPComponent.h"
#include "ImageProcessUnit.h"

namespace rr {
    class VideoStreamer {
    private:
        ImageProcessUnit* imageProcessUnit = nullptr;
        bool isStop;
    public:
        
        VideoStreamer();     
           
        void start();

        void stop();

        void setConstract();

    private:
        void sendVideoFrame(std::vector<uchar> &imageData);

    };
}


#endif /* RASPBERRY_ROBOT_VIDEOSTREAMER__*/
