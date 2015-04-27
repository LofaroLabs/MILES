/*
 * Node.h
 *
 *  Created on: 2015-02-13
 *      Author: rtindell
 */
#ifndef NODELIST_H
#define NODELIST_H

#include <Node.h>
#include <string>
#include <vector>

class NodeList{
 public:
	void NormalizeAll();
	void SortAll(bool up);
	std::string toString();
	std::string toString(int x);
	std::string toString(int begin, int end);
	std::string RouterInfo(std::string MAC);
	Node list[2000];
	int nodeCount;
	void loadFromFile(std::string wifiFILE);
	void toFile(std::string filename);
	void Subtract(Node *curReading);
	void Add();
	std::vector<int> findMins();
	void FindNN(NodeList *plist, int index);
       
};
#endif
