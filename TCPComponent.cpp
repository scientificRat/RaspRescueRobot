//
//  TCPComponent.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 25/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "TCPComponent.hpp"
#include "Services.hpp"

#include <string>

namespace rr {

    TCPComponent::TCPComponent() {
        //create tcp socket
        if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
            std::cerr << "socket initial failed\n";
        }
        //set the addr of the local socket
        bzero(&workingAddr, sizeof(sockaddr_in));
        workingAddr.sin_family = AF_INET;
        workingAddr.sin_port = htons(8900);
        workingAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        //bind
        if (-1 == (bind(sockfd, (sockaddr * ) & workingAddr, sizeof(workingAddr)))) {
            close(sockfd);
            sockfd = -2;
            std::cerr << "socket bind failed\n";
        }
        //set the address of the remote server
        bzero(&serverAddr, sizeof(sockaddr_in));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8902);
        serverAddr.sin_addr.s_addr = inet_addr("123.206.21.185");
        //connect
        if (-1 == connect(sockfd, (sockaddr * ) & serverAddr, sizeof(serverAddr))) {
            close(sockfd);
            sockfd = -3;
            std::cerr << "socket connect failed\n";
        }
    }

    void TCPComponent::init() {
        //create json data
        Json::Value root;
        Json::FastWriter writer;
        root["requestType"] = Json::Value("login");
        root["loginName"] = Json::Value("caesar");
        root["password"] = Json::Value("123456");
        std::string RequestJson = writer.write(root); //含有'\n'

        //just for debug
        std::cout << "RequestJson :" << RequestJson << std::endl;
        std::cout << "RequestJson.length : " << RequestJson.length() << std::endl;

        // 发送登录请求
        sendRequest(RequestJson.c_str(), RequestJson.length() - 1);

        //check login info
        Services &services = Services::getInstance();
        char *headBuffer = new char[5];
        //读取头部信息(type,length) 共5byte
        read(this->sockfd, headBuffer, 5);
        char *dataBuffer = new char[*((int *) (headBuffer + 1))];
        read(this->sockfd, dataBuffer, *((int *) (headBuffer + 1)));

        //just for debug
        std::cout << dataBuffer << std::endl;

        if (headBuffer[0] == 'm') {
            std::string action = "";
            std::string ResponseJson = std::string(dataBuffer);
            Json::Reader reader;
            Json::Value root;
            if (reader.parse(ResponseJson, root)) {
                action = root["action"].asString();
                //just for debug
                std::cout << action << std::endl;

            }

            if (action == "startVideo") {
                services.startVedioStreamer();
                //start the receiveThreads
                this->receiveThread = new std::thread(receive, this);
                // synchronize threads:
                this->receiveThread->join();
            }
        }

        std::cout << "ResponseJson error" << std::endl;
    }

    void TCPComponent::receive(TCPComponent *that) {
        Services &services = Services::getInstance();
        while (that->recieveThreadRun) {

            char *headBuffer = new char[5];
            //读取头部信息(type,length) 共5byte
            read(that->sockfd, headBuffer, 5);
            char *dataBuffer = new char[*((int *) (headBuffer + 1))];
            read(that->sockfd, dataBuffer, *((int *) (headBuffer + 1)));
            // TODO:具体动作(注意：服务器返回的消息type='m' 内容为json, 控制端发送的消息type ='c'表示命令)

            //debug
            std::cout << dataBuffer << std::endl;

            if (headBuffer[0] == 'c') {
                // FIXME:这里可能需要更改（根据协议情况调整）
                float left = *((float *) dataBuffer);
                float right = *((float *) dataBuffer + 1);
            }
            else if (headBuffer[0] == 'm') {
                std::string action = "";
                std::string ResponseJson = std::string(dataBuffer);
                Json::Reader reader;
                Json::Value root;
                if (reader.parse(ResponseJson, root)) {
                    action = root["action"].asString();
                    std::cout << action << std::endl;
                }

                if (action == "startVideo") {
                    services.startVedioStreamer();
                }

                //just for deubg
                std::cout << "ResponseJson: " << ResponseJson << std::endl;
            }
            else {

            }
            delete[] headBuffer;
            delete[] dataBuffer;
        }
    }

    void TCPComponent::sendMessage(const void *data, int length) {
        this->sendMutex.lock();
        write(this->sockfd, data, length);
        this->sendMutex.unlock();
    }

    void TCPComponent::sendRequest(const char *JSONBytes, int length) {
        char *sendBuffer = new char[5 + length];
        sendBuffer[0] = 'r';
        char *ptr = (char *) &length;
        sendBuffer[1] = *ptr;
        sendBuffer[2] = *(ptr + 1);
        sendBuffer[3] = *(ptr + 2);
        sendBuffer[4] = *(ptr + 3);

        for (int i = 5; i < 5 + length; i++) {
            sendBuffer[i] = JSONBytes[i - 5];
        }

        sendMessage(sendBuffer, 5 + length);
    }
}

