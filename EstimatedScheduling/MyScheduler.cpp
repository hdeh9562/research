#include "StdAfx.h"
#include "MyScheduler.h"
#include "NodeJobLength.h"
#include <stdlib.h>

MyScheduler::MyScheduler(void)
{

}


MyScheduler::~MyScheduler(void)
{

}

void MyScheduler::print_scheduler(void )
{
	cout<<"\n++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	cout<<"MyScheduler Class information \n";
	cout<<"number of time horizons:\t"<<this->H<<endl;
	cout<<"number of break points:\t"<<this->b<<endl;
	cout<<"number of tools :\t"<<this->tow<<endl;
	//cout<<"the cost of machiens are\n";
	cout<<" values for estimated_task_block_running_time"<<endl;

	cout<<" values for real_task_block_running_time "<<endl;
	//PrintMatrix2d(this->real_task_block_running_time ,b,n);

	cout<<"\n values for estimated_task_block_running_time "<<endl;
	//this->PrintMatrix3d(this->estimated_task_block_running_time , tow,b,n);

	cout<<"\n values for tools_error_range "<<endl;
	PrintMatrix1d(tools_error_range,tow);

	cout<<"\n values for Weight tools at begin of interval [H] [tow]"<<endl;
	PrintMatrix2d(this->Weight_tools_begin_interval ,this->tow, this->H);
	cout<<"\n values estimated_task_run_time_begin_each_interval[n][h]"<<endl;
	//PrintMatrix2d(this->estimated_task_run_time_begin_each_interval,this->n, this->H);

	cout<<"set_T_worst_best_begin_each_interval "<<endl;
	PrintMatrix1d(T_best_begin_each_interval,H);
	PrintMatrix1d(T_worst_begin_each_interval,H);


	cout<<"Pareto point lists\n";
	for(int h=0;h<this->H;h++)
	{
		cout<<"interval h\t"<<h<<endl;
		for(int x=0;x<this->number_pareto_points[h];x++)
		{

			cout<<this->ParetoPointsList[h][x]<<"\t";
		}
		cout<<endl;
	}


}



bool MyScheduler::set_initial_value(int inputH, double inputH_interval,  int input_b, MyEnvironment * input_env)
{
	if(input_env!=NULL)
	{
		this->environment  = input_env;
	}
	else return false;
	this->tow = this->environment->tools_numbers;
	this->m=this->environment->machine_numebrs;
	this->n=this->environment->task_numebrs;
	if(inputH>0)		
		this->H = inputH;
	else return false;
	if(inputH_interval>0)
	{
		this->H_interval = inputH_interval * 60; // to second
	}
	else
		return false;
	if(input_b>0)
	{
		this->b = input_b;
	}
	else return false;
	this->real_task_block_running_time = GenerateRandom::createDoubleMatrix_2d(b,n); //this->createDoubleMatrix2d(b,n);
	this->estimated_task_block_running_time = GenerateRandom::createDoubleMatrix_3d(tow,b,n);//this->createDoubleMatrix3d(tow,b,n);

	this->Weight_tools_begin_interval= GenerateRandom::createDoubleMatrix_2d(tow,H); ///this->createDoubleMatrix2d(tow,H);

	this->estimated_task_running_time_by_each_tool = GenerateRandom::createDoubleMatrix_2d(tow,n);

	this->estimated_task_run_time_begin_each_interval= GenerateRandom::createDoubleMatrix_2d(n,H);
	this->T_best_begin_each_interval = new double[H];
	this->T_worst_begin_each_interval = new double[H];
	this->set_initail_error_injected_tools("error.txt");


	//this->X = this->createDoubleMatrix4d(this->r, this->H, this->m,this->n);
	//this->estimated_task_block_running_time= = NULL;
	//new double[n][b][tow];

	return true;
}

