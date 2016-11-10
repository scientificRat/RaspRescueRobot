//
//  CarHardware.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 22/10/16.
//  Copyright © 2016 robotcloud. SCU. All rights reserved.

#include <string.h>
#include "CarHardware.h"

namespace rr{
    
    CarHardware* CarHardware::car = nullptr;

    std::mutex CarHardware::instanceMutex;

    CarHardware::CarHardware(const char* device, int baud):
        speed(1024),carRun(false) {

            //initial all
            if (wiringPiSetup() <0){
                std::cerr<<"wiring pi setup error."<<std::endl;
                exit(0);
            }

            if ((serialFd= serialOpen(device,baud)) <0){
                std::cerr<<"serial open error."<<std::endl;
                exit(0);
            }else{
                std::cout<<"serial has been opend."<<std::endl;
            }
    }

    void CarHardware::start(){
        this->carRun = true;
    }

    void CarHardware::run(short left_speed, short right_speed){
        //限制范围，避免出错
        if(left_speed >1000){
            left_speed =1000;
        }
        if(left_speed <-1000){
            left_speed =-1000;
        }
        if(right_speed >1000){
            right_speed =1000;
        }
        if(right_speed <-1000){
            right_speed =-1000;
        }
        serialPutchar(serialFd,'c');
        char* buff1 = (char*)&left_speed;
        char* buff2 = (char*)&right_speed;
        serialPutchar(serialFd, buff1[0]);
        serialPutchar(serialFd, buff1[1]);
        serialPutchar(serialFd, buff2[0]);
        serialPutchar(serialFd, buff2[1]);
    }

    void CarHardware::goForwardOneStep(){
        serialPutchar(serialFd,'^');
    }

    void CarHardware::turnLeftOneStep(){
        serialPutchar(serialFd,'<');
    }

    void CarHardware::turnRightOneStep(){
        serialPutchar(serialFd,'>');
    }

    void CarHardware::goBackOneStep(){
        serialPutchar(serialFd,'v');
    }

    void CarHardware::turnLightOn(){
        serialPutchar(serialFd,'l');
    }

    void CarHardware::turnLightOff(){
        serialPutchar(serialFd,'o');
    }
    
    //release car resources
    void CarHardware::release(){
        this->carRun = false;
        serialClose(serialFd);
        delete this->car;
        this->car = nullptr;
    }
}
