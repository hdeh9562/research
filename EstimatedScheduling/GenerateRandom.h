#pragma once
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include "MyEnvironment.h"
using namespace std;

class GenerateRandom
{
public:
	GenerateRandom(void);
	~GenerateRandom(void);
	int generate_random_task_length(int number_task, string name_output_file, double min_task_length, double max_task_length, int type);
	static double generate_random_double_range(double min, double max, int type);
	static double ** createDoubleMatrix_2d(unsigned int rows, unsigned int cols);
	static double *** createDoubleMatrix_3d(unsigned int rows, unsigned int cols, unsigned int depths);
	static double **** createDoubleMatrix_4d(unsigned int rows, unsigned int cols, unsigned int depths, unsigned int dimen_fourth);

	static void  quickSort(double arr[], int left, int right) ;
	static void quicksort2(double *lp, double *rp);



};