bool MyScheduler::set_initail_error_injected_tools(string tools_error_file)
{
	this->tools_error_range = new double [tow];
	ifstream my_input_file;
	int counter=0;
	try {
		while(counter < this->tow)
		{
			my_input_file.open(tools_error_file);
			//Read file line by line
			double next_item;
			while(!my_input_file.eof() && counter < this->tow )
			{
				my_input_file >> next_item;
				if(next_item>0)
				{
					this->tools_error_range[counter]= next_item;
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


void MyScheduler::set_weight_tool_interval(int type)
{
	if(type == TOOL_BY_WIEGHTED_SUM)
	{
		//at h=0
		for(int tool= 0;tool<this->tow;tool++)
		{
			double temp = MyEnvironment::Mytrunc(1/double(this->tow), PRECISION);
			this->Weight_tools_begin_interval[tool][0] =temp ;
			//cout<<"this->Weight_tools_begin_interval\t"<<this->Weight_tools_begin_interval[tool][0]<<endl;
		}

		double * sum_nominator= new double [this->tow];
		for(int h=1;h < this->H;h++)
		{
			double sum_all_tools_differeces=0;
			for(int tool= 0;tool<this->tow;tool++)
			{
				sum_nominator[tool] = 0;
				for (int j=0;j<this->n;j++)
				{
					for (int bp=0;bp<this->b;bp++)
					{
						if(this->real_task_block_running_time[bp][j]  <= h*this->H_interval)
						{
							/*cout<<"bp\t"<<bp<<"\tj\t"<<j<<"\ttool\t"<<tool<<endl;
							cout<<"real_task_block_running_time[bp][j]\t"<<real_task_block_running_time[bp][j]<<endl;
							cout<<"h*this->H_interval\t"<<h*this->H_interval<<endl;
							*/
							double difference_real_with_estimated = MyEnvironment::Mytrunc( real_task_block_running_time[bp][j] - this->estimated_task_block_running_time[tool][bp][j] , PRECISION);
							if (difference_real_with_estimated <0)
							{
								difference_real_with_estimated = -difference_real_with_estimated ;
							}
							if (difference_real_with_estimated <1)
							{
								difference_real_with_estimated =1;
							}
							if(difference_real_with_estimated==0)
							{
								double nominator = this->real_task_block_running_time[bp][j];	//award a lot
								sum_nominator[tool] +=nominator ;							
							}
							else							
							{
								double nominator = this->real_task_block_running_time[bp][j] / difference_real_with_estimated;
								sum_nominator[tool] +=nominator ;							
								//cout<<"sum_nominator[tool]\t"<<sum_nominator[tool]<<endl;
							}							
						}
						else
							break;
					}
				}
				sum_all_tools_differeces +=sum_nominator[tool];
			}
			for(int tool= 0;tool<this->tow;tool++)
			{
				this->Weight_tools_begin_interval[tool][h] = MyEnvironment::Mytrunc(sum_nominator[tool]/sum_all_tools_differeces, PRECISION);
			}
		}
	}
}

void MyScheduler::set_esitmated_value_break_points(int type)
{
	if(type==ERROR_RANDOM_TOOLS)
	{
		for(int tool= 0;tool<this->tow;tool++)
		{
			double MAX_error_percentage = MyEnvironment::Mytrunc(this->tools_error_range[tool] / 100, PRECISION);
			//cout<<"MAX_error_percentage "<<tool<<"\tis \t"<<MAX_error_percentage <<endl;
			for(int i=0;i<this->n;i++)
			{
				for(int breakpoint = 0 ; breakpoint <this->b;breakpoint++)
				{
					double min = this->real_task_block_running_time[breakpoint][i] * (1-MAX_error_percentage);
					double max = this->real_task_block_running_time[breakpoint][i] * (1+MAX_error_percentage);
					/*cout<<"min value "<<tool<<"\tis \t"<<min<<endl;
					cout<<"max value "<<tool<<"\tis \t"<<max<<endl;*/
					double random_breakpoint_value = GenerateRandom::generate_random_double_range(min,max,UNIFORM);
					this->estimated_task_block_running_time[tool][breakpoint][i] =MyEnvironment::Mytrunc( random_breakpoint_value, PRECISION);
				}
			}
		}
	}
}


void MyScheduler::set_real_value_break_points(int type)
{
	if(type==EQUAL)
	{
		for(int i=0;i<this->n;i++)
		{
			double current_task_length = this->environment->actual_value_running_time.at(i);
			double each_breakpoint_length = MyEnvironment::Mytrunc(current_task_length / this->b ,PRECISION);
			for(int breakpoint = 0 ; breakpoint <this->b;breakpoint++)
			{
				this->real_task_block_running_time[breakpoint][i] =  MyEnvironment::Mytrunc(each_breakpoint_length *(breakpoint+1),PRECISION);
			}
		}
	}
}

double MyScheduler::get_sum_speed_most_efficient_resoureces(void)
{
	//this->environment->
	double sum_speed_most_eff = 0;
	this->environment->find_most_eff_machines();
	for(int index=0;index<this->environment->most_efficient_machines.size();index++)
	{
		int machine_id =(int) this->environment->most_efficient_machines.at(index);
		double speed_this_machine = this->environment->speed_cloud.at(machine_id);
		sum_speed_most_eff += speed_this_machine ;
	}
	if(sum_speed_most_eff >0)
		return sum_speed_most_eff ;
	else
		return 0;
}

int MyScheduler::calculate_number_of_pareto_points(double min_T_r,double max_T_r)
{
	if(max_T_r>min_T_r)
	{
		int numberofparetopoints = floor(max_T_r/3600) - floor(min_T_r/3600);
		return numberofparetopoints + 2;
	}
	else
	{
		int numberofparetopoints = floor(min_T_r/3600)-floor(max_T_r/3600) ;
		return numberofparetopoints + 2;
	}
	return -1;
}


int MyScheduler::set_pareto_points(int type)
{
	if(type==ERROR_RANDOM_TOOLS)
	{
		this->number_pareto_points = new int[this->H];
		this->ParetoPointsList = new double*[this->H];

		for(int h=0;h < this->H;h++)
		{
			double min_T = this->T_best_begin_each_interval[h];
			double max_T = this->T_worst_begin_each_interval[h];
			int r_of_this_h = this->calculate_number_of_pareto_points(min_T  ,max_T ); // input as second
			this->number_pareto_points[h] = r_of_this_h ;
			//create the pareop point list
			this->ParetoPointsList[h] = new double[r_of_this_h];
			for (int x=0;x < r_of_this_h;x++)
			{
				if(x==0)
				{
					this->ParetoPointsList[h][x] = min_T;
				}
				else if(x == r_of_this_h-1)
				{
					this->ParetoPointsList[h][x] = max_T;
				}
				else
				{
					double paretopoint_temp = (floor((min_T/3600))+x)*3600;//-EPSILONVALUEOFPARETOTIMEA;
					this->ParetoPointsList[h][x] = paretopoint_temp ;
					//this->ParetoPointsList is stored in terms of second
				}
			}
		}
		int temptest = 1;
	}
	return 1;
}
void MyScheduler::set_sum_estimated_run_time_all_task_each_interval(int type)
{
	if(type==ERROR_RANDOM_TOOLS)
	{
		this->sum_estimated_run_time_each_interval = new double[this->H];
		for(int h=0;h < this->H;h++)
		{
			this->sum_estimated_run_time_each_interval[h] = 0;
			for(int i =0;i<n;i++)
			{
				this->sum_estimated_run_time_each_interval[h] += this->estimated_task_run_time_begin_each_interval[i][h];
			}
		}
	}
}
void MyScheduler::set_workload_each_machine_time_interval_pareto(int type)
{
	if(type==ERROR_RANDOM_TOOLS)
	{
		//create the proper workload arrays
		this->Workload_each_machine_time_interval_pareto = new double**[this->m];		
		for(int i=0;i<m;i++)
		{
			this->Workload_each_machine_time_interval_pareto[i] = new double*[H];		
			for(int h=0;h < this->H;h++)
			{
				int num_parto_point_interval_h = this->number_pareto_points[h];
				this->Workload_each_machine_time_interval_pareto[i][h] = new double[num_parto_point_interval_h];
				for(int t_r= 0;t_r < num_parto_point_interval_h ; t_r++)
				{
					this->Workload_each_machine_time_interval_pareto[i][h][t_r] = 0;
				}

			}
		}
		//now assign the workload
		for(int h=0;h < this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			for(int t_r= 0;t_r < num_parto_point_interval_h ; t_r++)
			{					
				int counter_on_list_machines = 0;
				double reamining_unassigned_workload = Mytrunc( this->sum_estimated_run_time_each_interval[h],PRECISION);
				while(reamining_unassigned_workload > REMIANINGTASKTHRESHOLDEQUALITY )
				{
					int id_next_eff_machine = this->environment->sorted_machine_id_efficeincy.at(counter_on_list_machines++);
					//cout<<"id_next_eff_machine \t"<<id_next_eff_machine <<"\t";
					double available_capacity_this_machine = this->environment->speed_cloud.at(id_next_eff_machine)* ParetoPointsList[h][t_r];
					if(available_capacity_this_machine < reamining_unassigned_workload )
					{

						double assigned_wkl = Mytrunc( available_capacity_this_machine, PRECISION);
						this->Workload_each_machine_time_interval_pareto[id_next_eff_machine][h][t_r] = assigned_wkl ;
						reamining_unassigned_workload = Mytrunc(reamining_unassigned_workload-assigned_wkl, PRECISION);
						//	cout<<"assigned_wkl \t"<<assigned_wkl <<endl;
					}
					else
					{
						double assigned_wkl = Mytrunc(reamining_unassigned_workload , PRECISION);
						this->Workload_each_machine_time_interval_pareto[id_next_eff_machine][h][t_r] = assigned_wkl  ;
						reamining_unassigned_workload = 0;
						//cout<<"assigned_wkl \t"<<assigned_wkl <<endl;
					}
				}
			}
		}			

		/*for(int h=0;h < this->H;h++)
		{
		int num_parto_point_interval_h = this->number_pareto_points[h];
		for(int t_r= 0;t_r < num_parto_point_interval_h ; t_r++)
		{
		for(int i=0;i<this->m;i++)
		{
		cout<<"this->Workload_each_machine_time_interval_pareto["<<i<<"]["<<h<<"]["<<t_r<<"]\t"<<Workload_each_machine_time_interval_pareto[i][h][t_r] <<endl;
		}
		cout<<endl;
		}
		}*/
	}
}

void MyScheduler::set_T_worst_best_begin_each_interval(int type=ERROR_RANDOM_TOOLS)
{
	if(type == ERROR_RANDOM_TOOLS)
	{
		double sum_speed_most_eff = this->get_sum_speed_most_efficient_resoureces();
		double sum_speed_all_machines = this->get_sum_speed_all_machine();
		for(int h=0;h < this->H;h++)
		{
			double sum_estimtated_tasks = 0;
			for(int i=0;i<this->n;i++)
			{
				double estimated_task_run_time_begin_each_interval_i_h = this->estimated_task_run_time_begin_each_interval[i][h];
				sum_estimtated_tasks += estimated_task_run_time_begin_each_interval_i_h ;
			} 
			this->T_worst_begin_each_interval[h] = sum_estimtated_tasks / sum_speed_most_eff;  // in terms of second
			this->T_best_begin_each_interval[h] = sum_estimtated_tasks / sum_speed_all_machines;	// in terms of second
		}			
	}
	/*
	for(int h=0;h < this->H;h++)
	{
	cout<<"h\t"<<h<<endl;
	cout<<"this->T_worst_begin_each_interval[h]\t"<<this->T_worst_begin_each_interval[h]<<endl;
	cout<<"T_best_begin_each_interval[h]\t"<<this->T_best_begin_each_interval[h]<<endl;
	}
	*/

}

double MyScheduler::get_sum_speed_all_machine()
{
	double sum_speed_all_machines = 0;
	for(int index=0;index<this->environment->speed_cloud.size();index++)
	{
		sum_speed_all_machines += this->environment->speed_cloud.at(index);
	}
	return sum_speed_all_machines ;
}

void MyScheduler::set_estimated_running_time_each_task_each_tool(int type)
{

	if(type==ERROR_RANDOM_TOOLS)
	{
		for(int tool= 0;tool<this->tow;tool++)
		{
			for(int i=0;i<this->n;i++)
			{
				this->estimated_task_running_time_by_each_tool [tool][i] = this->estimated_task_block_running_time[tool][this->b-1][i];
			}
		}
	}
}

void MyScheduler::set_est_run_time_begin_each_interval(int type)
{
	if(type==TOOL_BY_WIEGHTED_SUM)
	{
		for(int i=0;i<this->n;i++)
		{
			for(int h=0;h < this->H;h++)
			{
				this->estimated_task_run_time_begin_each_interval[i][h] = 0;
				for(int tool= 0;tool<this->tow;tool++)
				{
					this->estimated_task_run_time_begin_each_interval[i][h] += this->Weight_tools_begin_interval[tool][h] * this->estimated_task_running_time_by_each_tool[tool][i];

				}
				//cout<<"estimated_task_run_time_begin_each_interval["<<h<<"]\tjob["<<i<<"]is\t"<<this->estimated_task_run_time_begin_each_interval[i][h]<<endl;
			}
		}
	}
}
void MyScheduler::sort_estimated_task_run_time_begin_each_interval(int type)
{
	if(type==TOOL_BY_WIEGHTED_SUM)
	{
		this->sorted_estimated_task_run_time_begin_each_interval = new double*[H];
		this->task_id_sorted_estimated_task_run_time_begin_each_interval= new int*[H];
		for(int h=0;h<this->H;h++)
		{
			this->sorted_estimated_task_run_time_begin_each_interval[h] = new double[n];
			this->task_id_sorted_estimated_task_run_time_begin_each_interval[h] = new int[n];

			for(int j=0;j<n;j++)
			{
				this->task_id_sorted_estimated_task_run_time_begin_each_interval[h][j] = j;
				this->sorted_estimated_task_run_time_begin_each_interval[h][j] = this->estimated_task_run_time_begin_each_interval[j][h];
			}
			//this->bubbleSort(sorted_estimated_task_run_time_begin_each_interval[h],this->n);
			this->bubbleSortWithArg(sorted_estimated_task_run_time_begin_each_interval[h],this->task_id_sorted_estimated_task_run_time_begin_each_interval[h],this->n);
			//GenerateRandom::quicksort2(&this->estimated_task_run_time_begin_each_interval[h][0],&this->estimated_task_run_time_begin_each_interval[h][n-1]);			
		}
	}
}



void  MyScheduler::PrintMatrix4d(double **** matrix ,unsigned int rows, unsigned int cols, unsigned int depths, unsigned int dimen_fourth)
{
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			for(int d=0;d<depths;d++)
			{
				for(int d4=0;d4<dimen_fourth;d4++)
				{
					cout<<matrix[i][j][d][d4]<<"\t";
				}
				cout<<endl;
			}
			cout<<"\n+++++++++++++++++++++++++++++++"<<endl;
		}
		cout<<"*******************************\n"<<endl;
	}
}

void  MyScheduler::PrintMatrix1d(double* matrix ,unsigned int rows)
{

	for(unsigned int j = 0; j < rows; j++)
	{
		cout<<matrix[j]<<"\t";
	}
	cout<<endl;
}

void  MyScheduler::PrintMatrix3d(double*** matrix ,unsigned int rows, unsigned int cols, unsigned int depths)
{

	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			for(int d=0;d<depths;d++)
			{
				cout<<matrix[i][j][d]<<"\t";
			}
			cout<<endl;
		}
		cout<<"++++++++++++++++++++"<<endl;
	}
}
double MyScheduler::Mytrunc(double input, int precesion)
{
	return MyEnvironment::Mytrunc(input,precesion);
}


void  MyScheduler::PrintMatrix2d(double** matrix ,unsigned int rows, unsigned int cols)	
{
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
			cout<<matrix[i][j]<<"\t\t";
		cout<<endl;
	}
}


