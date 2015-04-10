
#include <NodeList.h>
#include <Node.h>

//Normalize all the nodes in the nodelist
void NodeList::NormalizeAll(){
	for(int x=0;x<this->nodeCount;x++){
		this->list[x].Normalize();
	}
}

//Sort the MACs of all the nodes in the nodelist
void NodeList::SortAll(bool up){
	for(int x=0;x<this->nodeCount;x++){
		this->list[x].Sort(up);
	}
}
//Standard toString() for debugging. Prints out all nodes
std::string NodeList::toString(){
	std::string ret="";
	for(int x=0;x<this->nodeCount;x++){
		ret+="\n"+std::to_string(x)+this->list[x].toString();
	}
	return ret;
}
//Standard toString() for debugging. Prints a single node
std::string NodeList::toString(int x){
	return this->list[x].toString();
}
//Standard toString() for debugging. Prints node section from begin to end
std::string NodeList::toString(int begin, int end){
	std::string ret;
	if(begin>end){
		if(begin>this->nodeCount)
			begin=this->nodeCount;
		for(int x=end;x<begin;x++)
			ret+=this->list[x].toString();
	}	
	else{
		if(end>this->nodeCount)
			end=this->nodeCount;
		for(int x=begin;x<end;x++)
			ret+=this->list[x].toString();
	}
	return ret;
}
//Prints out a single router from all nodes. Show the change in strength over change in position
std::string NodeList::RouterInfo(std::string MAC){
	std::string ret=MAC+" info:\n";
	for(int x=0;x<this->nodeCount;x++){
		int y=0;
		while(this->list[x].routers[y].Strength!=0){
			if(this->list[x].routers[y].MAC==MAC){
				ret+="\n"+this->list[x].pos.toString()+" Strength: "+this->list[x].routers[y].toString();
				break;
			}
			y++;	
		}
		
	}
	return ret;
}

//Load the NodeList from file.
//The files needed will be the wifi file created from the wifi listener, 
//and the trajectory file created by rgbdslam
void NodeList::loadFromFile(std::string wifiFILE,std::string trajFILE)
{
	//Read from both the trajectory file from rgbdslam and the wifi file from wifi scanner
	FILE* wfile=fopen(wifiFILE.c_str(),"r");

	FILE* tfile=fopen(trajFILE.c_str(),"r");
	
	char line[10000];
	this->nodeCount =0;
	//Go through the wifi file line by line
	while(fgets(line,1000,wfile)!=NULL){
		std::string lineString = line;
		int front=0;

		//get the time stamp from the wifi file
		int back = lineString.find("|");
		std::string time = lineString.substr(0,back);
		
		printf("time %s\n",time.c_str());
		double wtime =atof(time.c_str());
		front=back;
	
		std::string xstring;
		std::string ystring;
		std::string zstring;
		//Go through the Trajectory file line by line
		while(fgets(line,1000,tfile)!=NULL){
			//Find the time that the trajectory was posted 
			//and compare with the wifi time
			std::string tstring = line;
			int b = tstring.find(" ");
			std::string t= tstring.substr(0,b);
			double ptime = atof(t.c_str());
 			
			//if the times are close, get the xyz data
			if(ptime-wtime<=.5&&ptime-wtime>=-.5){
				tstring= tstring.substr(b+1,-1);
				int f=0;
				printf("time found %f\n",ptime);
				
				b=tstring.find(" ");
				printf("f %d,b %d | %s",f,b,tstring.c_str());
				xstring = tstring.substr(f,b);
				printf("x %s\n",xstring.c_str());
				
				tstring= tstring.substr(b+1,-1);
				b=tstring.find(" ");
				ystring = tstring.substr(f,b);
				printf("y %s\n",ystring.c_str());
				tstring= tstring.substr(b+1,-1);
				
				b=tstring.find(" ")-f;
				zstring = tstring.substr(f,b);
				printf("z %s\n",zstring.c_str());
				break;
			}
		}
		
		//convert to double and place in the position of the node
		this->list[this->nodeCount].pos.x= atof(xstring.c_str());
		this->list[this->nodeCount].pos.y= atof(ystring.c_str());
		this->list[this->nodeCount].pos.z= atof(zstring.c_str());

		lineString=lineString.substr(back,-1);
		std::string MAC;
		std::string Str;
		int routercount = 0;
		
		while(lineString.find("MAC")!=std::string::npos){

			front=lineString.find("[MAC: ");
			back=lineString.find(",");
			MAC = lineString.substr(front+6,back-front-6);
			//std::cout <<"MAC "<<MAC<< std::endl;

			front=lineString.find("Str ");
			back=lineString.find("]");
			Str = lineString.substr(front+4,back-front-4);
			//std::cout <<"Str " <<Str<< std::endl;
			double num= std::stod(Str.c_str());
			lineString=lineString.substr(back+1,-1);
			//std::cout <<lineString<< std::endl;
			//convert from string and put in the router array
			this->list[this->nodeCount].routers[routercount].MAC=MAC;
			this->list[this->nodeCount].routers[routercount].Strength=num;
			routercount++;
		}
		this->nodeCount++;
	}
        fclose(wfile);
	fclose(tfile);
}

void NodeList::toFile(std::string filename){
	FILE* outfile=fopen(filename.c_str(),"w");
	fprintf(outfile,"%s\n",this->toString().c_str());
	fclose(outfile);

}
