//
//  CarHardware.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 22/10/16.
//  Copyright © 2016 robotcloud. SCU. All rights reserved.

#include "CarHardware.h"

namespace rr{
    
    CarHardware* CarHardware::car = nullptr;

    std::mutex CarHardware::instanceMutex;

    CarHardware::CarHardware(int motor_left_1,int motor_left_2,int motor_right_1,int motor_right_2)
        :MOTOR_LEFT_1(motor_left_1),
        MOTOR_LEFT_2(motor_left_2),
        MOTOR_RIGHT_1(motor_right_1),
        MOTOR_RIGHT_2(motor_right_2),
        speed(1024),
        carRun(false) {
            wiringPiSetup();//initial all
            softPwmCreate (MOTOR_LEFT_1, 0, 1024);
            softPwmCreate (MOTOR_LEFT_2, 0, 1024);
            softPwmCreate (MOTOR_RIGHT_1, 0, 1024);
            softPwmCreate (MOTOR_RIGHT_2, 0, 1024);
    }

    void CarHardware::start(){
        this->carRun = true;
    }

    void CarHardware::run(float left,float right){
        while (this->carRun){
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
            }
        }
    }

    //release car resources
    void CarHardware::release(){
        this->carRun = false;
        delete this->car;
        this->car = nullptr;
    }
}