void MyScheduler::printMatrixWorkloadEachMachineEachInterval()
{
	for (int h=0;h<H;h++)
	{	
		for(int i=0;i<m;i++)
		{
			cout<<"workload of machien\t"<<i<<endl;

			int num_parto_point_interval_h = this->number_pareto_points[h];
			for(int t_r= 0;t_r < num_parto_point_interval_h ; t_r++)
			{
				//cout<<"workload of pareto point\t"<<t_r<<endl;

				cout<<this->Workload_each_machine_time_interval_pareto[i][h][t_r]<<"\t";
			}
			cout<<endl;
			cout<<endl;
		}
		cout<<endl;
	}

}
void MyScheduler::assign_task_machine_each_interval_each_schema(int type )
{
	//first we build the X arrays for time hoirzon zero
	int h=0;



}

void MyScheduler::do_scheduling(int type)
{
	if(type==FFD_ALGORITHM)
	{
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			for(int t_r= 0;t_r<num_parto_point_interval_h ;t_r++)
			{
				if(h==0)
				{
					do_scheduling_h0(t_r,type);
					//print_scheduling_h0(t_r);
				}
				else if(h>0)
				{
					do_scheduling_h_gt0(h,t_r,type);;//
				}
				//print_allocation(h,t_r,type);
			}
		}
	}
	else if(type= BLIND_GREEDY_ALGORITHM)
	{

		for(int i=0;i<m;i++)
		{			
			NodeMachine mytestNodeMachine(i,0);
			pqNodeMachines.insert(mytestNodeMachine);
			pqNodeMachines.print();
			//print_pqNodeMachines();
		}
		print_pqNodeMachines();
		for(int j=0;j<n;j++)
		{
			double run_time_job = this->environment->actual_value_running_time.at(j);
			
			//int next_availble_machien_id = pqNodeMachines.top().machine_id ;
			NodeMachine min_finsh_time_NodeMachine = pqNodeMachines.deletemin();
			int next_availble_machien_id = min_finsh_time_NodeMachine.machine_id ;
			double speed_next_availble_machien = this->environment->speed_cloud.at(next_availble_machien_id);
			double relaase_time_last_job_in_this_machine = min_finsh_time_NodeMachine.release_time;
			//cout<<"next_availble_machien_id \t"<<next_availble_machien_id <<endl;
			//cout<<"run_time_job \t"<<run_time_job  << "\t"<<run_time_job /speed_next_availble_machien <<endl;
			//			pqNodeMachines.pop();
			//print_pqNodeMachines();
			//pqNodeMachines.print();
			char ch;
			//cin>>ch;
			
			double finish_time_this_job_this_machine = Mytrunc(run_time_job /speed_next_availble_machien ,PRECISION) + relaase_time_last_job_in_this_machine;
			this->completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM[next_availble_machien_id]+= run_time_job;
			NodeMachine tempmytestNodeMachine(next_availble_machien_id ,finish_time_this_job_this_machine );
			pqNodeMachines.insert(tempmytestNodeMachine);
		}

		/*		while ( !pqNodeMachines.empty() ) {			cout << pqNodeMachines.top().machine_id << endl;		pqNodeMachines.pop();		} cout << endl;
		*/

	}

}
void MyScheduler::print_pqNodeMachines()
{
	cout<<"size of queue\t"<<this->pqNodeMachines.size()<<"\t";

}
void MyScheduler::create_necessary_data_structurefor_BLIND_GREEDY_ALGORITHM()
{
	this->completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM = new double[m];
	for(int i=0;i<m;i++)
	{
		this->completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM[i]=0;
	}

}
void MyScheduler::print_scheduling_h0(int t_r)
{
	int h=0;
	cout<<"array X for t_r\t"<<t_r<<endl;
	for(int i=0;i<this->m;i++)
	{
		for(int j=0;j<this->n;j++)
		{
			cout<<X[h][t_r][i][j]<<" ";
		}
		cout<<endl;
	}
}
void MyScheduler::do_scheduling_h0(int t_r , int type)
{
	int h=0;
	if(type==FFD_ALGORITHM)
	{
		for(int j=0;j<this->n;j++)
		{
			int id_next_longer_job = this->task_id_sorted_estimated_task_run_time_begin_each_interval[h][j];
			//cout<<"id_next_longer_job number "<<j<<"\t is \t"<<id_next_longer_job <<endl;
			double estimated_length_job = this->estimated_task_run_time_begin_each_interval[id_next_longer_job][h];
			//cout<<"estimated_length_job number "<<id_next_longer_job<<"\t is \t"<<estimated_length_job <<endl;
			for(int i=0;i<this->m;i++)
			{


				int id_next_most_eff_machine = this->environment->sorted_machine_id_efficeincy.at(i);
				this->completed_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ]=0;
				//cout<<"id_next_most_eff_machine at time "<<h<<"\t is \t"<<id_next_most_eff_machine <<endl;
				double current_workload_machine = this->assigned_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ];
				//cout<<"current_workload_machine  "<<id_next_most_eff_machine<<"\t is \t"<<current_workload_machine <<endl;
				double cap_workload_this_machine = this->Workload_each_machine_time_interval_pareto[id_next_most_eff_machine ][h][t_r];
				//cout<<"cap_workload_this_machine "<<id_next_most_eff_machine<<"\t is \t"<<cap_workload_this_machine <<endl;
				if(current_workload_machine + estimated_length_job <= cap_workload_this_machine)
				{
					//do assginment of this job to this machien
					this->X[h][t_r][id_next_most_eff_machine ][id_next_longer_job] = true;
					//cout<<"true value set for "<<"X["<<h<<"]["<<t_r<<"]["<<id_next_most_eff_machine <<"]["<<id_next_longer_job<<"]"<<endl;

					this->assigned_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ] += estimated_length_job;
					//cout<<"assigned_workload_each_machine_yet["<<h<<"]["<<t_r<<"]["<<id_next_most_eff_machine<< " ]" <<assigned_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ]<<endl ;

					if(this->current_running_job_id_in_machine[h][t_r][id_next_most_eff_machine ] == -1)
					{
						this->current_running_job_id_in_machine[h][t_r][id_next_most_eff_machine ] = id_next_longer_job;
						this->start_time_each_job[id_next_longer_job][t_r] = 0; //send at time zero 
						this->running_status_task[h][t_r][id_next_longer_job] = id_next_most_eff_machine ;
					}
					else
					{
						this->running_status_task[h][t_r][id_next_longer_job] = ASSIGNED_BUT_NOT_DEPLOYED ;
						this->Queue_next_assigned_job_particular_machine_interval_time[h][t_r][id_next_most_eff_machine].insert(id_next_longer_job);
					}
					break;
				}
				else if ( (i==this->m-1))
				{
					//we cannot find any proper assignment
					//cout<<
					not_assigned_jobs_each_interval_aprteo_point[h][t_r][id_next_longer_job] = true; // this job will not be assgined to any machien
				}
			}
		} 
	}
}
void MyScheduler::do_scheduling_h_gt0(int h,int t_r,int type)
{

	if(type==FFD_ALGORITHM)
	{
		/*for(int i=0;i<m;i++)
		{
		this->Queue_next_assigned_job_particular_machine_interval_time[h-1][t_r][i].print_queue();
		}*/
		double current_time_in_simulation = h * H_interval;
		//cout<<" *******\ncurrent_time_in_simulation \t"<<current_time_in_simulation <<"\tin pareto point\t"<<t_r<<endl;
		for(int j=0;j<this->n;j++)
		{

			if(running_status_task[h-1][t_r][j]==-3)//-3 not yet assgined
			{
				running_status_task[h][t_r][j]=-3;
				//cout<<"running_status_task["<<h<<"]["<<t_r<<"]["<<j<<"]\t"<<running_status_task[h][t_r][j]<<endl;

			}

			if(running_status_task[h-1][t_r][j]>=0) //means this job have already assigned to some machine to run, we mush check whether it has been finished or not at h
			{
				unsigned int machineid = running_status_task[h-1][t_r][j];
				double start_time_this_task = start_time_each_job[j][t_r];
				//cout<<"start_time_ of task "<<j<<"\t\t"<<start_time_this_task <<endl;
				double actual_task_processing_time_regard_speed_machine = this->environment->actual_value_running_time.at(j) / this->environment->speed_cloud.at(machineid);
				//cout<<"actual_task_processing_time\t"<<j<<" \t"<<actual_task_processing_time_regard_speed_machine <<endl;
				//cout<<"actual_value_running_time.at( "<<j<<" ) \t"<<this->environment->actual_value_running_time.at(j)<<"\tspeed_cloud.at("<<machineid<<")\t"<<this->environment->speed_cloud.at(machineid)<<endl;
				double finish_time_this_job = start_time_this_task + actual_task_processing_time_regard_speed_machine ;
				//cout<<"finish_time_ of _job "<<j<<"\t"<<finish_time_this_job <<endl;

				if(finish_time_this_job  <= current_time_in_simulation)
				{
					this->completed_workload_each_machine_yet[h][t_r][machineid] += this->environment->actual_value_running_time.at(j) ;
					for(int temph=h;temph<this->H;temph++)
					{
						this->completed_workload_each_machine_yet[temph][t_r][machineid] = this->completed_workload_each_machine_yet[h][t_r][machineid] ;
						//	cout<<"completed_workload_each_machine_yet["<<temph<<"]["<<t_r<<"]["<<machineid<<"]\t"<<completed_workload_each_machine_yet[temph][t_r][machineid]<<endl;
					}

					//this job will finish before the current actual time in simulaiton of interval h
					//assigned_workload_each_machine_yet[h][t_r][machineid] = assigned_workload_each_machine_yet[h-1][t_r][machineid] + this->environment->actual_value_running_time.at(j) ;
					this->running_status_task[h][t_r][j] =  -2 ;//finished 
					for(int temph=h;temph<this->H;temph++)
					{
						this->running_status_task[temph][t_r][j] =  -2 ;//finished
						//cout<<"running_status_task["<<temph<<"]["<<t_r<<"]["<<j<<"]\t"<<running_status_task[temph][t_r][j]<<endl;

					}
					char ch;
					//	cin>>ch;
					while(finish_time_this_job  <= current_time_in_simulation)
					{
						//first we find the next job must assigned to this machien accrodign h and t_r from queue
						if(this->Queue_next_assigned_job_particular_machine_interval_time[h-1][t_r][machineid].isempty())
						{
							//cout<<"\ncompleted_workload_each_machine_yet["<<h<<"]["<<t_r<<"]["<<machineid<<"]"<<completed_workload_each_machine_yet[h][t_r][machineid] <<endl;
							current_running_job_id_in_machine[h][t_r][machineid]= -1;
							break;
						}
						else
						{
							int id_of_next_available_job_to_run_this_machie= this->Queue_next_assigned_job_particular_machine_interval_time[h-1][t_r][machineid].remove();
							//cout<<"id_of_next_available_job_to_run on machie" << machineid<<" from Queue \t"<<id_of_next_available_job_to_run_this_machie <<endl;
							actual_task_processing_time_regard_speed_machine = this->environment->actual_value_running_time.at(id_of_next_available_job_to_run_this_machie) / this->environment->speed_cloud.at(machineid);
							this->start_time_each_job[id_of_next_available_job_to_run_this_machie][t_r] = finish_time_this_job  ;						
							//cout<<"start_time_ of task "<<id_of_next_available_job_to_run_this_machie<<"\t on machine"<<machineid <<"\tis\t"<<this->start_time_each_job[id_of_next_available_job_to_run_this_machie][t_r]<<endl;
							running_status_task[h][t_r][id_of_next_available_job_to_run_this_machie] = machineid;
							//cout<<"running_status_task["<<h<<"]["<<t_r<<"]["<<id_of_next_available_job_to_run_this_machie<<"]\t"<<machineid<<endl;

							this->current_running_job_id_in_machine[h][t_r][machineid] =id_of_next_available_job_to_run_this_machie;
							//then we calculate does this job finish before current h? if so, do it and go to while
							finish_time_this_job = this->start_time_each_job[id_of_next_available_job_to_run_this_machie][t_r] + actual_task_processing_time_regard_speed_machine ;
							//cout<<"finish_time_ job no. "<<id_of_next_available_job_to_run_this_machie<<"\t"<<finish_time_this_job <<endl;
							if(finish_time_this_job  <= current_time_in_simulation)
							{
								this->completed_workload_each_machine_yet[h][t_r][machineid] += this->environment->actual_value_running_time.at(id_of_next_available_job_to_run_this_machie) ;

								for(int temph=h;temph<this->H;temph++)
								{
									this->completed_workload_each_machine_yet[temph][t_r][machineid] = this->completed_workload_each_machine_yet[h][t_r][machineid] ;
									//cout<<"completed_workload_each_machine_yet["<<temph<<"]["<<t_r<<"]["<<machineid<<"]\t"<<completed_workload_each_machine_yet[temph][t_r][machineid]<<endl;
								}

								//assigned_workload_each_machine_yet[h][t_r][machineid] += this->environment->actual_value_running_time.at(id_of_next_available_job_to_run_this_machie) ;
								this->running_status_task[h][t_r][id_of_next_available_job_to_run_this_machie] =  -2 ;//finished 
								for(int temph=h;temph<this->H;temph++)
								{
									this->running_status_task[temph][t_r][id_of_next_available_job_to_run_this_machie] =  -2 ;//finished
									//cout<<"running_status_task["<<temph<<"]["<<t_r<<"]["<<id_of_next_available_job_to_run_this_machie<<"]\t"<<running_status_task[temph][t_r][id_of_next_available_job_to_run_this_machie]<<endl;
								}
								char chl;
								//cin>>chl;

							}			
							else
							{
								//not finieshd between two interval
								assigned_workload_each_machine_yet[h][t_r][machineid] = this->estimated_task_run_time_begin_each_interval[id_of_next_available_job_to_run_this_machie][h];
								//this->environment->actual_value_running_time.at(id_of_next_available_job_to_run_this_machie) ;
							}
							//cout<<"\ncompleted_workload_each_machine_yet["<<h<<"]["<<t_r<<"]["<<machineid<<"]"<<completed_workload_each_machine_yet[h][t_r][machineid] <<endl;
							//cout<<"\nassigned_workload_each_machine_yet["<<h<<"]["<<t_r<<"]["<<machineid<<"]"<<assigned_workload_each_machine_yet[h][t_r][machineid] <<endl;

						}
					}
					//while finishing time this job <= current_time_in_simulation; this job must be extracted from x
				}
				else
				{
					this->running_status_task[h][t_r][j] = this->running_status_task[h-1][t_r][j];
				}

			}
		}
		//do algorithm ffd at time h
		//cout<<"\n*********************\nt-r is = "<<t_r<<"\tdo algorithm ffd at time "<<h<<endl;
		for(int j=0;j<this->n;j++)
		{
			int id_next_longer_job = this->task_id_sorted_estimated_task_run_time_begin_each_interval[h][j];
			//cout<<"id_next_longer_job \t"<<id_next_longer_job <<endl;
			//cout<<"this->running_status_task["<<h<<"]["<<t_r<<"][ "<<id_next_longer_job<<" ]\t"<<this->running_status_task[h][t_r][id_next_longer_job]<<endl;


			if(this->running_status_task[h][t_r][id_next_longer_job]==-1 || this->running_status_task[h][t_r][id_next_longer_job]==-3)
			{
				//cout<<"id_next_longer_job number not yet assigned or not running "<<"\t is \t"<<id_next_longer_job <<endl;
				//not yet deployed or not yet assigned
				double estimated_length_job_at_this_interval = this->estimated_task_run_time_begin_each_interval[id_next_longer_job][h];
				//cout<<"estimated_length_job number "<<id_next_longer_job<<"\t is \t"<<estimated_length_job_at_this_interval <<endl;
				for(int i=0;i<this->m;i++)
				{
					int id_next_most_eff_machine = this->environment->sorted_machine_id_efficeincy.at(i);
					double current_workload_machine =this->completed_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ] +  assigned_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ];
					//cout<<"current_workload_machine  "<<id_next_most_eff_machine<<"\t is \t"<<current_workload_machine <<endl;
					double cap_workload_this_machine = this->Workload_each_machine_time_interval_pareto[id_next_most_eff_machine ][h][t_r];
					//cout<<"cap_workload_this_machine "<<id_next_most_eff_machine<<"\t is \t"<<cap_workload_this_machine <<endl;
					if(current_workload_machine + estimated_length_job_at_this_interval <= cap_workload_this_machine)
					{
						//assign this job to this machien
						this->X[h][t_r][id_next_most_eff_machine ][id_next_longer_job] = true;
						//cout<<"true value set for "<<"X["<<h<<"]["<<t_r<<"]["<<id_next_most_eff_machine <<"]["<<id_next_longer_job<<"]"<<endl;

						this->assigned_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ] += estimated_length_job_at_this_interval;
						//cout<<"assigned_workload_each_machine_yet["<<h<<"]["<<t_r<<"]["<<id_next_most_eff_machine<< " ]" <<assigned_workload_each_machine_yet[h][t_r][id_next_most_eff_machine ]<<endl ;
						if(this->current_running_job_id_in_machine[h][t_r][id_next_most_eff_machine ] == -1)
						{
							//machien is not running any job, so deployed
							this->current_running_job_id_in_machine[h][t_r][id_next_most_eff_machine ] = id_next_longer_job;
							this->start_time_each_job[id_next_longer_job][t_r] =current_time_in_simulation ; //send at time h 
							this->running_status_task[h][t_r][id_next_longer_job] = id_next_most_eff_machine ;
						}
						else
						{
							this->running_status_task[h][t_r][id_next_longer_job] = ASSIGNED_BUT_NOT_DEPLOYED ;
							this->Queue_next_assigned_job_particular_machine_interval_time[h][t_r][id_next_most_eff_machine].insert(id_next_longer_job);
							//Queue_next_assigned_job_particular_machine_interval_time[h][t_r][id_next_most_eff_machine].print_queue();

						}
						break;
					}
					else if ( (i==this->m-1))
					{
						//we cannot find any proper assignment
						//cout<<
						not_assigned_jobs_each_interval_aprteo_point[h][t_r][id_next_longer_job] = true;
						this->running_status_task[h][t_r][id_next_longer_job]=-3;
					}
				}
			}
		}
	}

}
double * MyScheduler::gather_total_cost(int type )
{
	if(type==FFD_ALGORITHM)
	{
		this->array_total_cost_of_all_scheudling  = new double [this->number_pareto_points[H-1]];
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			double total_cost_fo_this_scheudling =0;
			for(int i=0;i<m;i++)
			{
				double final_workload_this_machie = this->completed_workload_each_machine_yet[H-1][t_r][i];
				double speed_thisMachine = this->environment->speed_cloud.at(i);
				double cost_this_machine_per_hour= this->environment->cost_cloud.at(i);
				double cost_ruuning =( floor(final_workload_this_machie  / (speed_thisMachine*3600) ) + 1 ) * cost_this_machine_per_hour;
				total_cost_fo_this_scheudling +=cost_ruuning ;
			}
			this->array_total_cost_of_all_scheudling[t_r]= total_cost_fo_this_scheudling ;
		}
		//print it
		cout<<"\n\nTOTAL COST OF ALL SCHEUDLGIN"<<endl;
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			cout<<array_total_cost_of_all_scheudling  [t_r]<<"\t";
		}
		cout<<endl;
		return array_total_cost_of_all_scheudling   ;
	}

	else if(type==BLIND_GREEDY_ALGORITHM)
	{
		cout<<endl;
		//assume that this scheduler.do_scheduling(BLIND_GREEDY_ALGORITHM); has been called
		//double * array_total_cost_of_scheudling  = new double [1];
		this->total_cost_of_BLIND_GREEDY_ALGORITHM =0;
		for(int i=0;i<m;i++)
		{
			double final_workload_this_machie = 	this->completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM[i];
			cout<<i<<"\t"<<final_workload_this_machie <<"\t\t";
			double speed_thisMachine = this->environment->speed_cloud.at(i);
			double cost_this_machine_per_hour= this->environment->cost_cloud.at(i);
			double cost_ruuning =( floor(final_workload_this_machie  /  (speed_thisMachine*3600) ) + 1 ) * cost_this_machine_per_hour;

			cout<<"\tcost\t"<<cost_ruuning <<"\t"<<endl;
			total_cost_of_BLIND_GREEDY_ALGORITHM +=cost_ruuning ;
		}
		char ch;
		//cin>>ch;
		return &total_cost_of_BLIND_GREEDY_ALGORITHM ;
	}
}

