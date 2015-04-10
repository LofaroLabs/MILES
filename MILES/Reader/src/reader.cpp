#include <NodeList.h>
#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h> 
#include <wifi_scanner/wifi_signal_msg.h>
#include <ros/ros.h>

#include "libusb-1.0/libusb.h"

void printdev(libusb_device *dev);

void test(wifi_scanner::wifi_signal_msg msg){
	//turn wifi reading into a node
	//Find closest matches to the new node from the NodeList
	Node curReading;
	int x=0;
	while(msg.Sig[x]!=0){
		curReading.routers[x].MAC=msg.MAC[x].c_str();
		curReading.routers[x].Strength=msg.Sig[x];
		x++;
	}
	curReading.Sort(true);
	curReading.Normalize();
	std::cout<<"\n\n"<<curReading.toString().c_str()<<std::endl;
	
	
};
int main(int argc, char** argv){
	libusb_device **devs;
	libusb_context *ctx = NULL;

	ros::init(argc, argv, "wifi_listener");
	ros::NodeHandle n;
	NodeList list;	
	list.loadFromFile("wifi.txt","trajectory.txt");
	list.SortAll(true);
	std::cout<< list.toString();
	std::cout<< "All are sorted"<<std::endl;
	list.NormalizeAll();
	std::cout<< list.toString();
	//std::cout<< list.RouterInfo("08:CC:68:DA:5C:11");
	ros::Subscriber sub = n.subscribe<wifi_scanner::wifi_signal_msg>("signals",10,test);
        ros::spin();
}


