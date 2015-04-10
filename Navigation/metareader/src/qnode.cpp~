/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/qdude/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qdude {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv )
{
	ros::init(argc,argv,"qdude");
	ros::NodeHandle n;
	Goal_publisher = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);
}

QNode::~QNode() {
}
bool QNode::SetGoal(double x, double y){
	geometry_msgs::PoseStamped msg;
	msg.header.frame_id="map";
	msg.pose.position.x=0;
	msg.pose.position.y=0;
	msg.pose.position.z=0;
	msg.pose.orientation.w=0;
	msg.pose.orientation.x=0;
	msg.pose.orientation.y=0;
	msg.pose.orientation.z=0;
	Goal_publisher.publish(msg);
	
}
}
