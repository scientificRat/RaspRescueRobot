//  NPDDector.h
//  rescueRobot
//
//  Created by Wang Han.SCU on 11/11/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.

namespace rr {
#include <opencv2/core/core.hpp>

#include "NPDCommon.h"
/* \breif Wraper for call Detector */
    class NPDDector{
    public:
        //single image detect
        void Detect();

        //get single instance
        static NPDDector& getInstance(){
            if (dector == nullptr) {
                dector = new TrainDetector();
            }
            return *dector;
        }

        void release();

    private:
        static NPDDector* dector;
        GAB* Gab = nullptr;
        NPDDector();
    };
}