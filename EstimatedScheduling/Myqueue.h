#pragma once
#include <iostream>
#include <cstring>
//#include <conio.h>
using namespace std;
#define QSIZE 5
class Myqueue
{
public:
	Myqueue(void);
	~Myqueue(void);
	int current_size ; 
	
	int *a;
	int front;
	int rear;
	void insert(int i);
	int remove();
	int isempty();
	int isfull();
	void print_queue();
	char name[20];
	void set_name(char * iname);
};

