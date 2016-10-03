//
//  TCPComponent.hpp
//  rescueRobot
//
//  Created by 黄正跃 on 25/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef TCPComponent_hpp
#define TCPComponent_hpp

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
namespace rr{

    class TCPComponent {
    private:
        bool recieveThreadRun  = true;
        sockaddr_in workingAddr;
        sockaddr_in serverAddr;
        int sockfd = -1;
        std::mutex sendMutex;
        std::thread* receiveThread = nullptr;
        
    public:
        //thread-safe singleton
        static TCPComponent& getInstance(){
            static TCPComponent instance;
            if(instance.sockfd < 0){
                throw std::runtime_error("socket initial failed");
            }
            return instance;
        }
        
        //delete these two dangerous function
        TCPComponent(const TCPComponent&)    = delete;
        TCPComponent& operator=(const TCPComponent&)    = delete;
        
        //the interface to send raw Message (不用size_t 自己定义的应用层协议只允许int这么大)
        void sendMessage(const void* data, int length);
        //the interface to send string data
        void sendRequest(const char* JSONBytes, int length);
        
    private:
        
        TCPComponent() {
            //create tcp socket
            if(-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))){
                std::cerr<<"socket initial failed\n";
            }
            //set the addr of the local socket
            bzero(&workingAddr, sizeof(sockaddr_in));
            workingAddr.sin_family=AF_INET;
            workingAddr.sin_port=htons(8900);
            workingAddr.sin_addr.s_addr = htonl (INADDR_ANY);
            //bind
            if(-1 == (bind(sockfd, (sockaddr *)&workingAddr, sizeof(workingAddr)))){
                close(sockfd);
                sockfd = -2;
                std::cerr<<"socket bind failed\n";
            }
            //set the address of the remote server
            bzero(&serverAddr, sizeof(sockaddr_in));
            serverAddr.sin_family=AF_INET;
            serverAddr.sin_port=htons(8902);
            serverAddr.sin_addr.s_addr = inet_addr("123.206.21.185");
            //connect
            if(-1 == connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr))){
                close(sockfd);
                sockfd = -3;
                std::cerr<<"socket connect failed\n";
            }
            //start the receiveThread
            receiveThread = new std::thread(receive, this);
            // TODO: 发送登录请求
            sendRequest(xxxxx);
        }
        

        //(this that is that this)
        static void receive(TCPComponent *that);
        
    };
}

#endif /* TCPComponent_hpp */
