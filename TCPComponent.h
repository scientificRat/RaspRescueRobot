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
#include <string>

#include "dist/json/json.h"
#include "RaspTypes.h"

namespace rr {

    class TCPComponent {
    private:

        //static instance
        static TCPComponent* tcpComponent;
        TCPComponent(const char* serviceAdrress,const int servicePort,const int workingPort);

        static void receive(TCPComponent *that);
        //get login name and password
        void getLoginInfo();

        bool loginState;
        bool recieveThreadRun;
        sockaddr_in workingAddr;
        sockaddr_in serverAddr;
        int sockfd;
        std::mutex sendMutex;
        std::thread* receiveThread;
        static std::thread* sendThread;

        std::string loginName;
        std::string password;

    public:

        static TCPComponent& create(const char* serviceAdrress = "123.206.21.185",
            const int servicePort=8902,const int workingPort = 8900){
            if (nullptr == tcpComponent) {
                tcpComponent = new TCPComponent(serviceAdrress,servicePort,workingPort);
            }
            if (tcpComponent->sockfd < 0) {
                throw std::runtime_error("Socket initial failed!");
            } 
            return *tcpComponent;
        }

        //get singleton
        static TCPComponent &getInstance() {
            if (nullptr == tcpComponent) {
                throw std::runtime_error("TCPComponent not created!");
            }
            return *tcpComponent;
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

        void stopConnection();

    };
}

#endif /* RASPBERRY_ROBOT_TCPCOMPOENT__*/