double * MyScheduler::gather_makespan(int type )
{
	if(type==FFD_ALGORITHM)
	{
		this->array_total_makespan_of_all_scheudling  = new double [this->number_pareto_points[H-1]];
		int * index_oftotal_makespan_of_all_scheudling   = new int [this->number_pareto_points[H-1]];
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			double max_runningtime_fo_this_scheudling =0;
			int index_makespan = -1;
			for(int i=0;i<m;i++)
			{
				double final_workload_this_machie = this->completed_workload_each_machine_yet[H-1][t_r][i];
				double speed_thisMachine = this->environment->speed_cloud.at(i);

				double ruuning_time_this_machine = (final_workload_this_machie  / (speed_thisMachine*3600) ) ;
				if(ruuning_time_this_machine > max_runningtime_fo_this_scheudling )
				{
					max_runningtime_fo_this_scheudling  = ruuning_time_this_machine;
					index_makespan  = i;
				}
			}
			this->array_total_makespan_of_all_scheudling [t_r]= max_runningtime_fo_this_scheudling ;
			index_oftotal_makespan_of_all_scheudling   [t_r]= index_makespan ;				 
		}	
		//print it
		cout<<"\n\nMakespan OF ALL SCHEUDLGIN"<<endl;
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			cout<<this->array_total_makespan_of_all_scheudling [t_r]<<"\t ";
		}
		cout<<"happend in machie\n";
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			cout<<index_oftotal_makespan_of_all_scheudling [t_r]<<"\t ";
		}

		cout<<endl;
		return this->array_total_makespan_of_all_scheudling   ;
	}
	else if(type == BLIND_GREEDY_ALGORITHM)
	{
		this->max_runningtime_of_BLIND_GREEDY_ALGORITHM_scheudling =0;

		int index_makespan = -1;
		for(int i=0;i<m;i++)
		{
			double final_workload_this_machie = this->completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM[i];
			double speed_thisMachine = this->environment->speed_cloud.at(i);
			double ruuning_time_this_machine = (final_workload_this_machie  / (speed_thisMachine*3600) ) ;
			if(ruuning_time_this_machine > max_runningtime_of_BLIND_GREEDY_ALGORITHM_scheudling  )
			{
				this->max_runningtime_of_BLIND_GREEDY_ALGORITHM_scheudling   = ruuning_time_this_machine;
				index_makespan  = i;
			}
		}
		return &this->max_runningtime_of_BLIND_GREEDY_ALGORITHM_scheudling ;

	}



}

