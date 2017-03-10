//  NPDDector.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 11/11/16.
//  Copyright 漏 2016 rescueRobot. SCU. All rights reserved.


#include <sys/time.h>
#include <omp.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "NPDDector.h"
#include "NPDLearnGAB.h"

namespace rr {
	
	NPDDector* NPDDector::dector = nullptr;

    NPDDector::NPDDector(){
        Options& opt = Options::GetInstance();
        this->Gab = new GAB();
        this->Gab->LoadModel(opt.outFile);
    }
    
    void NPDDector::release() {
        delete this->dector;
        this->dector = nullptr;
        delete this->Gab;
        this->Gab = nullptr;
        std::cout <<"NPD dector recourse release."<<std::endl;
    }
    
    void NPDDector::Detect(cv::Mat& img){

        timeval start, end;
        float time = 0;
        
        cv::cvtColor(img, img, CV_BGR2GRAY);
        std::vector<cv::Rect> rects;
        std::vector<float> scores;
        std::vector<int> index;
        gettimeofday(&start,NULL);
        index = this->Gab->DetectFace(img,rects,scores);
        gettimeofday(&end,NULL);
        float t = 1000 * (end.tv_sec-start.tv_sec)+ (end.tv_usec-start.tv_usec)/1000;
        printf("use time:%f\n",t);
        
        for(int i = 0;i < index.size(); i++){
            printf("%d %d %d %d %lf\n", rects[index[i]].x, rects[index[i]].y, rects[index[i]].width, rects[index[i]].height, scores[index[i]]);
            for (int i = 0; i < index.size(); i++) {
                if(scores[index[i]]>0)
                    img = this->Gab->Draw(img, rects[index[i]]);
            }
        }
    }
}//end of rr