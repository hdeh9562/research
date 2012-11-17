#include "StdAfx.h"
#include "Myqueue.h"


Myqueue::Myqueue(void)
{
	a = new int[QSIZE];
	current_size = QSIZE;
	front=0;
	rear=0;
}


Myqueue::~Myqueue(void)
{
	delete []a;
}
void Myqueue::insert(int i)
{
	if(isfull())
	{
		int * b= new int[current_size+QSIZE];
		for(int index = 0 ; index<current_size;index++)
		{
			b[index]=a[index];
		}
		delete []a;
		a=b;
		current_size=current_size+QSIZE;
		//cout<<"******Queue is FULL !!!we increase the size.******";		
	}
	a[rear] = i;
	rear++;
}
int Myqueue::remove()
{
	if(isempty())
	{
		front=0;
		rear=0;
		//cout<<"******Queue Empty !!!Value returned will be garbage.******";
		return (-9999999);
	}
	return(a[front++]);
}

int Myqueue::isempty()
{
	if(front == rear)
		return 1;
	else
		return 0;
}

int Myqueue::isfull()
{
	if(rear == current_size)
		return 1;
	else
		return 0;
}

void Myqueue::print_queue()
{
	if(!isempty())
	{
		cout<<this->name;
		cout<<"\n"<<"Quese size is\t"<<rear-front<<endl;
		for(int i =0;i<current_size;i++)
		{
			cout<<a[i]<<"\t";
		}
		cout<<endl;
	}
}

void Myqueue::set_name(char * iname)
{
	strcpy(name,iname);

}