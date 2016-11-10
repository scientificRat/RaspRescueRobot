//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 2/11/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>

#include "Services.h"

namespace rr{

	//constructor
	Services::Services():
    hardwareState(false),
    streamerState(false),
    connectionState(false),
		lightState(false){
         this->videoStreamer = new VideoStreamer();
         this->stopThread = new std::thread(stopService, this);
    }
    void Services::stopService(Services* that){
        std::string cmd;
        while(true){
            std::cin >> cmd;
            if (cmd == "QUIT" || cmd == "quit") {
                if (that->streamerIsStarted()) {
                     that->stopVedioStreamer();
                }
                if (that->hardwareIsStarted()) {
                     that->stopMovementHardware();
                }
                if (that->connectionIsStarted()) {
                     that->stopConnection();
                }
                break;
            }
        }
        //stop service
        exit(0);
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
         this->connectionState = true;
         TCPComponent& tcpComponent = rr::TCPComponent::getInstance();
         tcpComponent.login();
    }

    void Services::stopConnection(){
         this->connectionState = false;
         TCPComponent& tcpComponent = rr::TCPComponent::getInstance();
         tcpComponent.stopConnection();
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

    bool Services::streamerIsStarted(){
        return this->streamerState;
    }

    bool Services::connectionIsStarted(){
        return this->connectionState;
    }

    bool Services::lightIsOn() {
				return this->lightState;
		}

    int Services::getImageProperty (int propId) {
         return this->videoStreamer->getImageProperty(propId);
    }

    bool Services::setImageProperty (int propId,int value) {
         return this->videoStreamer->setImageProperty(propId,value);
    }

    void Services::move(short left_speed,short right_speed){
        if (true == hardwareIsStarted()){
            this->car = rr::CarHardware::getInstance();
            this->car->run(left_speed,right_speed);
        }else{
            std::cerr<<"You should start hardware at first."<<std::endl;
        }
    }
    void Services::goForwardOneStep(){
        this->car = rr::CarHardware::getInstance();
        this->car->goForwardOneStep();
    }
    void Services::turnLeftOneStep(){
        this->car = rr::CarHardware::getInstance();
        this->car->turnLeftOneStep();
    }
    void Services::turnRightOneStep(){
        this->car = rr::CarHardware::getInstance();
        this->car->turnRightOneStep();
    }
    void Services::goBackOneStep(){
        this->car = rr::CarHardware::getInstance();
        this->car->goBackOneStep();
    }
    void Services::turnLightOn(){
			  this->lightState = true;
        this->car = rr::CarHardware::getInstance();
        this->car->turnLightOn();
    }
    void Services::turnLightOff(){
				this->lightState = false;
        this->car = rr::CarHardware::getInstance();
        this->car->turnLightOff();
    }

    void Services::setDelayTime(long delayTime){
         this->videoStreamer->setDelayTime(delayTime);
    }
}
