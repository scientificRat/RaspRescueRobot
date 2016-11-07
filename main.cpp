//
//  main.cpp
//  rescueRobot
//
//  Created by Wang Han on 9/15/16.
//  Last Modified by Wang Han on 11/2/2016
//  Copyright (C) 2016 Wang Han. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "Services.h"
#include "TCPComponent.h"

int main(int argc, char** argv) {
    
     char serverAddress[16] = "123.206.21.185";
     int serverPort = 8902;
     int workingPort = 8900;
     long delayTime = 50000;
  
     const char* optString= "a:p:P:t:";
     int opt = getopt(argc,argv,optString);
     while(opt!= -1){
         switch(opt) {
             case 'a':
                 strncpy(serverAddress,optarg,15);
                 std::cout<<"serverAddress : "<<serverAddress <<std::endl;
                 break;          
             case 'p':
                 serverPort = atoi(optarg);
                 std::cout<<"serverPort : "<<serverPort<<std::endl;
                 break; 
             case 'P':
                 workingPort = atoi(optarg);
                 std::cout<<"workingPort : "<<workingPort<<std::endl;
                 break; 
             case 't':
                 delayTime = atol(optarg);
                 std::cout<<"delayTime : "<<delayTime<<std::endl;
                 break;
             default:
                 std::cerr<<"Usage :"<< argv[0]<<" [- a serverAddress] [-p serverPort] [-P workingPort] [-t delayTime]"<<std::endl;
                 exit(EXIT_FAILURE);
         }
         opt = getopt(argc,argv,optString);
    }

    if (optind > argc) {
        std::cerr << "Expected argument after options"<<std::endl;
        exit(EXIT_FAILURE);
    }
    
     rr::Services& services = rr::Services::getInstance();
     services.setDelayTime(delayTime);

     //std::cout<<optind<<argc<<std::endl;
     rr::TCPComponent& tcp =  rr::TCPComponent::create(serverAddress,serverPort,workingPort);
    
     //start connection 
     services.startConnection();    
     return 0;
}
