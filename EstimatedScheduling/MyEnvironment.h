#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
//#include <cmath>
//
#include <stdlib.h>

using namespace std;


class MyEnvironment
{

public:
	MyEnvironment(void);
	~MyEnvironment(void);
	int machine_numebrs;
	int task_numebrs;
	int tools_numbers;
		
	string cost_input_file;
	string speed_input_file;
	string  task_length_input_file;
	
	vector<double> speed_cloud;  //up to machine_numebrs // 1 means 1 unit of task per second
	vector<double> cost_cloud;  //up to machine_numebrs
	vector<int> sorted_machine_id_efficeincy; //up to machine_numebrs

	vector<double> actual_value_running_time; // up to task_numebrs

	vector<double> most_efficient_machines ;// only most eff machines

	bool set_inputs(int number_machines, int number_tasks, int number_tools); //return if inputs are false;
	bool set_file_names(string cost_input_file_name, string speed_input_file_name, string task_length_input_file_name); //return if inputs are false;

	bool argsort_efficiency(void);
	bool read_cost_machine_from_file(void);
	bool read_speed_machine_from_file(void);
	bool read_length_of_task_from_file(void);
	bool read_input_files(void);
	void bubbleSort(vector<double>::iterator input_array_it,int length);//Bubble sort function 
	int * ArgSort(vector<double>::iterator it,int length);//Arg sort function 
	void print_environemnt();
	static double Mytrunc(double input, int precesion);
	void find_most_eff_machines();

	
	
	static void print_vector(vector<double>::iterator it,int length,string name);
	static void iprint_vector(vector<int>::iterator it,int length,string name);


};

