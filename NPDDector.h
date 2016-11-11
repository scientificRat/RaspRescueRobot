//  NPDDector.h
//  rescueRobot
//
//  Created by Wang Han.SCU on 11/11/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.


#include <opencv2/core/core.hpp>

#include "NPDCommon.h"
#include "NPDLearnGAB.h"

namespace rr {
    // breif Wraper for call Detector 
    class NPDDector{
    public:
        //single image detect
        void Detect(cv::Mat& img);

        //get single instance
        static NPDDector& getInstance(){
            if (dector == nullptr) {
                dector = new NPDDector();
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