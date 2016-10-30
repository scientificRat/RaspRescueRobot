//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 23/10/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "Services.h"
#include "CarHardware.h"

namespace rr{

	//constructor
	Services::Services():
    hardwareState(false){
         videoStreamer = new VideoStreamer();
    }

    void Services::startVedioStreamer(){
         this->videoStreamer->start();
    }

    void Services::startConnection(){
         TCPComponent& tcpComponent =  rr::TCPComponent::getInstance();
         tcpComponent.login();
    }

    void Services::startMovementHardware(){
        // int MOTOR_LEFT_1=8;
        // int MOTOR_LEFT_2=9;
        // int MOTOR_RIGHT_1=7;
        // int MOTOR_RIGHT_2=0;
        //create an instance of car
        car = rr::CarHardware::getInstance();
        car->start();
    }

    //stop and release hardware resources
    void Services::stopMovementHardware(){
        car = rr::CarHardware::getInstance();
        car->release();
    }

    bool Services::hardwareIsStarted(){
        return hardwareState;
    }

    void Services::move(float left,float right){
        if (true == hardwareIsStarted()){
            car = rr::CarHardware::getInstance();
            car->run(left,right);
        }else{
            std::cerr<<"You should start hardware at first."<<std::endl;
        }
    }
}
