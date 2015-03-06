/*
 * Node.h
 *
 *  Created on: 2015-02-13
 *      Author: rtindell
 */
#ifndef NODE_H
#define NODE_H


#include <string>
#include <Pose.h>
#include <Router.h>


class Node{
    public:
	void Normalize(); //Will normalize the node
	void Sort(bool up); //Sorts the MAC addresses by signal strength, maybe should be by name
	std::string toString(); //For printing
	router routers[20]; //The router MAC and signal strength values
	pose pos;  //The position of this node with respect to the map
	double prob; //The probability that we are at this Node

};

#endif
