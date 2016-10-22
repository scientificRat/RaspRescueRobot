//
//  Services.h
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef RASPBERRY_ROBOT_SERVICES__
#define RASPBERRY_ROBOT_SERVICES__

#include "VideoStreamer.h"

namespace rr{
     class Services {
     private:
         VideoStreamer *videoStreamer =nullptr;
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
        
         void move(float,float);
        
    };

}


#endif /* Services_hpp */
