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

#include "dist/json/json.h"

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
		//init TCPComponent
		void init();
        
    private:
        
        TCPComponent();  
		//(this that is that this)
        static void receive(TCPComponent *that);
        

    };
}

#endif /* TCPComponent_hpp */
