//
//  main.cpp
//  rescueRobot
//
//  Created by 黄正跃 on 9/15/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include <iostream>
<<<<<<< HEAD
#include "Services.h"
int main(int argc, const char * argv[]) {
     rr::Services& services = rr::Services::getInstance();
     //services.startVedioStreamer();
     
     services.startConnection();
    
     return 0;
=======
#include "Services.hpp"

int main(int argc, const char *argv[]) {
    rr::Services &services = rr::Services::getInstance();
    //services.startVedioStreamer();

    services.startConnection();

    return 0;
>>>>>>> origin/master
}
