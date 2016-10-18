//
//  Services.hpp
//  rescueRobot
//
//  Created by 黄正跃 on 23/09/2016.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#ifndef Services_hpp
#define Services_hpp

#include "VideoStreamer.hpp"

namespace rr {
    class Services {
    private:
        VideoStreamer *videoStreamer = nullptr;
    public:
        static Services &getInstance() {
            static Services services;
            return services;
        }

        Services() {
            videoStreamer = new VideoStreamer();
        }

        Services(const Services &) = delete;

        Services &operator=(const Services &) = delete;

        void startVedioStreamer();

        void startConnection();

        void startMovementHardware();

        void move(float, float);

    };

}


#endif /* Services_hpp */