int ** MyScheduler::find_not_assigned_jobs(int type)
{
	if(type==FFD_ALGORITHM)
	{
		this->not_assigned_jobs_each_paerto_scheudling = new int *[this->number_pareto_points[H-1]];
		int * number_of_not_assigned_jobs_each_paerto_scheudling = new int [this->number_pareto_points[H-1]];
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			int number_of_un_assigned_jobs = 0;
			for(int j=0;j<n;j++)
			{
				if(this->running_status_task[H-1][t_r][j]==-3)
				{
					number_of_un_assigned_jobs++;
				}

			}
			number_of_not_assigned_jobs_each_paerto_scheudling [ t_r] =  number_of_un_assigned_jobs;
			this->not_assigned_jobs_each_paerto_scheudling[t_r] = new int[number_of_un_assigned_jobs];
			int index=0;
			for(int j=0;j<n;j++)
			{
				if(this->running_status_task[H-1][t_r][j]==-3)
				{
					this->not_assigned_jobs_each_paerto_scheudling[t_r][index++] = j; 
				}
			}
		}

		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			cout<<"number_of_un_assigned_jobs in pareto  point "<<t_r<<"\tis \t"<<number_of_not_assigned_jobs_each_paerto_scheudling [ t_r]<<endl;
			cout<<"the following index of jobs\n";
			for(int index=0;index<number_of_not_assigned_jobs_each_paerto_scheudling [ t_r];index++)
			{
				cout<<this->not_assigned_jobs_each_paerto_scheudling[t_r][index]<<"\t";
			}
		}
		cout<<endl;
		return this->not_assigned_jobs_each_paerto_scheudling;
	}//end type
}


