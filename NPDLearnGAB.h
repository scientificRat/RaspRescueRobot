//  NPDLearnGAB.h
//  rescueRobot
//
//  Created by Wang Han.SCU on 11/11/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.

#ifndef RASPBERRY_ROBOT_IMAGE_NPD_LEARN_GAB__
#define RASPBERRY_ROBOT_IMAGE_NPD_LEARN_GAB__

#include <opencv2/core/core.hpp>
#include <vector>

#include "NPDCommon.h"

namespace rr {
/*
 * breif The Detector for face classification
 */
    class GAB{
    public:
        /*
         * \breif Init Feature map
         * Not only generate feature map and also generate feature-coordinate map
         */
        GAB();
        /*
         * \breif Validate the region is a face or not
         * Go throw all the stages and accumulate the scores, 
         * only if the score passed all the threshold, judge it to be a face
         *
         * param test  the region to be test
         * param score  the score finally it got
         * param sIndex  the index of winSize
         */
        bool NPDClassify(cv::Mat test,float &score,int sIndex);
        /*
         * \breif Get the coordinates by feature id
         * the feature number is calculate by (objSize*objSize)*(objSize*objSize-1)/2
         * so if you have a feature id, use this function to get the coordinates
         * got the coordinates you can calculate the feature value in image.
         * here use two maps which store feature-coordinates
         *
         * \param feaid  Feature Id
         * \param x1  coordinate of point A.x
         * \param x2  coordinate of point A.y
         * \param y1  coordinate of point B.x
         * \param y2  coordinate of point B.y
         */
        void GetPoints(int feaid, int *x1, int *y1, int *x2, int *y2);
        /*
         * \breif Mining Negative Samples
         * Use NextImage to get regions and than use NPDClassify to validate it's a face or not
         * Using a Mining rate to control travel speed
         * Using region_pool for openmp
         *
         * \param n  the negative size final condition
         * \param neg  negative dataset
         */
        void LoadModel(std::string path);
        /*
         * \breif Draw rect in a image
         *
         * \param img  the image need to be draw
         * \param rects  the box
         */
        cv::Mat Draw(cv::Mat& img, cv::Rect& rects);
    public:
        /* \breif indicate how many stages the dector have */
        int stages;
        /* \breif vectors contain the model */
        std::vector<int> treeIndex;
        std::vector<int> feaIds, leftChilds, rightChilds;
        std::vector<unsigned char> cutpoints;
        std::vector<float> fits;
        std::vector<float> thresholds;
        int numBranchNodes;
        /* \breif save the points of feature id */
        std::vector< std::vector<int> > points1,points2;
        /* \breif vector contain point-feature map */
        std::vector<int> lpoints;
        std::vector<int> rpoints;
        /* \breif A feature map used for speed up calculate feature */
        cv::Mat ppNpdTable;
    public:
        /* \breif model template size */
        int DetectSize;
        /* 
         * \breif wraper for Detect faces from a image
         * Sliding and resize window to scrach all the regions
         * return a vector which save the index of face regions
         *
         * /param img  The image need to be detected
         * /param rects  The vector that contain the location of faces
         * /param scores  the vector thar contain the faces score
         */
        std::vector<int> DetectFace(cv::Mat img,std::vector<cv::Rect>& rects, std::vector<float>& scores);
        /*
         * \breif nms Non-maximum suppression
         * the Nms algorithm result concerned score of areas
         *  
         * \param rects     area of faces
         * \param scores    score of faces
         * \param Srect size of rects
         * \param overlap   overlap threshold
         * \param img  get size of origin img
         * \return          picked index
         */
        std::vector<int> Nms(std::vector<cv::Rect>& rects, std::vector<float>& scores, std::vector<int>& Srect, float overlap, cv::Mat img);
        /*
         * \breif function for Partation areas
         * From Predicate mat get a paration result
         * 
         * \param predicate  The matrix marked cross areas
         * \param label  The vector marked classification label
         * return number of classfication
         */
        int Partation(cv::Mat predicate,std::vector<int>& label);
        /*
         * \breif Find classfication area parent
         *
         * \param parent  parent vector
         * \param x  current node
         */
        int Find(std::vector<int>& parent,int x);
        /*
         * \breif Compute score
         * y = log(1+exp(x));
         *
         * \param scores  score vector
         * \param index  score index
         */
        std::vector<float> Logistic(std::vector<float> scores ,std::vector<int> index);
    };
}
#endif /*RASPBERRY_ROBOT_IMAGE_NPD_LEARN_GAB__*/
