#pragma once
class NodeMachine
{
public:
	NodeMachine(void);
	NodeMachine(int,double);
	~NodeMachine(void);
	int machine_id;
	double release_time;//show when it finishes its assigned job
};

/*
class Student {
public:
	char* chName;
	int nAge;
	Student(): chName(""),nAge(0){}
	Student( char* chNewName, int nNewAge ):chName(chNewName), nAge(nNewAge){}
};

*/