//
//  TCPComponent.h
//  rescueRobot
//
//  Created by 黄正跃 on 25/09/2016.
//  Last Modified by Wang han on 28/10/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef RASPBERRY_ROBOT_TCPCOMPOENT__
#define RASPBERRY_ROBOT_TCPCOMPOENT__

#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <mutex>
#include <string>
#include <errno.h>

#include "dist/json/json.h"

namespace rr {

    class TCPComponent {
    private:
        TCPComponent();
        //(this that is that this)
        static void receive(TCPComponent *that);

        bool loginState;
        bool recieveThreadRun;
        sockaddr_in workingAddr;
        sockaddr_in serverAddr;
        int sockfd;
        std::mutex sendMutex;
        std::thread* receiveThread;
        static std::thread* sendThread;

        //server params
        char* serviceAdrress;
        int servicePort;

    public:
        //thread-safe singleton
        static TCPComponent &getInstance() {
            static TCPComponent instance;
            if (instance.sockfd < 0) {
                throw std::runtime_error("socket initial failed");
            }
            return instance;
        }

        //delete these two dangerous function
        TCPComponent(const TCPComponent&) = delete;
        TCPComponent& operator=(const TCPComponent&)  = delete;

        //the interface to send raw Message
        void sendMessage(const void *data, int length);

        //the interface to send string data
        void sendRequest(const char* JSONBytes, int length);
        
        //login server
        void login();

        //reconnection server
        void reconnection();

        /*
        //set server address
        static void setServerAddress(char* address);
        */
    };
}

#endif /* RASPBERRY_ROBOT_TCPCOMPOENT__*/
