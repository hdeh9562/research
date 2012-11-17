// EstimatedScheduling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyEnvironment.h"
#include "CoinChanges.h"
#include "Kadane.h"
#include "SMSPermutation.h"
#include "longest_inc_sub_seq2d.h"
#include "GenerateRandom.h"
#include "MyScheduler.h"
#include <time.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	char ch='y';
	while (ch!='n')
	{
		SMSPermutation * objSMSPermutation=new SMSPermutation;
		objSMSPermutation->permutation(objSMSPermutation->numberofvaluse);
		cout<<"solution is:\t"<<endl;
		objSMSPermutation->print_result();
		cout<<"\nwant to continue?(y/n)"<<endl;		
		cin>>ch;
	}
	return 0;*/
	/*
	char ch='y';
	while (ch!='n')
	{
		Kadane *kadane=new Kadane;
		cout<<"solution is:\t"<<kadane->maxSubarray();
		cout<<"\nwant to continue?(y/n)"<<endl;		
		cin>>ch;
	}
	return 0;
	*/
	/*CoinChanges *coinchagne;
	coinchagne=new CoinChanges ();
	coinchagne->run();
	return 0;
	longest_inc_sub_seq2d *LIS =new longest_inc_sub_seq2d (10,10);
	LIS->run_calculation();
	char ch;
	cin>>ch;
	return 0;*/
	//srand ( time(NULL) );
	MyEnvironment e;
	GenerateRandom g;
	int task_number = 1100;
	int machines = 2;
	int tools = 2;
	e.set_inputs(machines, task_number,tools);
	int MINTAKSLENGTH= 1040;
	int MAXTAKSLENGTH= 3050;
	double * data = new double[10];
	data[0]= MINTAKSLENGTH;
	data[1]= MAXTAKSLENGTH;
	data[2]= 0;
	int inputH=180;
	double H_interval=50.0; //in minitus
	int inputB=5;


	g.generate_random_task_length(task_number,"task_length.txt",MINTAKSLENGTH,MAXTAKSLENGTH,UNIFORM ); // in terms of second
	cout<<"setting the files"<<endl;
	if(!e.set_file_names("cost.txt","speed.txt","task_length.txt"))
	{
		cout<<"problem in filing names";
		return false;
	}
	e.read_input_files();	
	e.argsort_efficiency();
	e.print_environemnt();



	MyScheduler scheduler;


	scheduler.set_initial_value(inputH,H_interval,inputB,&e);	
	scheduler.set_real_value_break_points(EQUAL);
	scheduler.set_esitmated_value_break_points(ERROR_RANDOM_TOOLS);
	scheduler.set_weight_tool_interval(TOOL_BY_WIEGHTED_SUM);
	scheduler.set_estimated_running_time_each_task_each_tool(ERROR_RANDOM_TOOLS);
	scheduler.set_est_run_time_begin_each_interval(TOOL_BY_WIEGHTED_SUM);
	scheduler.set_T_worst_best_begin_each_interval(ERROR_RANDOM_TOOLS);
	scheduler.set_pareto_points(ERROR_RANDOM_TOOLS);

	scheduler.set_sum_estimated_run_time_all_task_each_interval(ERROR_RANDOM_TOOLS);
	scheduler.set_workload_each_machine_time_interval_pareto(ERROR_RANDOM_TOOLS);
	//scheduler.printMatrixWorkloadEachMachineEachInterval();
	scheduler.sort_estimated_task_run_time_begin_each_interval(TOOL_BY_WIEGHTED_SUM);
	cout<<"sorted sorted_estimated_task_run_time_begin_each_interval\n"<<endl;
	//scheduler.PrintMatrix2d(scheduler.task_id_sorted_estimated_task_run_time_begin_each_interval, scheduler.H , scheduler.n);

	/*for(unsigned int i = 0; i < scheduler.H; i++)
	{
	for(unsigned int j = 0; j < scheduler.n; j++)
	cout<<scheduler.task_id_sorted_estimated_task_run_time_begin_each_interval[i][j]<<"\t\t";
	cout<<"\n";
	cout<<endl;
	}*/


	scheduler.create_necessary_data_structure_for_algorithm(FFD_ALGORITHM);
	scheduler.do_scheduling(FFD_ALGORITHM);
	scheduler.gather_total_cost(FFD_ALGORITHM);
	scheduler.gather_makespan(FFD_ALGORITHM);
	scheduler.find_not_assigned_jobs(FFD_ALGORITHM);
	scheduler.gather_wasted_resource_capacity_last_hour_per_machien_per_apreto(FFD_ALGORITHM);
	scheduler.outputresult_toFile(FFD_ALGORITHM);


	scheduler.create_necessary_data_structurefor_BLIND_GREEDY_ALGORITHM();
	scheduler.do_scheduling(BLIND_GREEDY_ALGORITHM);
	scheduler.print_allocation(-1,-1,BLIND_GREEDY_ALGORITHM);

	cout<<"\ntotal cost of BLIND_GREEDY_ALGORITHM \n"<<*scheduler.gather_total_cost(BLIND_GREEDY_ALGORITHM)<<endl;
	cout<<"makespan of BLIND_GREEDY_ALGORITHM \n"<<*scheduler.gather_makespan(BLIND_GREEDY_ALGORITHM)<<endl;
	scheduler.outputresult_toFile(BLIND_GREEDY_ALGORITHM);
	scheduler.outputresult_toFile(GENERAL_INFORMATION,data);


	scheduler.call_python_mojudle(FFD_ALGORITHM);







	//scheduler.print_scheduler();

	//e.find_most_eff_machines();
	//vector<double>::iterator it = e.most_efficient_machines.begin();
	//e.print_vector(it,e.most_efficient_machines.size(),"most eff machine type");

	//	double *** matrix3d = GenerateRandom::createDoubleMatrix_3d(8,5,2);
	char chh;
	cout<<"\nplease enter to finish";
	cin>>chh;
	return 0;
}

