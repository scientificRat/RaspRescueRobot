//
//  Services.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "Services.hpp"
namespace rr{
    void Services::startVedioStreamer(){
        this->videoStreamer->start();
    }
}

