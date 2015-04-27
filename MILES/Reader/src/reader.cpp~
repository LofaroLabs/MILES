#include <NodeList.h>
#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h> 
#include <wifi_scanner/wifi_signal_msg.h>
#include <ros/ros.h>

#include <std_msgs/String.h>
//#include <visualization_msgs/Marker.h>



ros::Publisher location_pub;
ros::Publisher marker_pub;
NodeList mlist;	

void localize(wifi_scanner::wifi_signal_msg msg){
	//turn wifi reading into a node
	//Find closest matches to the new node from the NodeList
	Node curReading;
	int x=0;
	curReading.routerCount=0;
	curReading.pos.x=0;
	curReading.pos.y=0;	
	curReading.pos.z=0;
	curReading.prob=0;
	while(msg.MAC[x]!=""){
		curReading.routers[x].MAC=msg.MAC[x].c_str();
		curReading.routers[x].Strength=msg.Sig[x];
		//std::cout<<msg.MAC[x].c_str()<<"\n"<<msg.Sig[x]<<"\n";
		//std::cout<<curReading.routers[x].MAC.c_str()<<"\n"<<curReading.routers[x].Strength<<"\n";
		x++;
		curReading.routerCount++;
		
	}
	curReading.Sort(true);
	curReading.Normalize();
	//std::cout<<"\n\n"<<curReading.toString().c_str()<<std::endl;
	//std::cout<<"\n\n"<<list.list[0].toString().c_str()<<std::endl;
	mlist.Subtract(&curReading);
	mlist.Add();
	std::vector<int> mins = mlist.findMins();
	std::cout<< mins.size()<<std::endl;
	//Here we add the probability
	NodeList plist;
	if (mins.size()<100){
		for(int y=0;y<mins.size();y++)
			mlist.FindNN(&plist,mins[y]);
		mins = mlist.findMins();
	}
	std::cout<< mins.size()<<std::endl;
	

	//std::cout<<"\n\n"<<curReading.toString().c_str()<<std::endl;
	//if(mins.size()<10){
	/*	for(int y=0;y<100;y++){
			visualization_msgs::Marker marker;
			if(y<mins.size()){
			marker.pose.position.x = mlist.list[mins[y]].pos.x;
			marker.pose.position.y = mlist.list[mins[y]].pos.y;
			}
			marker.header.frame_id = "map";
			marker.header.stamp = ros::Time::now();
			marker.ns = "Mins";
			marker.id = y;
			marker.scale.x = 1;
			marker.scale.y = 1;
			marker.scale.z = 1;

			marker.color.r = 0.0f;
			marker.color.g = 1.0f;
			marker.color.b = 0.0f;
			marker.color.a =1;
			marker_pub.publish(marker);
			//std::cout<<list.list[mins[y]].toString().c_str()<<std::endl;
		}
	//}*/
	
	Node send=mlist.list[mins[1]];
	std_msgs::String msg2;
	msg2.data = send.pos.toString();
	location_pub.publish(msg2);
	ROS_INFO("%s",msg2.data.c_str());
	
};
int main(int argc, char** argv){

	ros::init(argc, argv, "localizer");
	ros::NodeHandle n;
	mlist.loadFromFile("wifi.txt");
	mlist.SortAll(true);
	
	std::cout<< "All are sorted"<<std::endl;
	mlist.NormalizeAll();
	//std::cout<< list.list[0].toString();
	//std::cout<< list.toString();
	//std::cout<< list.RouterInfo("08:CC:68:DA:5C:11");
	
	location_pub = n.advertise<std_msgs::String>("location",1000);
	//marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 2000);

	ros::Subscriber sub = n.subscribe<wifi_scanner::wifi_signal_msg>("signals",10,localize);
        ros::spin();
}


