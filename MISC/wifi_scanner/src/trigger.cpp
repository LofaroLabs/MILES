
#include <thread>
#include <chrono>
#include <string>
#include "ros/ros.h"
#include <wifi_scanner/trigger.h>

double start;
double tim;
double avg=1.5;

wifi_scanner::trigger msg;//signal message
std::string interfaces[8];
int x=0;

void parse_args(int argc,char* argv[]){
    int i =1;
    
    while (i< argc){
	printf("%d\n",i);
 	interfaces[x] = std::string(argv[i]);
        i++;
	x++;
 
    }
};

int main(int argc, char* argv[])
{
	parse_args(argc,argv);
        ros::init(argc, argv, "Trigger_Node");
        ros::NodeHandle n;
	ros:: Publisher signal_pub = n.advertise<wifi_scanner::trigger>("wifitrig", 1000);
	
	
	avg=avg/x;
	avg=1/avg;
	std::cout<<avg<<std::endl;
	ros::Rate loop_rate(avg);

    int trigNum=0;
        while (ros::ok()){
            msg.trigNum=trigNum;
	    msg.trigString=interfaces[trigNum];
	    signal_pub.publish(msg);
	    ROS_INFO("Triggered %s",msg.trigString.c_str());
	    trigNum++;
            if(trigNum==x){
		trigNum=0;
		//ROS_INFO("Triggered All");
	    }
	    ros::spinOnce();
	    loop_rate.sleep();
        }
    
    return 0;
}
