#include <SerialStream.h>
#include <iostream>
#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <ros/package.h>
std::string location="";
void LocationCallBack(const std_msgs::String::ConstPtr &msg){
	location=msg->data;
}

int main(int argc, char** argv){
using namespace LibSerial ;
	ros::init(argc, argv, "SerialListener");
	ros::NodeHandle n;
	
	ros::Subscriber sub = n.subscribe("location",1000,LocationCallBack);
	
	SerialStream serial_port ;
	serial_port.Open(argv[1]);
	serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
	serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	serial_port.SetNumOfStopBits(1) ;
	serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
	serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
	serial_port.SetVTime(15);
 	serial_port.SetVMin(10);
	
	if ( ! serial_port.good() )
	{
		printf("Serial Port did not open\n");
		return 0;
	}
	std::string pkg = ros::package::getPath("seriallistener");

	while(1){
		printf("Listening for Requests\n");
		const int BUFFER_SIZE = 512 ;
		char input_buffer[BUFFER_SIZE] ; 

		serial_port.read( input_buffer, BUFFER_SIZE ) ;
		std::string answer(input_buffer);
		printf("Request: %s\n",answer.c_str());

		if(answer.find("LONCE")!=std::string::npos){
			printf("Requested Location Once\n");
			ros::spinOnce();
			serial_port << location.c_str();
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
			answer=answer.substr(t,std::string::npos);
			//37 4
			t=answer.find(" ");
			std::string buildNo=answer.substr(0,t);
			std::string floorNo=answer.substr(t,std::string::npos);

			std::string filename=pkg+"/metaFiles/B"+buildNo+"-F"+floorNo+"-Meta"+".txt";
			printf("%s\n",filename.c_str());			
			
			FILE* mfile=fopen(filename.c_str(),"r");
			char line[10000];
			//send data line by line
			while(fgets(line,1000,mfile)!=NULL){
				serial_port << line;
			}
			serial_port << "<END>";
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
