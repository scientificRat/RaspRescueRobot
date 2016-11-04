//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 2/11/2016
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
         TCPComponent& tcpComponent = rr::TCPComponent::getInstance();
         tcpComponent.login();
    }

    void Services::startMovementHardware(){
        this->hardwareState = true;
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

    int Services::getImageProperty (int propId) {
         return videoStreamer->getImageProperty(propId);
    }

    bool Services::setImageProperty (int propId,int value) {
         return videoStreamer->setImageProperty(propId,value);
    }
    
    void Services::move(char* command){
        if (true == hardwareIsStarted()){
            this->car = rr::CarHardware::getInstance();
            this->car->run(command);
        }else{
            std::cerr<<"You should start hardware at first."<<std::endl;
        }
    }
}
