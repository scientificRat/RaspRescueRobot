//
//  VideoStreamer.h
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

<<<<<<< HEAD:VideoStreamer.h
#ifndef RASPBERRY_ROBOT_VIDEOSTREAMER__
#define RASPBERRY_ROBOT_VIDEOSTREAMER__
=======
#ifndef VideoStreamer_hpp
#define VideoStreamer_hpp

>>>>>>> origin/master:VideoStreamer.hpp
#include <vector>

#include "TCPComponent.h"
#include "ImageProcessUnit.h"

namespace rr {
    class VideoStreamer {
    private:
        ImageProcessUnit* imageProcessUnit = nullptr;
        bool isStop;
    public:
<<<<<<< HEAD:VideoStreamer.h
        
        VideoStreamer();        
=======

        VideoStreamer() : isStop(false) {
            imageProcessUnit = new ImageProcessUnit();
        }

>>>>>>> origin/master:VideoStreamer.hpp
        void start();

        void stop();

        void setConstract();

    private:
        void sendVideoFrame(std::vector<uchar> &imageData);

    };
}


#endif /* VideoStreamer_hpp */
