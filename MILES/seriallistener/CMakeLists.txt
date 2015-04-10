cmake_minimum_required(VERSION 2.8.3)
project(seriallistener)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=gnu++0x -Os -fPIC")
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  roslib
)
catkin_package(CATKIN_DEPENDS roscpp roslib std_msgs)


###################################
## catkin specific configuration ##

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(seriallistener src/seriallistener.cpp)
TARGET_LINK_LIBRARIES( seriallistener serial ${catkin_LIBRARIES})
