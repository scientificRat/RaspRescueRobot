//
//  VideoStreamer.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "VideoStreamer.hpp"

namespace rr{
void VideoStreamer::start(){
    this->imageProcessUnit->start();
    this->isStop = false;
    while (!this->isStop) {
		 std::vector<uchar>& image=this->imageProcessUnit->getEncodeImage();
		 sendVideoFrame(image);
    }
    this->imageProcessUnit->stop();
}

void VideoStreamer::stop(){
    this->isStop=true;
}

void VideoStreamer::sendVideoFrame(std::vector<uchar>& imageData){
    TCPComponent& tcpComponent = TCPComponent::getInstance();
    char type='v';
    int length=static_cast<int>(imageData.size());
    tcpComponent.sendMessage(&type,1);
    tcpComponent.sendMessage(&length,4);
    tcpComponent.sendMessage(imageData.data(), imageData.size());

}

}
