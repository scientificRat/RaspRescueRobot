//
//  main.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 9/15/16.
//  Last Modified by Wang Han on 11/2/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "Services.h"
//#include "TCPComponent.h"

int main(int argc, char* argv[]) {
    /*
     int opt;  
     rr::TCPComponent& tcp =  rr::TCPComponent::getInstance();
     const char* optString= "t:";
     
     opt = getopt(argc, argv, optString);
     while( opt!= -1){
         switch(opt) {
             case 't':
                 tcp.setServerAddress(optarg);
                 break;          
             default:
                 std::cout<<"error params."<<std::endl;
                 break;
         }
         opt == getopt(argc,argv,optString);
    }

    */
     rr::Services& services = rr::Services::getInstance();
     services.startConnection();    
     return 0;
}
