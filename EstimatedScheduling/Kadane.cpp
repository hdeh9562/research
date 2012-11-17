#include "StdAfx.h"
#include "Kadane.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;



Kadane::Kadane(void)
{
	int givenumElements;
	cout<<"enter numElements";
	cin>>givenumElements;

	this->numElements = givenumElements>0?givenumElements:0;
	if(numElements >0)
	{
		this->a= new double[this->numElements];
		for(int i=0;i<this->numElements;i++)
		{
			this->a[i]= rand() % 100 -50;
			cout<<this->a[i]<<endl;
		}
	}

}

double	Kadane::maxSubarray()
{
	double max=0;
	double here=0;
	int i;

	for(i = 0; i < numElements; i++)
	{
		here = (here + a[i] > 0) ? here + a[i] : 0;
		max = (here > max) ? here : max;
		cout<<"iteration\t"<<i<<endl;
		cout<<"here\t"<<here<<endl;
		cout<<"max\t"<<max<<endl;
	}
	return max;
}


Kadane::~Kadane(void)
{
}
