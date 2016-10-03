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
        char* headBuffer = new char[5];
        //读取头部信息(type,length) 共5byte
        read(that->sockfd,headBuffer, 5);
        char* dataBuffer = new char[*((int*)(headBuffer+1))];
        read(that->sockfd,dataBuffer,messageHead->length);
        // TODO:具体动作(注意：服务器返回的消息type='m' 内容为json, 控制端发送的消息type ='c'表示命令)
        if(headBuffer[0]=='c'){
            // FIXME:这里可能需要更改（根据协议情况调整）
            float xxxxxxxx=*((float*)dataBuffer);
            float oooooooo=*((float*)dataBuffer+1);
        }
        else if(headBuffer[0]=='m'){
            // TODO: 这里需要把包的内容打印出来(JSON)
        }
        else{

        }
        delete[] headBuffer;
        delete[] dataBuffer;
        delete messageHead;
    }
}

void TCPComponent::sendMessage(const void *data, int length) {
    this->sendMutex.lock();
    write(this->sockfd,data,length);
    this->sendMutex.unlock();
}

void TCPComponent::sendRequest(const char* JSONBytes, int length){
    char *sendBuffer = new char[5+length];
    sendBuffer[0] = 'r';
    char* ptr = (char*)&length;
    sendBuffer[1] = *ptr;
    sendBuffer[2] = *(ptr+1);
    sendBuffer[3] = *(ptr+2);
    sendBuffer[4] = *(ptr+3);
    for(int i=5;i<5+length;i++){
        sendBuffer[i] = JSONBytes[i-5];
    }
    sendMessage(sendBuffer);
}


}

