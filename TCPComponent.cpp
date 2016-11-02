//
//  TCPComponent.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 25/09/2016.
//  Last Modified by Wang han on 2/11/2016
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include <string>
#include <string.h>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>

#include "TCPComponent.h"
#include "Services.h"
#include "CarHardware.h"

#define DEBUG

namespace rr{

    std::thread* TCPComponent::sendThread = nullptr;

    TCPComponent::TCPComponent():
    recieveThreadRun(true),
    serviceAdrress("123.206.21.185"),
    servicePort(8902),
    loginState(false),
    receiveThread(nullptr),
    sockfd(-1) {
        //create tcp socket
        if(-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))){
            std::cerr<<"socket initial failed\n";
        }
        //set the addr of the local socket
        bzero(&workingAddr, sizeof(sockaddr_in));
        workingAddr.sin_family=AF_INET;
        workingAddr.sin_port=htons(8900);
        workingAddr.sin_addr.s_addr = htonl (INADDR_ANY);
        //bind
        if(-1 == (bind(sockfd, (sockaddr *)&workingAddr, sizeof(workingAddr)))){
            close(sockfd);
            sockfd = -2;
            std::cerr<<"socket bind failed\n";
        }
        //set the address of the remote server
        bzero(&serverAddr, sizeof(sockaddr_in));
        serverAddr.sin_family=AF_INET;
        serverAddr.sin_port=htons(servicePort);
        serverAddr.sin_addr.s_addr = inet_addr(serviceAdrress);
        //connect
        if(-1 == connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr))){
            close(sockfd);
            sockfd = -3;
            std::cerr<<"socket connect failed\n";
        }
    }

    void TCPComponent::login(){
     //create json data
         Json::Value root;
         Json::FastWriter writer;
         root["requestType"] = Json::Value("login");
         root["loginName"] = Json::Value("caesar");
         root["password"] = Json::Value("123456");
         std::string RequestJson = writer.write(root); //include '\n'

         //just for debug
         //std::cout <<"RequestJson :"<<RequestJson<<std::endl;
         //std::cout <<"RequestJson.length : "<<RequestJson.length()<<std::endl;

         // 发送登录请求
         sendRequest(RequestJson.c_str(),RequestJson.length()-1);

         Services& services = Services::getInstance();

         //check login info
         char* headBuffer = new char[5];
         //读取头部信息(type,length) 共5byte
         read(this->sockfd,headBuffer, 5);
         char* dataBuffer = new char[*((int*)(headBuffer+1))];
         read(this->sockfd,dataBuffer,*((int*)(headBuffer+1)));

         //just for debug
         #ifdef DEBUG
         printf("headBuffer : %d %d %d %d %d\n", headBuffer[0],headBuffer[1],headBuffer[2],headBuffer[3],headBuffer[4]);
         std::cout << dataBuffer<<std::endl;
         std::cout <<"In "<<__FILE__<<" at "<<__LINE__<<" line."<<std::endl;
         #endif

         if(headBuffer[0]=='m'){
             std::string status ="";
             std::string ResponseJson = std::string(dataBuffer);
             Json::Reader reader;
             Json::Value root;
             //parse the response json
             if (reader.parse(ResponseJson, root)) {
                 status = root["success"].asString();

                 //just for debug
                 #ifdef DEBUG
                 std::cout <<"status is "<<status<<std::endl;
                 std::cout <<"In "<<__FILE__<<" at "<<__LINE__<<" line."<<std::endl;
                 #endif
             }

             if (status == "true"){
                 this->loginState = true;
                 //login success, then start the receiveThread
                 this->receiveThread = new std::thread(receive, this);
                 this->receiveThread->join();

             }else{

             }
         }

         delete[] headBuffer;
         delete[] dataBuffer;
         std::cerr << "No ResponseJson "<<std::endl;
    }


    //reconnection server
    void TCPComponent::reconnection(){
        while(!this->loginState) {
            //wait 10s, then try again
            sleep(10);
            //try logining again
            login();
        }
    }

    void TCPComponent::receive(TCPComponent *that) {
        Services& services = Services::getInstance();

        while (that->recieveThreadRun){

            char* headBuffer = new char[5];
            //读取头部信息(type,length) 共5byte
            read(that->sockfd,headBuffer, 5);
            char* dataBuffer = new char[*((int*)(headBuffer+1))];
            read(that->sockfd,dataBuffer,*((int*)(headBuffer+1)));
            // (注意：服务器返回的消息type='m' 内容为json, 控制端发送的消息type ='c'表示命令)

            //debug
            #ifndef DEBUG
            printf("headBuffer : %d %d %d %d %d\n", headBuffer[0],headBuffer[1],headBuffer[2],headBuffer[3],headBuffer[4]);
            std::cerr << dataBuffer<<std::endl;
            std::cerr <<"In "<<__FILE__<<" at "<<__LINE__<<" line."<<std::endl;
            #endif

            if(headBuffer[0]=='c'){
                 float left=*((float*)dataBuffer);
                 float right=*((float*)dataBuffer+1);

                 //debug
                 //#ifndef DEBUG
                 std::cerr << "left:"<< left <<"\tright: "<<right<<std::endl;
                 std::cerr <<"In "<<__FILE__<<" at "<<__LINE__<<" line."<<std::endl;
                 //#endif
                 if (services.hardwareIsStarted()){
                     services.move(dataBuffer);
                 }
                 else{
                     services.startMovementHardware();
                     services.move(dataBuffer);
                 }
            }
            else if(headBuffer[0]=='m'){
                 std::string action ="";
                 std::string ResponseJson = std::string(dataBuffer);
                 Json::Reader reader;
                 Json::Value root;
                 if (reader.parse(ResponseJson, root)) {
                     action = root["action"].asString();
                     
                 }

                 if (action == "startVideo") {
                    if (services.streamerISStarted())
                         //start the sendThread
                         std::cerr<<"streamer had been started!"<<std::endl;
                    else{
                         sendThread = new std::thread(&Services::startVedioStreamer,&services);
                         std::cout<<"video streamer started!"<<std::endl;
                    }
                }else if(action == "stopVideo") {
                    services.stopVedioStreamer();
                    std::cout<<"video streamer stoped!"<<std::endl;
                }

                //just for deubg
                 #ifdef DEBUG
                 std::cout<<"ResponseJson: "<<ResponseJson<<std::endl;
                 std::cout <<"In "<<__FILE__<<" , at "<<__LINE__<<" line."<<std::endl;
                 #endif
            }
            else {
                 std::cout << "ResponseJson error" << std::endl;
            }

            delete[] headBuffer;
            delete[] dataBuffer;
        }//end of while

    }

    void TCPComponent::sendMessage(const void *data, int length) {
         this->sendMutex.lock();
         int writeState = -1;
         int closeState = -1;

         if(this->sockfd <0 || data == nullptr || length < 0) {
            std::cerr <<"write params error."<<std::endl;
            return;
         }

         writeState = write(this->sockfd,data,length);
         //deal with write error
         if(writeState<=0)
         {
            this->loginState = false;

            if(errno == EINTR) {

                std::cerr <<"server socket write error."<<std::endl;
                //deal with close error
                closeState = close(this->sockfd);

                if (closeState < 0){
                     if(errno == EBADF) {
                         std::cerr<<"fd is not a valid socket."<<std::endl;
                     }else if(errno == EIO) {
                         std::cerr<<"IO error."<<std::endl;
                     }else if (errno == EINTR) {
                         std::cerr <<"socket is shutdown by interrupt signal."<<std::endl;
                    }
                }
                 reconnection();
            }
            else if(errno == EPIPE) {
                std::cerr <<"server socket had been closed. And try to connect again after 10 seconds"<<std::endl;
                reconnection();
            }
         }
         this->sendMutex.unlock();
    }//end of sendMessage

    void TCPComponent::sendRequest(const char* JSONBytes, int length){
         char *sendBuffer = new char[5+length];
         sendBuffer[0] = 'r';
         char* ptr = (char*)&length;
         sendBuffer[1] = *ptr;
         sendBuffer[2] = *(ptr+1);
         sendBuffer[3] = *(ptr+2);
         sendBuffer[4] = *(ptr+3);

         for(int i=5;i<5+length;i++){
             sendBuffer[i] = JSONBytes[i-5];
         }
         sendMessage(sendBuffer,5+length);

         delete[] sendBuffer;
    }//end of sendRequest

    /*
    static void TCPComponent::setServerAddress(char* address){
        serviceAdrress = std::string(address);
        std::cout<<"new server address: "<<address<<std::endl;
    }
    */

}
