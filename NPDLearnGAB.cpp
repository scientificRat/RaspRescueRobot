//  NPDLearnGAB.cpp
//  rescueRobot
//
//  Created by Wang Han.SCU on 11/11/16.
//  Copyright © 2016 rescueRobot. SCU. All rights reserved.


#include <math.h>
#include <sys/time.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <map>
#include <omp.h>
#include <iostream>

#include "NPDLearnGAB.h"

namespace rr{

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))


    int pWinSize[]={24,29,35,41,50,60,72,86,103,124,149,178,214,257,308,370,444,532,639,767,920,1104,1325,1590,1908,2290,2747,3297,3956};

    GAB::GAB(){
        const Options& opt = Options::GetInstance();
        stages = 0;

        ppNpdTable = cv::Mat(256,256,CV_8UC1);
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                double fea = 0.5;
                if(i > 0 || j > 0) {
                    fea = double(i) / (double(i) + double(j));
                }

                fea = floor(256 * fea);
                
                if(fea > 255) { 
                    fea = 255;
                }

                ppNpdTable.at<uchar>(i,j) = (unsigned char) fea;
            }
        }

        std::size_t numPixels = opt.objSize*opt.objSize;
        
        for(int i = 0; i < numPixels; i++) {
            for(int j = i+1; j < numPixels; j ++) {
                lpoints.push_back(i);
                rpoints.push_back(j);
            }
        } 

        points1.resize(29);
        points2.resize(29);

    }


    void GAB::GetPoints(int feaid, int *x1, int *y1, int *x2, int *y2){
        const Options& opt = Options::GetInstance();
        int lpoint = lpoints[feaid];
        int rpoint = rpoints[feaid];
        //use the model trained by yourself
        /*
        *y1 = lpoint%opt.objSize;
        *x1 = lpoint/opt.objSize;
        *y2 = rpoint%opt.objSize;
        *x2 = rpoint/opt.objSize;
        */
        //use the 1226model

        *y1 = lpoint/opt.objSize;
        *x1 = lpoint%opt.objSize;
        *y2 = rpoint/opt.objSize;
        *x2 = rpoint%opt.objSize;
    }

    void GAB::LoadModel(std::string path){
        FILE* file;
        
        if((file = fopen(path.c_str(), "rb"))==NULL) {
            std::cerr <<"model not find."<<std::endl;
            return;
        }

        fread(&DetectSize,sizeof(int),1,file);
        fread(&stages,sizeof(int),1,file);
        fread(&numBranchNodes,sizeof(int),1,file);
        printf("stages num :%d\n",stages);

        int *_tree = new int[stages];
        float *_threshold = new float[stages];
        fread(_tree,sizeof(int),stages,file);
        fread(_threshold,sizeof(float),stages,file);

        for(int i = 0;i<stages;i++) {
            treeIndex.push_back(_tree[i]);
            thresholds.push_back(_threshold[i]);
        }

        delete []_tree;
        delete []_threshold;

        int *_feaId = new int[numBranchNodes];
        int *_leftChild = new int[numBranchNodes];
        int *_rightChild = new int[numBranchNodes];
        unsigned char* _cutpoint = new unsigned char[2*numBranchNodes];
        fread(_feaId,sizeof(int),numBranchNodes,file);
        fread(_leftChild,sizeof(int),numBranchNodes,file);
        fread(_rightChild,sizeof(int),numBranchNodes,file);
        fread(_cutpoint,sizeof(unsigned char),2*numBranchNodes,file);

        for(int i = 0;i<numBranchNodes;i++){
            feaIds.push_back(_feaId[i]);
            leftChilds.push_back(_leftChild[i]);
            rightChilds.push_back(_rightChild[i]);
            cutpoints.push_back(_cutpoint[2*i]);
            cutpoints.push_back(_cutpoint[2*i+1]);
            for(int j = 0;j<29;j++){
                int x1,y1,x2,y2;
                GetPoints(_feaId[i],&x1,&y1,&x2,&y2);
                float factor = (float)pWinSize[j]/(float)DetectSize;
                int p1x = x1*factor;
                int p1y = y1*factor;
                int p2x = x2*factor;
                int p2y = y2*factor;
                points1[j].push_back(p1y*pWinSize[j]+p1x);
                points2[j].push_back(p2y*pWinSize[j]+p2x);
            }
        }

        delete []_feaId;
        delete []_leftChild;
        delete []_rightChild;
        delete []_cutpoint;

        int numLeafNodes = numBranchNodes+stages;
        float *_fit = new float[numLeafNodes];
        fread(_fit,sizeof(float),numLeafNodes,file);
        for(int i = 0;i<numLeafNodes;i++){
            fits.push_back(_fit[i]);
        }

        delete []_fit;
        fclose(file);
    }

    std::vector<int> GAB::DetectFace(cv::Mat img,std::vector<cv::Rect>& rects,std::vector<float>& scores){
        const Options& opt = Options::GetInstance();

        int minFace = 20;
        int maxFace = 3000;

        omp_set_num_threads(opt.numThreads);

        int height = img.rows;
        int width =  img.cols;
        const unsigned char *O = (unsigned char *) img.data;
        unsigned char *I = new unsigned char[width*height];
        int k =0;

        for(int i = 0;i<width;i++){
            for(int j = 0;j<height;j++){
                I[k]=*(O+j*width+i);
                k++;
            }
        }

        minFace = max(minFace, opt.objSize);
        maxFace = min(maxFace, min(height, width));

        std::vector<int> picked;
        if(min(height, width) < minFace) {
            return picked;
        }

        // process each scale
        for(int k = 0; k < 29; k++) {
            if(pWinSize[k] < minFace) 
                continue;
            else if(pWinSize[k] > maxFace) 
                break;

            // determine the step of the sliding subwindow
            int winStep = (int) floor(pWinSize[k] * 0.1);
            if(pWinSize[k] > 40) winStep = (int) floor(pWinSize[k] * 0.05);

            // calculate the offset values of each pixel in a subwindow
            // pre-determined offset of pixels in a subwindow
            std::vector<int> offset(pWinSize[k] * pWinSize[k]);
            int pp1 = 0, pp2 = 0, gap = height - pWinSize[k];
            
            // column coordinate 
            for(int j=0; j < pWinSize[k]; j++) {
                // row coordinate
                for(int i = 0; i < pWinSize[k]; i++) {
                    offset[pp1++] = pp2++;
                }
                pp2 += gap;
            }
            int colMax = width - pWinSize[k] + 1;
            int rowMax = height - pWinSize[k] + 1;

            // process each subwindow
            #pragma omp parallel for
            // slide in column
            for(int c = 0; c < colMax; c += winStep) {
            const unsigned char *pPixel = I + c * height;
                // slide in row
                for(int r = 0; r < rowMax; r += winStep, pPixel += winStep) {
                    float _score = 0;
                    int s;
                    // test each tree classifier
                    for(s = 0; s < stages; s++) {
                        int node = treeIndex[s];

                        // test the current tree classifier
                        // branch node
                        while(node > -1) {
                            unsigned char p1 = pPixel[offset[points1[k][node]]];
                            unsigned char p2 = pPixel[offset[points2[k][node]]];
                            unsigned char fea = ppNpdTable.at<uchar>(p1,p2);
                            if(fea < cutpoints[2*node] || fea > cutpoints[2*node+1]) 
                                node = leftChilds[node];
                            else 
                                node = rightChilds[node];
                        }

                      node = - node - 1;
                      _score = _score + fits[node];

                    if(_score < thresholds[s]){
                        break; // negative samples
                    }
                }
                
                // a face detected
                if(s == stages) {
                    cv::Rect roi(c, r, pWinSize[k], pWinSize[k]);
                    // modify the record by a single thread
						#pragma omp critical
						{ 
							rects.push_back(roi);
							scores.push_back(_score);
						}
					}
                }
            }
        }

        std::vector<int> Srect;
        picked = Nms(rects,scores,Srect,0.5,img);

        int imgWidth = img.cols;
        int imgHeight = img.rows;


        //you should set the parameter by yourself
        for(std::size_t i = 0;i<picked.size();i++) {
        int idx = picked[i];
        int delta = floor(Srect[idx]*opt.enDelta);
        int y0 = max(rects[idx].y - floor(3.0 * delta),0);
        int y1 = min(rects[idx].y + Srect[idx],imgHeight);
        int x0 = max(rects[idx].x + floor(0.25 * delta),0);
        int x1 = min(rects[idx].x + Srect[idx] - floor(0.25 * delta),imgWidth);

        rects[idx].y = y0;
        rects[idx].x = x0;
        rects[idx].width = x1-x0 + 1;
        rects[idx].height = y1-y0 + 1;
        }

        delete []I;
        return picked;
    }



    std::vector<int> GAB::Nms(std::vector<cv::Rect>& rects, std::vector<float>& scores, std::vector<int>& Srect, float overlap, cv::Mat Img) {
        int numCandidates = rects.size();
        cv::Mat predicate = cv::Mat::eye(numCandidates,numCandidates,IPL_DEPTH_1U);
        for(int i = 0;i<numCandidates;i++) {
            for(int j = i+1;j<numCandidates;j++) {
                int h = min(rects[i].y+rects[i].height,rects[j].y+rects[j].height) - max(rects[i].y,rects[j].y);
                int w = min(rects[i].x+rects[i].width,rects[j].x+rects[j].width) - max(rects[i].x,rects[j].x);
                int s = max(h,0)*max(w,0);

                if ((float)s/(float)(rects[i].width*rects[i].height+rects[j].width*rects[j].height-s)>=overlap){
                    predicate.at<bool>(i,j) = 1;
                    predicate.at<bool>(j,i) = 1;
                }
            }
        }

        std::vector<int> label;

        int numLabels = Partation(predicate,label);

        std::vector<cv::Rect> Rects;
        Srect.resize(numLabels);
        std::vector<int> neighbors;
        neighbors.resize(numLabels);
        std::vector<float> Score;
        Score.resize(numLabels);

        for(int i = 0;i<numLabels;i++){
            std::vector<int> index;
            for(int j = 0;j<numCandidates;j++){
                if(label[j]==i)
                    index.push_back(j);
            }
            std::vector<float> weight;
            weight = Logistic(scores,index);
            float sumScore=0;
            for(std::size_t j=0;j<weight.size();j++)
                sumScore+=weight[j];
            Score[i] = sumScore;
            neighbors[i]=index.size();

            if (sumScore == 0) {
                for(std::size_t j=0;j<weight.size();j++)
                    weight[j] = 1/sumScore;
            }else {
                for(std::size_t j=0;j<weight.size();j++)
                    weight[j] = weight[j]/sumScore;
            }

            float size = 0;
            float col = 0;
            float row = 0;
            for(std::size_t j=0;j<index.size();j++){
                size += rects[index[j]].width*weight[j];
            }

            Srect[i] = (int)floor(size);

            for(std::size_t j=0;j<index.size();j++) {
                col += (rects[index[j]].x + rects[index[j]].width/2)*weight[j];
                row += (rects[index[j]].y + rects[index[j]].width/2)*weight[j];
            }
            int x = floor(col-size/2);
            int y = floor(row-size/2);
            cv::Rect roi(x,y,Srect[i],Srect[i]);
            Rects.push_back(roi);
      }


      predicate = cv::Mat::zeros(numLabels,numLabels,IPL_DEPTH_1U);

      for(int i = 0;i<numLabels;i++){
        for(int j = i+1;j<numLabels;j++){
          int h = min(Rects[i].y+Rects[i].height,Rects[j].y+Rects[j].height) - max(Rects[i].y,Rects[j].y);
          int w = min(Rects[i].x+Rects[i].width,Rects[j].x+Rects[j].width) - max(Rects[i].x,Rects[j].x);
          int s = max(h,0)*max(w,0);

          if((float)s/(float)(Rects[i].width*Rects[i].height)>=overlap || (float)s/(float)(Rects[j].width*Rects[j].height)>=overlap)
          {
            predicate.at<bool>(i,j) = 1;
            predicate.at<bool>(j,i) = 1;
          }
        }
      }

      std::vector<int> flag;
      flag.resize(numLabels);
      for(int i = 0;i<numLabels;i++)
        flag[i]=1;

      for(int i = 0;i<numLabels;i++){
        std::vector<int> index;
        for(int j = 0;j<numLabels;j++){
          if(predicate.at<bool>(j,i)==1)
            index.push_back(j);
        }
        if(index.size()==0)
          continue;

        float s = 0;
        for(int j  = 0;j<index.size();j++){
          if(Score[index[j]]>s)
            s = Score[index[j]];
        }
        if(s>Score[i])
          flag[i]=0;
      }

      std::vector<int> picked;
      for(int i = 0;i<numLabels;i++){
        if(flag[i]){
          picked.push_back(i);
        }
      }

      int height = Img.rows;
      int width = Img.cols;

      for(int i = 0;i<picked.size();i++){
        int idx = picked[i];
        if(Rects[idx].x<0)
          Rects[idx].x = 0;

        if(Rects[idx].y<0)
          Rects[idx].y = 0;

        if(Rects[idx].y+Rects[idx].height>height)
          Rects[idx].height = height-Rects[idx].y;

        if(Rects[idx].x+Rects[idx].width>width)
          Rects[idx].width= width-Rects[idx].x;
      }

      rects = Rects;
      scores = Score;
      return picked;
    }

    std::vector<float> GAB::Logistic(std::vector<float> scores ,std::vector<int> index){
        std::vector<float> Y;
        for(int i = 0;i<index.size();i++){
            float tmp_Y = log(1+exp(scores[index[i]]));
            if(isinf(tmp_Y))
                tmp_Y = scores[index[i]];
            Y.push_back(tmp_Y);
        }
        
        return Y;
    }

    int GAB::Partation(cv::Mat predicate, std::vector<int>& label){
        int N = predicate.cols;
        std::vector<int> parent;
        std::vector<int> rank;

        for(int i=0;i<N;i++) {
            parent.push_back(i);
            rank.push_back(0);
        }

        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                if (predicate.at<bool>(i,j)==0)
                    continue;
                int root_i = Find(parent,i);
                int root_j = Find(parent,j);

                if(root_j != root_i) {
                    if (rank[root_j] < rank[root_i])
                        parent[root_j] = root_i;
                    else if (rank[root_j] > rank[root_i])
                        parent[root_i] = root_j;
                    else{
                        parent[root_j] = root_i;
						rank[root_i] = rank[root_i] + 1;
                    }
                }
            }
        }

        int nGroups = 0;
		
        label.resize(N);
        for(int i=0;i<N;i++) {
            if(parent[i]==i){
                label[i] = nGroups;
                nGroups++;
            }
            else{ 
                label[i] = -1;
            }
        }

        for(int i=0;i<N;i++){
            if(parent[i]==i)
                continue;
            int root_i = Find(parent,i);
            label[i]=label[root_i];
        }

        return nGroups;
    }

    int GAB::Find(std::vector<int>& parent,int x){
        int root = parent[x];
        if(root != x)
            root = Find(parent,root);
        return root;
    }


    cv::Mat GAB::Draw(cv::Mat& img, cv::Rect& rects){
        cv::Mat img_ = img.clone();
        cv::rectangle(img_,rects,cv::Scalar(0, 0, 255), 2);
        return img_;
    }

}