double ** MyScheduler::gather_wasted_resource_capacity_last_hour_per_machien_per_apreto(int type)
{
	if(type==FFD_ALGORITHM)
	{
		this->wasted_resource_capacity_last_hour_per_machien_per_apreto= new double *[this->number_pareto_points[H-1]];
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			this->wasted_resource_capacity_last_hour_per_machien_per_apreto[t_r] = new double [this->m];
			for(int i=0;i<m;i++)
			{
				double final_workload_this_machie = this->completed_workload_each_machine_yet[H-1][t_r][i];
				double speed_thisMachine = this->environment->speed_cloud.at(i);
				double hom_many_hours_this_machine_used = Mytrunc(final_workload_this_machie / speed_thisMachine,PRECISION) ;
				//	cout<<"hom_many_hours_this_machine_used\t"<<hom_many_hours_this_machine_used<<endl;
				int fractianl_ofhom_many_hours_this_machine_used  = floor( hom_many_hours_this_machine_used );
				if( abs(fractianl_ofhom_many_hours_this_machine_used   - hom_many_hours_this_machine_used ) > THRESHOLDEQUALITY )
				{
					//cout<<"fractianl_ofhom_many_hours_this_machine_used\t"<<fractianl_ofhom_many_hours_this_machine_used<<endl;
					double wasted_this_machiine = (1- (hom_many_hours_this_machine_used - fractianl_ofhom_many_hours_this_machine_used  )) * speed_thisMachine ;

					//cout<<"speed_thisMachine\t"<<speed_thisMachine<<endl;
					//cout<<"wasted_this_machiine\t"<<wasted_this_machiine<<endl;
					this->wasted_resource_capacity_last_hour_per_machien_per_apreto[t_r][i] = Mytrunc(wasted_this_machiine, PRECISION);
				}
				else 
					this->wasted_resource_capacity_last_hour_per_machien_per_apreto[t_r][i] = 0 ;

			}
		}

		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			cout<<"wasted_resource_capacity_last_hour_per_machien_per_apreto"<<t_r<<"\tis \t"<<endl;			
			for(int index=0;index<m;index++)
			{
				cout<<this->wasted_resource_capacity_last_hour_per_machien_per_apreto[t_r][index]<<"\t";
			}
		}
		cout<<endl;
		return this->wasted_resource_capacity_last_hour_per_machien_per_apreto; 
	}
}

