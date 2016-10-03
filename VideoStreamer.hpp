//
//  VideoStreamer.hpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef VideoStreamer_hpp
#define VideoStreamer_hpp
#include <vector>
#include "TCPComponent.hpp"
#include "ImageProcessUnit.hpp"
namespace rr{
    class VideoStreamer {
    private:
        ImageProcessUnit *imageProcessUnit = nullptr;
        bool isStop;
    public:
        
        VideoStreamer():isStop(false) {
            imageProcessUnit = new ImageProcessUnit();
        }
        
        void start();
        void stop();
        void setConstract();
        
    private:
        void sendVideoFrame(std::vector<u_char>& imageData);
        
    };
}


#endif /* VideoStreamer_hpp */
