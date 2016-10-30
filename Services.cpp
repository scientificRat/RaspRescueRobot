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
    hardwareState(false),
    streamerState(false){
         videoStreamer = new VideoStreamer();
    }

    void Services::startVedioStreamer(){
         this->streamerState = true;
         this->videoStreamer->start();
    }

    void Services::stopVedioStreamer(){
         this->streamerState = false;
         this->videoStreamer->stop();
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
        this->car = rr::CarHardware::getInstance();
        this->car->start();
    }

    //stop and release hardware resources
    void Services::stopMovementHardware(){
        this->hardwareState = false;
        this->car = rr::CarHardware::getInstance();
        this->car->release();
    }

    bool Services::hardwareIsStarted(){
        return this->hardwareState;
    }

    bool Services::streamerISStarted(){
        return this->streamerState;
    }

    void Services::move(float left,float right){
        if (true == hardwareIsStarted()){
            this->car = rr::CarHardware::getInstance();
            this->car->run(left,right);
        }else{
            std::cerr<<"You should start hardware at first."<<std::endl;
        }
    }
}
