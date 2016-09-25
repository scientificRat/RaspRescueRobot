//
//  VideoStreamer.cpp
//  UDPClientA
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
        std::vector<u_char>& image=this->imageProcessUnit->getEncodeImage();
        sendMessage(image);
    }
    this->imageProcessUnit->stop();
}

void VideoStreamer::stop(){
    this->isStop=true;
}

void VideoStreamer::sendMessage(std::vector<u_char>& imageData){
    TCPComponent& tcpComponent = TCPComponent::getInstance();
    MessageHead messageHead;
    messageHead.type='v';
    messageHead.length=static_cast<int>(sizeof(messageHead)+imageData.size());
    tcpComponent.sendMessage(&messageHead, sizeof(messageHead));
    tcpComponent.sendMessage(imageData.data(), imageData.size());
}

}