void MyScheduler::outputresult_toFile(int type,double * externaldata)
{
	if(type==FFD_ALGORITHM)
	{
		ofstream myfile;
		myfile.open ("result_FFD.txt");

		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			myfile<<this->array_total_makespan_of_all_scheudling [t_r]<<"\t";
		}
		myfile<<endl;
		for(int t_r=0;t_r<this->number_pareto_points[H-1];t_r++)
		{
			myfile<<array_total_cost_of_all_scheudling  [t_r]<<"\t";
		}

		//myfile << "Writing this to a file.\n";
		myfile.close();

	}
	else if(type==BLIND_GREEDY_ALGORITHM)
	{
		ofstream myfile;
		myfile.open ("result_BLIND_GREEDY.txt");

		myfile<<max_runningtime_of_BLIND_GREEDY_ALGORITHM_scheudling;
		myfile<<endl;
		myfile<<total_cost_of_BLIND_GREEDY_ALGORITHM;
		myfile.close();
	}
	else if(type==GENERAL_INFORMATION)
	{
		ofstream myfile;
		myfile.open ("GENERAL_INFORMATION.txt");
		myfile<<this->n;
		myfile<<endl;
		myfile<<this->m;
		myfile<<endl;
		myfile<<this->H;
		myfile<<endl;
		myfile<<this->b;
		myfile<<endl;
		myfile<<this->tow;
		myfile<<endl;
		if(externaldata)
		{
			int i=0;
			while (*(externaldata+i)!=0)
			{
				myfile<<*(externaldata+i);
				myfile<<endl;
				i++;
			}
		}
	}

}

int MyScheduler::call_python_mojudle(int type)
{
	int return_code;
	if(type==FFD_ALGORITHM)
	{

		return_code = system ("python plt.py");
	}
	cout<<"return_code\t"<<return_code;
	return 	return_code;

}


void MyScheduler::print_allocation(int h,int t_r,int type)
{
	if(type==FFD_ALGORITHM)
	{
		cout<<"RUNNING STATUS of JOBS in MACHINES at time\t"<<h<<"\t for scheudling "<<t_r<<endl;
		for (int j=0;j<this->n;j++)
		{
			cout<<this->running_status_task[h][t_r][j]<<"\t";
		}
		cout<<endl;
		cout<<"current workload of each machines at time \t"<<h<<"\t for scheudling "<<t_r<<endl;
		for(int i = 0 ; i < this->m; i++)
		{
			cout<<this->assigned_workload_each_machine_yet[h][t_r][i]<<"\t";
		}
		cout<<endl;


		cout<<"completed_workload_each_machine_yet"<<" at time="<<h<<" t_r=  "<<t_r<<endl;;
		for(int i=0;i<m;i++)
		{
			cout<<completed_workload_each_machine_yet[h][t_r][i]<<"\t";
		}
		char ch;
		//cin>>ch;


	}
	if(type==BLIND_GREEDY_ALGORITHM)
	{
		cout<<"BLIND_GREEDY_ALGORITHM workload of each machine\n";
		for(int i=0;i<m;i++)
		{
			cout<<this->completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM[i]<<"\t";
		}

	}
}
void MyScheduler::do_deploy_h0(int t_r,   int type) //send jobs to machien
{
	int h=0;
	if(type==FFD_ALGORITHM)
	{
		;
		//;
	}

}

