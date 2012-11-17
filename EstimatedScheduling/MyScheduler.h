#pragma once
#include "MyEnvironment.h"
#include "GenerateRandom.h"
#include "Myqueue.h"
#include "NodeMachine.h"
#include "mycomparison.h"
#include "Heap.h"
#include <queue>
using namespace std;

class MyScheduler
{
public:
	MyScheduler(void);
	~MyScheduler(void);
	
	MyEnvironment * environment;
	int H;//number of time horizons
	double H_interval;//. in seconds
	int b;//number of break points
	
	int tow; 
	int n; //number of jobs
	int m; //number of machiens
	double * tools_error_range;

	double *** estimated_task_block_running_time; //estimated task block running time [n ][ b ][ tow]
	
	double ** real_task_block_running_time; //real task block running time[n ][b ]
	double ** Weight_tools_begin_interval; //Weight tools at begin of interval [H] [tow]

	double ** estimated_task_running_time_by_each_tool ; //[tow][n];
	double ** estimated_task_run_time_begin_each_interval; //[n][h]
	double * sum_estimated_run_time_each_interval;//[h]  -> it is sum of all double ** estimated_task_run_time_begin_each_interval in each inteval

	double *T_best_begin_each_interval;
	double *T_worst_begin_each_interval;
	int * number_pareto_points ; //[H] //number of pareto points
	double ** ParetoPointsList  ;//list pareto points [H][r]
	double *** Workload_each_machine_time_interval_pareto; //[m][H][r]

	double ** sorted_estimated_task_run_time_begin_each_interval; //[h][n]
	int ** task_id_sorted_estimated_task_run_time_begin_each_interval; //[h][n] task id of sorted task according to estimated run time

	bool ****X; //assigned job j to machien m at time h for pareto point r either 0 or 1 [h][r][m][n]
	int *** running_status_task;//[h][r][n] // -3 not yet assgined, -2 means finished, -1 means not yet deployed,  0...m-1 means machine number currently running	
	int *** current_running_job_id_in_machine; // [h][r][m] m is the machien id, shows the id of job which is currently runninig ,-1 means machien is empty
	double *** assigned_workload_each_machine_yet ; //[h][r][m] ; workload of each machine at time h, sum (?)of exact value of finished job plus esitemad current assigned job
	double *** completed_workload_each_machine_yet; //[h][r][m] ; completed workload of each machine at time h, sum of exact value of finished job 
	double ** start_time_each_job; //[n][r] start time of each job in terms of second 
	bool *** not_assigned_jobs_each_interval_aprteo_point; //[h][r][n]which jobs not assigned to any machien false means will be assigned, true means not assigned
	Myqueue *** Queue_next_assigned_job_particular_machine_interval_time ; //[h][r][m]*
	//result gathering
	double * array_total_cost_of_all_scheudling  ;
	double total_cost_of_BLIND_GREEDY_ALGORITHM;
	double * array_total_makespan_of_all_scheudling  ;
	double max_runningtime_of_BLIND_GREEDY_ALGORITHM_scheudling;
	int ** not_assigned_jobs_each_paerto_scheudling;
	double ** wasted_resource_capacity_last_hour_per_machien_per_apreto;

	//structures for GREEDY BLIND_GREEDY_ALGORITHM
	double * completed_workload_each_machine_yetBLIND_GREEDY_ALGORITHM; //m

	//Declare a priority_queue and specify the ORDER as <
	//The priorities will be assigned in the Ascending Order of Age
	//priority_queue<NodeMachine, vector<NodeMachine>, mycomparison   > pqNodeMachines;
	// Create the heap
	 Heap pqNodeMachines; // = new Heap();

	
	void print_pqNodeMachines();
	void  PrintMatrix2d(double** matrix ,unsigned int rows, unsigned int cols)	;
	void  PrintMatrix3d(double*** matrix ,unsigned int rows, unsigned int cols, unsigned int depths);
	void  PrintMatrix4d(double **** matrix ,unsigned int rows, unsigned int cols, unsigned int depths, unsigned int dimen_fourth);
	void  PrintMatrix1d(double* matrix ,unsigned int rows);
	void printMatrixWorkloadEachMachineEachInterval();
	void print_scheduler(void );
	bool set_initial_value(int inputH, double H_interval, int input_b, MyEnvironment * input_env);
	void set_real_value_break_points(int type);
	bool set_initail_error_injected_tools(string tools_error_file);
	void set_esitmated_value_break_points(int type);
	void set_weight_tool_interval(int type);
	void set_estimated_running_time_each_task_each_tool(int type);
	void set_est_run_time_begin_each_interval(int type);
	void set_sum_estimated_run_time_all_task_each_interval(int type);

	void set_T_worst_best_begin_each_interval(int type);
	double  get_sum_speed_most_efficient_resoureces(void);
	double get_sum_speed_all_machine();
	int calculate_number_of_pareto_points(double min_T_r,double max_T_r);
	int set_pareto_points(int type);
	void set_workload_each_machine_time_interval_pareto(int type);
	void sort_estimated_task_run_time_begin_each_interval(int type);

	double Mytrunc(double input, int precesion);
	void bubbleSort(double* it,int length);
	void bubbleSortWithArg(double* it, int*argit, int length);
	
	void create_necessary_data_structure_for_algorithm(int type );
	void assign_task_machine_each_interval_each_schema(int type );
	void do_scheduling(int type);
	double * gather_total_cost(int type );
	double * gather_makespan(int type );
	int ** find_not_assigned_jobs(int type);
	double ** gather_wasted_resource_capacity_last_hour_per_machien_per_apreto(int type);

	void create_necessary_data_structurefor_BLIND_GREEDY_ALGORITHM();


	void do_scheduling_h0(int t_r,   int type);
	void do_deploy_h0(int t_r,   int type); //send jobs to machien
	void do_scheduling_h_gt0(int h,int t_r,int type);
	void print_allocation(int h,int t_r,int type);
	//void do_scheduling_h_gt0(int t_r, int type);
	void print_scheduling_h0(int t_r);
	void outputresult_toFile(int type,double * ex=0);
	int call_python_mojudle(int type);

};

