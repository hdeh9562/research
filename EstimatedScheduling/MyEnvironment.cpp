#include "StdAfx.h"
#include "MyEnvironment.h"



MyEnvironment::MyEnvironment(void)
	: machine_numebrs(0)
{
}


MyEnvironment::~MyEnvironment(void)
{
}


bool MyEnvironment::set_inputs(int number_machines, int number_tasks, int number_tools)
{
	if(number_machines>0)
	{
		this->machine_numebrs = number_machines;
	}
	else 
		return false;
	if(number_tasks>0)
	{
		this->task_numebrs = number_tasks;
	}
	else 
		return false;

	if(number_tools>0)
	{
		this->tools_numbers = number_tools;
	}
	else 
		return false;
	return true;
}
bool MyEnvironment::set_file_names(string cost_input_file_name, string speed_input_file_name, string task_length_input_file_name)
{
	if(!cost_input_file_name.empty())
	{
		this->cost_input_file = cost_input_file_name;
	}
	else
		return false;

	if(!speed_input_file_name.empty())
	{
		this->speed_input_file= speed_input_file_name;
	}
	else
		return false;

	if(!task_length_input_file_name.empty())
	{
		this->task_length_input_file= task_length_input_file_name;
	}
	else
		return false;

	return true;
}

bool MyEnvironment::read_input_files(void)
{
	if(!this->read_cost_machine_from_file())
	{
		cout<<"problem in read_cost_machine_from_file ";
		return false;
	}
	if(!this->read_speed_machine_from_file())
	{
		cout<<"problem in read_speed_machine_from_file ";
		return false;
	}
	if(!this->read_length_of_task_from_file())
	{
		cout<<"problem in read_length_of_task_from_file ";
		return false;
	}
	return 	true;	
}


bool MyEnvironment::read_cost_machine_from_file(void)
{
	ifstream my_input_file_cost_machine;
	int counter=0;
	try {
		my_input_file_cost_machine.open(this->cost_input_file);
		//Read file line by line
		double next_cost;

		while(!my_input_file_cost_machine.eof() && counter<this->machine_numebrs )
		{
			my_input_file_cost_machine >> next_cost;
			if(next_cost>0)
			{
				this->cost_cloud.push_back(next_cost);
				counter++;
			}
		}
	}
	catch (ifstream::failure e) {
		cout << "Exception opening/reading file";
		return false;
	}
	my_input_file_cost_machine.close();
	return true;
}


bool MyEnvironment::read_speed_machine_from_file(void)
{
	ifstream my_input_file_speed_machine;
	int counter=0;
	try {
		my_input_file_speed_machine.open(this->speed_input_file);
		//Read file line by line
		double next_item;

		while(!my_input_file_speed_machine.eof() && counter < this->machine_numebrs )
		{
			my_input_file_speed_machine >> next_item;
			if(next_item>0)
			{
				this->speed_cloud.push_back(next_item);
				counter++;
			}
		}
	}
	catch (ifstream::failure e) {
		cout << "Exception opening/reading file";
		return false;
	}
	my_input_file_speed_machine.close();
	return true;
}



bool MyEnvironment::read_length_of_task_from_file(void)
{
	ifstream my_input_file;
	int counter=0;
	try {
		while(counter < this->task_numebrs)
		{
			my_input_file.open(this->task_length_input_file);
			//Read file line by line
			double next_item;
			while(!my_input_file.eof() && counter < this->task_numebrs )
			{
				my_input_file >> next_item;
				if(next_item>0)
				{
					this->actual_value_running_time.push_back(next_item);
					counter++;
				}
			}
			my_input_file.close();
		}
	}
	catch (ifstream::failure e) {
		cout << "Exception opening/reading file";
		return false;
	}

	return true;
}


void MyEnvironment::print_environemnt()
{
	cout<<"MyEnvironment Class information \n";
	cout<<"number of machiens:\t"<<this->machine_numebrs<<endl;
	cout<<"number of tasks:\t"<<this->task_numebrs<<endl;
	cout<<"number of tools :\t"<<this->tools_numbers<<endl;
	cout<<"the cost of machiens are\n";

	for(vector<double>::iterator it = this->cost_cloud.begin(); it != this->cost_cloud.end(); ++it) 	
	{
		cout<<*it<<"\t";
	}
	cout<<endl;
	cout<<"the speed of machiens are\n";

	for(vector<double>::iterator it = speed_cloud.begin(); it != speed_cloud.end(); ++it) 	
	{
		cout<<*it<<"\t";
	}
	cout<<endl;

	cout<<"the actual length and sum_length of tasks is\n";
	double sum_length= 0;
	for(vector<double>::iterator it = this->actual_value_running_time.begin(); it != actual_value_running_time.end(); ++it) 	
	{
		sum_length+=*it;
		//cout<<*it<<"\t";
	}
	cout<<sum_length<<endl;

	vector<int>::iterator intit;
	intit = sorted_machine_id_efficeincy.begin();
	iprint_vector(intit,sorted_machine_id_efficeincy.size(),"ARG sorted machine id_efficeincy SORTED");

}

