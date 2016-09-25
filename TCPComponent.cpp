//
//  TCPComponent.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 25/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "TCPComponent.hpp"
#include "Services.hpp"
namespace rr{
    
void TCPComponent::receive(TCPComponent *that) {
    Services& services = Services::getInstance();
    while (that->recieveThreadRun){
        MessageHead* messageHead = new MessageHead();
        read(that->sockfd,messageHead, sizeof(messageHead));
        char* dataBuffer = new char[messageHead->length];
        read(that->sockfd,dataBuffer,messageHead->length);
        //TODO:具体动作
        
        
        delete[] dataBuffer;
        delete messageHead;
    }
}

void TCPComponent::sendMessage(const void *data, size_t length) {
    this->sendMutex.lock();
    write(this->sockfd,data,length);
    this->sendMutex.unlock();
}

}

