#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <wifi_scanner/wifi_signal_msg.h>
#include <visualization_msgs/Marker.h>

tf::TransformListener *listener;
std::string fileName= "wifi.txt";
ros::Publisher marker_pub;

void writeToFile(wifi_scanner::wifi_signal_msg msg){
                FILE* outfile=fopen("wifi.txt","a");
		visualization_msgs::Marker marker;
		std::string markText="";		
		
		try{
			
			tf::StampedTransform transform;
			printf("Scanner message found, writing to file\n");
  			//listener->lookupTransform("/odom", "/base_link", ros::Time(0), transform);
			listener->lookupTransform("/map", "/base_link", ros::Time(0), transform);
			fprintf(outfile,"x:%f||y:%f||z:%f||",
				transform.getOrigin().x(),
				transform.getOrigin().y(),
				transform.getOrigin().z());
			fprintf(outfile,"%f||",ros::Time::now().toSec());

			int x=0;
			printf("1\n");
			while(msg.MAC[x]!=""&&x<99){
                            fprintf(outfile,"[MAC: %s, Str %d]",msg.MAC[x].c_str(),msg.Sig[x]);		//printf("2\n");
	    		    if(x<10)
			    	markText+="[MAC: "+msg.MAC[x]+", Str: "+std::to_string(msg.Sig[x])+"]\n";
			    x++;
			}
			printf("%d routers were found!\n",x);
			fprintf(outfile,"\n");
			marker.header.frame_id = "/base_link";
			marker.header.stamp = ros::Time::now();

			marker.ns = "point";
			marker.id = 0;

			//marker.pose.position.x = transform.getOrigin().x();
			//marker.pose.position.y = transform.getOrigin().y();
			//marker.pose.position.z = transform.getOrigin().z();

			marker.scale.x = 0.1;
			marker.scale.y = 0.1;
			marker.scale.z = 0.1;

			marker.color.r = 0.0f;
			marker.color.g = 1.0f;
			marker.color.b = 0.0f;
			marker.color.a =1;

			marker.lifetime = ros::Duration();

			//marker_pub.publish(marker);
			
			marker.header.stamp = ros::Time::now();
			marker.ns = "text";
			marker.id = 1;
			marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
			marker.text=markText;

			marker.pose.position.x = transform.getOrigin().x()+0.5;
			marker.pose.position.y = transform.getOrigin().y()-0.5;

			marker.scale.z = 0.5;	

			marker_pub.publish(marker);


   		}
		catch (tf::TransformException ex){
    			ROS_ERROR("%s",ex.what());
      			ros::Duration(1.0).sleep();
   		}
 		fclose(outfile);

};
int main(int argc, char** argv){

	ros::init(argc, argv, "my_tf_listener");
	ros::NodeHandle node;
	listener = new tf::TransformListener();
	FILE* outfile=fopen("wifi.txt","w");
         
        fclose(outfile);
	marker_pub = node.advertise<visualization_msgs::Marker>("visualization_marker", 1);

        ros::Subscriber sub = node.subscribe<wifi_scanner::wifi_signal_msg>("signals",10,writeToFile);
        ros::spin();
}


