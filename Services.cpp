//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 11/11/2016
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
    lightState(false),
    detectorState(false){
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
                if (that->detectorIsStarted()) {
                     that->stopDetector();
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

    void Services::startDetector() {

    }

    void Services::stopDetector() {

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

    bool Services::detectorIsStarted() {
         return this->detectorState;
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

    void Services::goOneStep(int direction){
         this->car = rr::CarHardware::getInstance();
         switch(direction){
             case RASPBERRY_ROBOT_DIRECTION_FORWARD:
                 this->car->goForwardOneStep();
                 break;
             case RASPBERRY_ROBOT_DIRECTION_LEFT:
                 this->car->turnLeftOneStep();
                 break;
             case RASPBERRY_ROBOT_DIRECTION_RIGHT: 
                 this->car->turnRightOneStep();
                 break;
             case RASPBERRY_ROBOT_DIRECTION_BACK:
                 this->car->goBackOneStep();
                 break;
             default:
                 throw std::runtime_error("car direction error.");
        }
    }

    void Services::turnLight(int state) {
         this->car = rr::CarHardware::getInstance();
         switch(state) {
             case RASPBERRY_ROBOT_LIGHT_STATE_ON:
                 this->lightState = true;
                 this->car->turnLightOn();
                 break;
             case RASPBERRY_ROBOT_LIGHT_STATE_OFF:
                 this->lightState = false;  
                 this->car->turnLightOff();  
                 break;
             default: 
                 throw std::runtime_error("light state error.");            
         }
    }

    void Services::setDelayTime(long delayTime){
         this->videoStreamer->setDelayTime(delayTime);
    }
}
