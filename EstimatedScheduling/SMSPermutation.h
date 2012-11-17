#pragma once
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class SMSPermutation
{
public:
	SMSPermutation(void);
	~SMSPermutation(void);
	vector<string> result;
	int numberofvaluse;	
	string values[10];
	string input;
	void permutation(int index);
	void print_result();
	
	 

};

