#include <SerialStream.h>
#include <iostream>
#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <ros/package.h>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>

std::string location="X:15 Y:20";
void LocationCallBack(const std_msgs::String::ConstPtr &msg){
	location=msg->data;
}

int main(int argc, char** argv){
using namespace boost ;
	ros::init(argc, argv, "SerialListener");
	ros::NodeHandle n;
	
	ros::Subscriber sub = n.subscribe("location",1000,LocationCallBack);

	std::string pkg = ros::package::getPath("seriallistener");
	
	asio::io_service io;
	asio::serial_port port(io);
	port.open(argv[1]);
	port.set_option(asio::serial_port_base::baud_rate(9600));
	port.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
      	port.set_option(asio::serial_port::parity(asio::serial_port::parity::none));
        port.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
     	port.set_option(asio::serial_port::character_size(8));
	while(1){
		std::string answer="";
		printf("Listening for Requests\n");
		while(1){
			char c;
			printf("Before\n");
			asio::read(port, asio::buffer(&c,1));
			printf("After\n");
			if(c=='\n')
				break;	
			answer=answer+c;
			printf("%s\n",answer.c_str());
		}
		
		printf("Request: %s\n",answer.c_str());

		if(answer.find("LONCE")!=std::string::npos){
			printf("Requested Location Once\n");
			ros::spinOnce();
			printf("AfterLONCE\n");
			asio::write(port,asio::buffer(location.c_str(),location.length()-1));
			//send location from ros topic
		}
		else if(answer.find("LAUTO")!=std::string::npos){
			printf("Requested Location Auto\n");
			//send location from ros topic at 5 Hz
			ros::Rate r(5);
			while (ros::ok())
			{
				ros::spinOnce();
				//listen for a stop message for a sec
				r.sleep();
			}
		}
		else if(answer.find("MD")!=std::string::npos){
			printf("Requested Metadata\n");
			//ex: "MD 37 4"
			//send specific metadata file through port
			//Parse the file name
			int t =answer.find(" ");
			answer=answer.substr(t+1,std::string::npos);
			//printf("%s\n",answer.c_str());
			//37 4
			t=answer.find(" ");
			std::string buildNo=answer.substr(0,t);
			printf("%s\n",buildNo.c_str());
			std::string floorNo=answer.substr(t+1,1);
			printf("%s\n",floorNo.c_str());
	
			std::string filename=pkg+"/metaFiles/B"+buildNo+"-F"+floorNo+"-Meta"+".txt";
			printf("%s\n",filename.c_str());
			FILE* mfile;			
			try{
				mfile=fopen(filename.c_str(),"r");
			}
			catch(int e){
				asio::write(port,asio::buffer("<END>\10",6));		
			}
			char line[255];
			//send data line by line
			while(fgets(line,255,mfile)!=NULL){
			//	asio::write(port,asio::buffer("||", 2));
				std::string send(line);
				asio::write(port,asio::buffer(send.c_str(), send.length()));
				//asio::write(port,asio::buffer("\n",1));
				printf("%s",send.c_str());
				//sleep(5);
			}
			fclose(mfile);
			asio::write(port,asio::buffer("<END>\n",6));
			
		}
		else if(answer.find("MAP")!=std::string::npos){
			printf("Requested Map\n");
			//send map through the port
			std::string filename=pkg+"/Maps/map.pgm";
		}
		else{
			printf("Unknown Request\n");
		}
		
	}
}
