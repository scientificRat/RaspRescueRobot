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

    CarHardware::CarHardware(char* device, int baud):
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

    void CarHardware::run(char* cmd){
            /*
            int mLeft = (int) speed * left;
            int mRight = (int) speed * right;
            if (mLeft == 0 && mRight == 0 ){
                softPwmWrite (MOTOR_LEFT_1,0);
                softPwmWrite (MOTOR_LEFT_2,0);
                softPwmWrite (MOTOR_RIGHT_1,0);
                softPwmWrite (MOTOR_RIGHT_2,0);
            }else if(mLeft < 0 && mRight < 0 ){
                softPwmWrite (MOTOR_LEFT_1,0);
                softPwmWrite (MOTOR_LEFT_2,-mLeft);
                softPwmWrite (MOTOR_RIGHT_1,0);
                softPwmWrite (MOTOR_RIGHT_2,-mRight);
            }else if(mLeft < 0 && mRight > 0 ){
                softPwmWrite (MOTOR_LEFT_1,0);
                softPwmWrite (MOTOR_LEFT_2,-mLeft);
                softPwmWrite (MOTOR_RIGHT_1,mRight);
                softPwmWrite (MOTOR_RIGHT_2,0);
            }else if(mLeft > 0 && mRight < 0 ){
                softPwmWrite (MOTOR_LEFT_1,mLeft);
                softPwmWrite (MOTOR_LEFT_2,0);
                softPwmWrite (MOTOR_RIGHT_1,0);
                softPwmWrite (MOTOR_RIGHT_2,-mRight);
            }else if(mLeft > 0 && mRight > 0 ){
                softPwmWrite (MOTOR_LEFT_1,mLeft);
                softPwmWrite (MOTOR_LEFT_2,0);
                softPwmWrite (MOTOR_RIGHT_1,mRight);
                softPwmWrite (MOTOR_RIGHT_2,0);
            }*/

         char* command = new char[2*sizeof(float)+1];
         strncpy(command, cmd, 2*sizeof(float));
         serialPuts (serialFd, command);
         delete[] command;
    }

    //release car resources
    void CarHardware::release(){
        this->carRun = false;
        serialClose(serialFd);
        delete this->car;
        this->car = nullptr;
    }
}
