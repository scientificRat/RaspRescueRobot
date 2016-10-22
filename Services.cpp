//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 23/10/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "Services.h"
namespace rr{

	//constructor
	Services::Services(){
         videoStreamer = new VideoStreamer();
    }
	
    void Services::startVedioStreamer(){
         this->videoStreamer->start();
    }
    
    void Services::startConnection(){
         TCPComponent& tcpComponent =  rr::TCPComponent::getInstance();
         tcpComponent.login();
    }
}

