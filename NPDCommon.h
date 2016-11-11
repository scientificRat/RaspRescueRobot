//  NPDCommon.h
//  rescueRobot
//
//  Created by Wang Han.SCU on 11/11/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.

#ifndef RASPBERRY_ROBOT_IMAGE_NPD_COMMON__
#define RASPBERRY_ROBOT_IMAGE_NPD_COMMON__

#include <string>
#include <stdio.h>

/*
 * \breif Configure of NPD
 */
namespace rr {
    class Options{
        public:
            static inline Options& GetInstance() {
                static Options opt;
                return opt;
            }
        
            /* \breif Size of Template */
            int objSize;
            /* \breif a text file for positive dataset */
            std::string faceDBFile;
            /* \breif a text file for negative dataset */
            std::string nonfaceDBFile;
            /* \breif path of model */
            std::string outFile;
            /* \breif path of FDDB */
            std::string fddb_dir;
            /* \breif a text file for resume training status */
            std::string tmpfile;
            /* \breif Init Neg Samples */
            std::string initNeg;
            /* \breif depth of a stage */
            int treeLevel;
            /* \breif max number of stages */
            int maxNumWeaks;
            /* \breif threads to use */
            int numThreads;
            /* \breif recall of positive in every stages */
            double minDR;
            /* \breif end condition of the training */
            double maxFAR;
            /* \breif max value of weight */
            int maxWeight;
            /* \breif factor for decide leaf number */
            double minLeafFrac;
            /* \breif minimum leaf number */
            int minLeaf;
            /* \breif factor to decide how many samples should be filter befor training a stage */
            double trimFrac;
            /* \breif minimum samples required */
            int minSamples;
            /* \breif data augment or not */
            bool augment;
            /* \breif step of stages to save the model */
            int saveStep;
            /* \breif generate init neg if need */
            bool generate_hd;
            /* \breif use for resize box */
            float enDelta;
            /* \use hd or not */
            bool useInitHard;

        private:
            Options();
            Options(const Options& other);
            Options& operator=(const Options& other);

    };
}
#endif /*RASPBERRY_ROBOT_IMAGE_NPD_COMMON__*/
