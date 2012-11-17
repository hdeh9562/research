#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include "NodeMachine.h"

using namespace std;
class Heap
{
public:
	Heap(void);
	~Heap(void);
	void insert(NodeMachine element);
	NodeMachine deletemin();
	 void print();
	  int size() { return heap.size(); }

	  private:
     int left(int parent);
    int right(int parent);
    int parent(int child);
    void heapifyup(int index);
    void heapifydown(int index);
private:
    vector<NodeMachine> heap;

};