void MyEnvironment::iprint_vector(vector<int>::iterator it,int length,string name)
{

	cout<<"items in array called \t\""<<name<<"\"\t are as\n";

	for(int i=0  ; i<length ; i++) 
	{
		cout<<*it<<"\t";
		it++;

	}
	cout<<endl;
}

void MyEnvironment::print_vector(vector<double>::iterator it,int length,string name)
{

	cout<<"items in array called \t\""<<name<<"\"\t are as\n";

	for(int i=0  ; i<length ; i++) 
	{
		cout<<*it<<"\t";
		it++;

	}
	cout<<endl;
}

void MyEnvironment::find_most_eff_machines()
{
	int id = 0;
	//vector<double> most_cost_eff;
	vector<double>::iterator it;
	vector<int>::iterator intit;
	vector<double>::iterator itcost;
	double most_cost_eff_machine_value = 0;

	for( it = speed_cloud.begin(), itcost=this->cost_cloud.begin() ; it != speed_cloud.end(); it++, itcost++) 
	{
		double cost_effectivy_this_machine=  Mytrunc((double)((*it)/(*itcost)),PRECISION);
		if(cost_effectivy_this_machine > most_cost_eff_machine_value)
		{
			most_cost_eff_machine_value = cost_effectivy_this_machine;
		}
	}

	for( it = speed_cloud.begin(), itcost=this->cost_cloud.begin() ; it != speed_cloud.end(); it++, itcost++, id++) 
	{
		double cost_effectivy_this_machine=  Mytrunc((double)((*it)/(*itcost)),PRECISION);
		if(abs(cost_effectivy_this_machine-most_cost_eff_machine_value)< THRESHOLDEQUALITY )
		{
			most_efficient_machines.push_back(id);		
		}

	}
}

bool MyEnvironment::argsort_efficiency(void)
{
	int counter =  0;

	vector<double> cost_eff;

	vector<double>::iterator it;
	vector<int>::iterator intit;
	vector<double>::iterator itcost;
	for( it = speed_cloud.begin(), itcost=this->cost_cloud.begin() ; it != speed_cloud.end(); it++, itcost++) 
	{
		double cost_effectivy_this_machine=  Mytrunc((double)((*it)/(*itcost)),PRECISION);
		cost_eff.push_back(cost_effectivy_this_machine);

	}
	it = cost_eff.begin();
	//print_vector(it,cost_eff.size(),"cost efficeiny array");

	//it = cost_eff.begin();
	//bubbleSort(it,cost_eff.size());


	it = cost_eff.begin();
	int * argsortlist;
	argsortlist = ArgSort(it,cost_eff.size());

	it = cost_eff.begin();
	//print_vector(it,cost_eff.size(),"cost efficeiny SORTED");


	for(int c=0;c<cost_eff.size();c++)
	{
		int arg_item = *(argsortlist+c);
		//cout<<arg_item <<endl;
		this->sorted_machine_id_efficeincy.push_back(arg_item );
	}


	return true;

}

void MyEnvironment::bubbleSort(vector<double>::iterator it,int length)//Bubble sort function 
{
	int i,j;
	for(i=0;i<length;i++)
	{
		for(j=0;j<i;j++)
		{
			if(	*(it+i)	> *(it+j))
			{
				double temp=*(it+i); //swap 
				*(it+i)=*(it+j);
				*(it+j)=temp;
			}
		}
	}
}


int * MyEnvironment::ArgSort(vector<double>::iterator it,int length)//Arg sort function 
{
	int i,j;
	int * argsort_list=new int[length];


	for(i=0;i<length;i++)
	{
		argsort_list[i]=i;		

	}
	//cout<<"initial value of argsort list"<<endl;
	/*for(i=0;i<length;i++)
	{
	cout<<*(argsort_list+i)<<"\t";
	}
	cout<<endl;*/


	for(i=0;i<length;i++)
	{
		for(j=0;j<i;j++)
		{
			if(	*(it+i)	> *(it+j))
			{
				//cout<<"\nin swapping"<<endl;
				double temp=*(it+i); //swap 				
				*(it+i)=*(it+j);
				*(it+j)=temp;
				//cout<<"*(it+i)\t"<<*(it+i);
				//cout<<"\tit+j\t"<<*(it+j); 
				int itemp=*(argsort_list+i);
				*(argsort_list+i)=*(argsort_list+j);
				*(argsort_list+j)=itemp;
				//cout<<"\ni & j\t"<<i<<"\t"<<j;
			}
		}
	}
	/*cout<<"\n\nfinal value of argsort list"<<endl;
	for(i=0;i<length;i++)
	{	
	cout<<*(argsort_list+i)<<"\t";
	}
	cout<<endl;*/

	return argsort_list;
}



double MyEnvironment::Mytrunc(double input, int precesion)
{
	int p = pow (10.0,precesion);
	double b = floor  (input * p) / p;
	return b;
}

