//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

<<<<<<< HEAD
#include "Services.h"
namespace rr{

	//constructor
	Services::Services(){
         videoStreamer = new VideoStreamer();
    }

    //get single instance
    static Services& Services::getInstance(){
         static Services services;
         return services;
    }

    void Services::startVedioStreamer(){
         this->videoStreamer->start();
    }
    
    void Services::startConnection(){
         TCPComponent& tcpComponent =  rr::TCPComponent::getInstance();
         tcpComponent.init();
         // tcpComponent.receive(&tcpComponent);
    }
=======
#include "Services.hpp"

namespace rr {
    void Services::startVedioStreamer() {
        this->videoStreamer->start();
    }

    void Services::startConnection() {
        TCPComponent &tcpComponent = rr::TCPComponent::getInstance();
        tcpComponent.init();
        // tcpComponent.receive(&tcpComponent);
    }
>>>>>>> origin/master
}

