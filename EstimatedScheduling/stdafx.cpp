// stdafx.cpp : source file that includes just the standard includes
// EstimatedScheduling.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "NodeMachine.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
/*
double Mytrunc(double input, int precesion)
{
	int p = pow (10.0,precesion);
	double b = floor  (input * p) / p;
	return b;
}*/

bool operator< (const NodeMachine& NodeMachine1, const NodeMachine &NodeMachine2)
{
	return NodeMachine1.release_time < NodeMachine2.release_time ;	
		
}
bool sortingVector(const NodeJobLength& key1, const NodeJobLength& key2)
	{
			return key1.joblength < key2.joblength;
	}
