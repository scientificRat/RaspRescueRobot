cmake_minimum_required (VERSION 2.8)
project (rescueRobot)
set(raspicam_DIR /home/pi/raspicam-0.1.3/build)
find_package(raspicam REQUIRED)
find_package(OpenCV)
FIND_PACKAGE(OpenMP)
  if(OPENMP_FOUND)
     message("OPENMP FOUND")
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
include_directories(SYSTEM ${OpenMP_INCLUDE_PATH})
add_definitions(-std=c++11)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
MESSAGE("OpenCV FOUND")
MESSAGE("RASPICAM_CV FOUND")
MESSAGE(STATUS "COMPILING RASPIVISION")
INCLUDE_DIRECTORIES()
SET(SRC_LIST main.cpp
        ImageCaptureUnit.cpp
        ImageProcessUnit.cpp
        Services.cpp      
        TCPComponent.cpp
        VideoStreamer.cpp
		dist/jsoncpp.cpp
        )

ADD_EXECUTABLE(raspivision ${SRC_LIST})
INCLUDE_DIRECTORIES(${raspicam_CV_INCLUDE_DIRS})
target_link_libraries (raspivision pthread)
target_link_libraries (raspivision wiringPi)
target_link_libraries (raspivision jsoncpp)
target_link_libraries (raspivision ${raspicam_CV_LIBS})
ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()