void MyScheduler::create_necessary_data_structure_for_algorithm(int type )
{
	if(type==FFD_ALGORITHM)
	{
		char charecthre;
		//cin>>charecthre;
		/*cout<<"\ncreate X"<<endl;
		cout<<"\nthis->H"<<this->H<<endl;
		cout<<"\nthis->m"<<this->m<<endl;
		cout<<"\nthis->n"<<this->n<<endl;*/
		this->X = new bool ***[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			//cout<<"\n num_parto_point_interval_h "<<num_parto_point_interval_h <<endl;
			this->X[h] = new bool **[num_parto_point_interval_h];
			for(int t_r= 0;t_r < num_parto_point_interval_h ; t_r++)
			{
				this->X[h][t_r] = new bool*[this->m];
				for (int i=0;i<this->m;i++)
				{
					this->X[h][t_r][i]= new bool[this->n];
					for(int j=0;j<this->n;j++)
					{
						this->X[h][t_r][i][j] = false;
					}
				}
			}
		}
		// cout<< "sizeof( X) =  \t"<< sizeof( this->X ); 
		cout<<"create running_status_task"<<endl;
		this->running_status_task = new int**[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			this->running_status_task[h] = new int *[num_parto_point_interval_h];
			for(int t_r=0;t_r<num_parto_point_interval_h;t_r++)
			{
				this->running_status_task[h][t_r] = new int[this->n];
				for(int j=0;j<this->n;j++)
				{
					this->running_status_task[h][t_r][j] = NOT_YET_ASSIGNED ;//-3 ;//means not yet assigned 
				}
			}
		}						 
		cout<<"current_running_job_id_in_machine "<<endl;
		this->current_running_job_id_in_machine = new int **[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			this->current_running_job_id_in_machine[h] = new int*[num_parto_point_interval_h];
			for(int t_r=0;t_r<num_parto_point_interval_h;t_r++)
			{
				this->current_running_job_id_in_machine[h][t_r] = new int[this->m];
				for(int i=0;i<this->m;i++)
				{
					this->current_running_job_id_in_machine[h][t_r][i] = -1;//means not yet any job dispatched to this machine
				}
			}
		}


		cout<<"assigned_workload_each_machine_yet"<<endl;
		this->assigned_workload_each_machine_yet = new double **[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			this->assigned_workload_each_machine_yet[h] = new double*[num_parto_point_interval_h];
			for(int t_r=0;t_r<num_parto_point_interval_h;t_r++)
			{
				this->assigned_workload_each_machine_yet[h][t_r] = new double[this->m];
				for(int i=0;i<this->m;i++)
				{
					this->assigned_workload_each_machine_yet[h][t_r][i] = 0;//means not yet any workload to this machine
				}
			}
		}		

		cout<<"completed_workload_each_machine_yet"<<endl;
		this->completed_workload_each_machine_yet = new double **[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			this->completed_workload_each_machine_yet[h] = new double*[num_parto_point_interval_h];
			for(int t_r=0;t_r<num_parto_point_interval_h;t_r++)
			{
				this->completed_workload_each_machine_yet[h][t_r] = new double[this->m];
				for(int i=0;i<this->m;i++)
				{
					this->completed_workload_each_machine_yet[h][t_r][i] = 0;//means not yet any workload finished at this machine
				}
			}
		}		


		cout<<"start_time_each_job"<<endl;
		this->start_time_each_job = new double*[this->n];
		for(int j=0;j<this->n;j++)
		{
			this->start_time_each_job[j] = new double[MAX_T_R];
			for(int t_r=0;t_r<MAX_T_R;t_r++)
			{
				this->start_time_each_job[j][t_r] = -1;//means not yet dispatched

			}
		}
		cout<<"create not_assigned_jobs_each_interval_aprteo_point"<<endl;
		this->not_assigned_jobs_each_interval_aprteo_point = new bool**[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];
			this->not_assigned_jobs_each_interval_aprteo_point[h] = new bool *[num_parto_point_interval_h];
			for(int t_r=0;t_r<num_parto_point_interval_h;t_r++)
			{
				this->not_assigned_jobs_each_interval_aprteo_point[h][t_r] = new bool[this->n];
				for(int j=0;j<this->n;j++)
				{
					this->not_assigned_jobs_each_interval_aprteo_point[h][t_r][j] = false ;//means every mahine will be assigned later
				}
			}
		}
		cout<<"create Queue_next_assigned_job_particular_machine_interval_time "<<endl;

		this->Queue_next_assigned_job_particular_machine_interval_time = new Myqueue **[this->H];
		for(int h=0;h<this->H;h++)
		{
			int num_parto_point_interval_h = this->number_pareto_points[h];			
			this->Queue_next_assigned_job_particular_machine_interval_time[h] = new Myqueue *[num_parto_point_interval_h];
			for(int t_r= 0;t_r < num_parto_point_interval_h ; t_r++)
			{
				this->Queue_next_assigned_job_particular_machine_interval_time[h][t_r] = new Myqueue[this->m];
				for (int i=0;i<this->m;i++)
				{
					char myString [17]= "Q_m_"; 					
					char * buffer  = new char[5];
					itoa(i,buffer,10);
					strcat ( myString , buffer);
					delete [] buffer;
					char *buffer2  = new char[5];
					buffer2="_r_";
					strcat ( myString , buffer2);
					char * buffer3  = new char[5];
					itoa(t_r,buffer3,10);
					strcat ( myString , buffer3);
					delete [] buffer3;
					char * buffer4  = new char[5];
					buffer4="_h_";
					strcat ( myString , buffer4);
					char * buffer5  = new char[5];
					itoa(h,buffer5,10);
					strcat ( myString , buffer5);
					delete [] buffer5;
					/*for(int y=-9;y<0;y++)
					{
					this->Queue_next_assigned_job_particular_machine_interval_time[h][t_r][i].insert(y);
					}*/
					this->Queue_next_assigned_job_particular_machine_interval_time[h][t_r][i].set_name(myString );
					//this->Queue_next_assigned_job_particular_machine_interval_time[h][t_r][i].print_queue();
				}
			}
		}		
	}
}



void MyScheduler::bubbleSortWithArg(double* it, int*argit, int length)
{
	vector<NodeJobLength> VectorofNodeJobLength;
	vector<NodeJobLength>::iterator vectorit;
	for(int i=0;i<length;i++)
	{
		NodeJobLength newNodeJobLength;
		newNodeJobLength.jobindex = *(argit+i);
		newNodeJobLength.joblength = *(it+i);
		VectorofNodeJobLength.push_back(newNodeJobLength);
	}
	std::sort(VectorofNodeJobLength.begin(), VectorofNodeJobLength.end(), sortingVector);
	int index=0;
	for(vectorit=VectorofNodeJobLength.begin();vectorit!=VectorofNodeJobLength.end();vectorit++)
	{
		*(argit+index) = (*vectorit).jobindex;
		*(it+index) = (*vectorit).joblength;		
	}

	/*cout<<"\n Before sort\n";
	for(vectorit=VectorofNodeJobLength.begin();vectorit!=VectorofNodeJobLength.end();vectorit++)
	{
	cout<<"("<<(*vectorit).jobindex<<","<<(*vectorit).joblength<<")\t";
	}
	cout<<"\n Arg before sort\n";
	for(vectorit=VectorofNodeJobLength.begin();vectorit!=VectorofNodeJobLength.end();vectorit++)
	{
	cout<<(*vectorit).jobindex<<"\t";
	}

	cout<<"\n After sort\n";
	
	for(vectorit=VectorofNodeJobLength.begin();vectorit!=VectorofNodeJobLength.end();vectorit++)
	{
	cout<<"("<<(*vectorit).jobindex<<","<<(*vectorit).joblength<<")\t";
	}
	cout<<"\n Arg sort\n";
	for(vectorit=VectorofNodeJobLength.begin();vectorit!=VectorofNodeJobLength.end();vectorit++)
	{
	cout<<(*vectorit).jobindex<<"\t";
	}
	*/

	/*
	int i,j;
	for(i=0;i<length;i++)
	{
	for(j=0;j<i;j++)
	{
	if(	*(it+i)	> *(it+j))
	{
	int tempid = *(argit+i);
	*(argit+i)=*(argit+j);
	*(argit+j)=tempid;
	double temp=*(it+i); //swap 
	*(it+i)=*(it+j);
	*(it+j)=temp;
	}
	}
	}*/

}

void MyScheduler::bubbleSort(double* it,int length)//Bubble sort function 
{
	std::sort(it,it+length);
	/*
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
	}*/
}