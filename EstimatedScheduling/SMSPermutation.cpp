#include "StdAfx.h"
#include "SMSPermutation.h"


SMSPermutation::SMSPermutation(void)
{
	//char key[][6]={"", "vtfrq", "ftk", "wzbg", "rs", "", "fir", "p", "io", "p"};
	string pattern[] = {"","vtfrq","ftk","wzbg","rs","","fir","p","io","p"};
	//this->values= new char*[10];
	for(int i=0;i<10;i++)
	{
		this->values[i]=pattern[i];		
	}
	cout<<"pleas enter the toFind string\n";
	//this->input =new char[100];
	cin>>this->input;
	numberofvaluse = this->input.length();

}


SMSPermutation::~SMSPermutation(void)
{
}
void SMSPermutation::print_result()
{
	vector<string>::iterator it;
	it=this->result.begin();
	while(it!=result.end())
	{
		cout<<*it<<endl;
		it++;
	}
}
void SMSPermutation::permutation(int index)
{
	if(index<0)
	{
		return;
	}
	
	char inputchar = this->input[index];
	int input_user = inputchar -'0';

	if(index==0)
	{
		string newstring = this->values[input_user];
		char ch;// =new char[1];
		int j=0;
		ch = this->values[input_user][j++];
		while(ch !='\0')
		{
			string s(&ch);
			this->result.push_back(s);
			ch = this->values[input_user][j++];
		}
	}
	permutation(index-1);	
	int sizeofrsult = this->result.size();
	int k=0;
	while(k<sizeofrsult )
	{
		string s = this->result.at(k);
		string newstring = this->values[input_user];
		char ch ;//=new char[1];
		int j=0;
		ch = this->values[input_user][j++];
		while(ch !='\0')
		{
			s += ch;
			this->result.push_back(s);
			ch = this->values[input_user][j++];
		}
		k++; 
	}

	k=0;
	vector<string>::iterator it;
	it=this->result.begin();
	while(k<sizeofrsult )
	{
		this->result.erase(it);
		k++;
	}
	 
}