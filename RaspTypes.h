//
//  RaspTypes.h
//  rescueRobot
//
//  Created by Wang Han on 11/11/16.
//  Last Modified by Wang Han on 11/11/2016
//  Copyright (C) 2016 Wang Han. All rights reserved.
//
#ifndef RASPBERRY_ROBOT_TYPES__
#define RASPBERRY_ROBOT_TYPES__
namespace rr{

    //light state 
    enum RASPBERRY_ROBOT_LIGHT_STATE
    {
         RASPBERRY_ROBOT_LIGHT_STATE_ON,
         RASPBERRY_ROBOT_LIGHT_STATE_OFF
    };

    // direction
    enum RASPBERRY_ROBOT_DIRECTION
    {
         RASPBERRY_ROBOT_DIRECTION_LEFT,
         RASPBERRY_ROBOT_DIRECTION_RIGHT,
         RASPBERRY_ROBOT_DIRECTION_FORWARD,
         RASPBERRY_ROBOT_DIRECTION_BACK
    }
}

#endif /*RASPBERRY_ROBOT_TYPES__*/