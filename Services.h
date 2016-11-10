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

namespace rr
{
class Services
{
  private:
    Services();

    Services(const Services &) = delete;
    Services &operator=(const Services &) = delete;

    VideoStreamer *videoStreamer = nullptr;
    CarHardware *car = nullptr;
    bool hardwareState;
    bool streamerState;
    bool connectionState;
    bool lightState;

    std::thread *stopThread = nullptr;
    static void stopService(Services *that);

  public:
    static Services &getInstance()
    {
        static Services services;
        return services;
    }

    void startVedioStreamer();

    void stopVedioStreamer();

    void startConnection();

    void stopConnection();

    void startMovementHardware();

    //stop and release hardware resources
    void stopMovementHardware();

    bool hardwareIsStarted();

    bool streamerIsStarted();

    bool connectionIsStarted();

    bool lightIsOn();
    // 直接控制速度,在远端控制时不能使用
    void move(short left_speed, short right_speed);

    void goForwardOneStep();

    void turnLeftOneStep();

    void turnRightOneStep();

    void goBackOneStep();

    void turnLightOn();

    void turnLightOff();

    int getImageProperty(int propId);

    bool setImageProperty(int propId, int value);

    void setDelayTime(long delayTime);
};
}

#endif /* RASPBERRY_ROBOT_SERVICES__*/
