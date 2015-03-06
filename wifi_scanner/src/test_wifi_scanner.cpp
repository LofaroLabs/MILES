#include "wifi_scan.h"
#include <thread>
#include <chrono>
#include <string>
#include "ros/ros.h"
#include <wifi_scanner/wifi_signal_msg.h>
#include <wifi_scanner/trigger.h>
std::string iface ="wlan2";
static std::chrono::time_point<std::chrono::system_clock> start;
static std::chrono::time_point<std::chrono::system_clock> tim;
static std::chrono::duration<double> elapsed;
static int AP=0;
wifi_scanner::wifi_signal_msg msg;//signal message
wifi_scan ws;
int amount=0;

ros:: Publisher signal_pub;

void parse_args(int argc,char* argv[]){
    int i =0;
    while (i< argc){
        std::string t = std::string(argv[i]);
        if( t == "-i"|| t == "--interface"){
            iface = std::string(argv[i+1]);
            i=i+2;

        }
        i=i+1;
    }
};
std::function<scanning_callback> cb = [](access_point &ap){
         if(ap.essid=="MASON-SECURE"){
		if(AP<20){
         		msg.MAC[AP]=ap.mac_address;
         		std::cout<<"        \t mac: "<<ap.mac_address<<std::endl;
                	msg.Sig[AP]=ap.signal_strength;
         		std::cout<<"        \t signal: "<<ap.signal_strength<<std::endl;
         		std::cout<<"        \t essid: "<<ap.essid<<std::endl<<std::endl;
		}
		else{
			//std::cout<<"More than 18 routers too much not keeping this one"<<std::endl;
		 	
         }
         AP++;
	}
    };
void scan(wifi_scanner::trigger trig){

    if(trig.trigString==iface){
            tim=std::chrono::system_clock::now();
	    ROS_INFO("Triggered %s",iface.c_str());
            ws.scan();
	    std::cout<<"APS Found: "<<AP<<std::endl;
	    elapsed = std::chrono::system_clock::now()-tim;
     	    std::cout<<"Time Taken: "<<elapsed.count()<<std::endl;
	    signal_pub.publish(msg);
	    for (int i=0; i<AP&&i<20; i++){
            	msg.MAC[i]="";
		msg.Sig[i]=0;
            }
	    AP=0;
	    amount++;
	    std::cout<<"Scan Num: "<<amount<<std::endl;
    }
};

int main(int argc, char* argv[])
{
	parse_args(argc,argv);
        ros::init(argc, argv, iface);
	if (ws.init(iface, cb)){
          ros::NodeHandle n;
	  signal_pub = n.advertise<wifi_scanner::wifi_signal_msg>("signals", 1000);
	  //ros::Rate loop_rate(10);
    	  ros::Subscriber sub = n.subscribe<wifi_scanner::trigger>("wifitrig",10,scan);
          ros::spin();
       }
       else {
        std::cout<<"Could not open socket on "<<iface<<std::endl;
    }
	
    
    return 0;
}
