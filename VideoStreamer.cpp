//
//  VideoStreamer.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 7/11/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include <sys/time.h>

#include "VideoStreamer.h"

#define DEBUG
namespace rr{

    //constructor
    VideoStreamer::VideoStreamer()
    :isStop(false),
    delayTime(50000) {
        imageProcessUnit = new ImageProcessUnit();
    }

    //start VideoStramer
    void VideoStreamer::start() {
         this->isStop = false;
         this->imageProcessUnit->start();
         while (!this->isStop) {
             std::vector<uchar> &image = this->imageProcessUnit->getEncodeImage();
             sendVideoFrame(image);
             //avoid TCP flow control
             usleep(this->delayTime);
         }
         this->imageProcessUnit->stop();

    }

    void VideoStreamer::stop() {
         this->isStop = true;
    }

    int VideoStreamer::getImageProperty (int propId) {
         return imageProcessUnit->getImageProperty(propId);
    }

    bool VideoStreamer::setImageProperty (int propId,int value) {
         return imageProcessUnit->setImageProperty(propId,value);
    }

    void VideoStreamer::setDelayTime(long delayTime){
         this->delayTime = delayTime;
    }

    void VideoStreamer::sendVideoFrame(std::vector<uchar> &imageData) {
        TCPComponent &tcpComponent = TCPComponent::getInstance();
        char type = 'v';
        int length = static_cast<int>(imageData.size());

/*
        //just for debug
         #ifdef DEBUG
         std::cout << "imageData length: " << length << std::endl;
         std::cout <<"In "<<__FILE__<<" , at "<<__LINE__<<" line."<<std::endl;
         #endif
*/

        tcpComponent.sendMessage(&type, 1);
        tcpComponent.sendMessage(&length, 4);
        tcpComponent.sendMessage(imageData.data(), imageData.size());

    }

}
