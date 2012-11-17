#include "StdAfx.h"
#include "NodeMachine.h"


NodeMachine::NodeMachine(void)
{
}

NodeMachine::NodeMachine(int input_id,double input_time){
	this->machine_id = input_id;
	this->release_time = input_time;
	}


NodeMachine::~NodeMachine(void)
{
}
