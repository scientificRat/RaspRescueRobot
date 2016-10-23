//
//  Services.h
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Last Modified by Wang han on 23/10/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef RASPBERRY_ROBOT_SERVICES__
#define RASPBERRY_ROBOT_SERVICES__

#include "VideoStreamer.h"
#include "CarHardware.h"

namespace rr{
     class Services {
     private:
         VideoStreamer *videoStreamer = nullptr;
         CarHardware* car = nullptr;
         bool hardwareState;
     public:
         static Services& getInstance(){
			static Services services;
			return services;
		}
        
         Services();
        
         Services(const Services&)   = delete;
         Services& operator=(const Services&)  = delete;
        
         void startVedioStreamer();
        
         void startConnection();
        
         void startMovementHardware();

         //stop and release hardware resources
         void stopMovementhardwar();     

         bool hardwareIsStarted();   
         
         void move(float left,float right);

    };

}


#endif /* RASPBERRY_ROBOT_SERVICES__*/
