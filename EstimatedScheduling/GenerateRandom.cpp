#include "StdAfx.h"
#include "GenerateRandom.h"
#include <time.h>


GenerateRandom::GenerateRandom(void)
{

}


GenerateRandom::~GenerateRandom(void)
{
}

int GenerateRandom::generate_random_task_length(int number_task, string name_output_file, double min_task_length, double max_task_length, int type)
{
	ofstream outputFile;
	try {
		outputFile.open(name_output_file);
	}
	catch (ifstream::failure e) {
		cout << "\n\nException writing file\n";
		return false;
	}
	if(type==UNIFORM)//uniformly distributed random number in the span [a,b]
	{
		int cntr = 0 ;
		if(number_task>0)
		{
			while(cntr<number_task)
			{
				double range = (max_task_length - min_task_length) + 1;
				double r = MyEnvironment::Mytrunc( min_task_length + range *  (double)rand() / ((double)RAND_MAX+1) , 1);
				try {
					outputFile << r << endl;
				}
				catch (ifstream::failure e) {
					cout << "\n\nException writing file\n";
					return false;
				}
				cntr++;
			}
		}
	}

}

double GenerateRandom::generate_random_double_range(double min, double max, int type)
{
	if(type==UNIFORM)//uniformly distributed random number in the span [a,b]
	{
		
		double range = (max - min) + 1;
		double r = MyEnvironment::Mytrunc( min + range *  (double)rand() / ((double)RAND_MAX+1) , 1);
		return r;
	}
}
double ** GenerateRandom::createDoubleMatrix_2d(unsigned int rows, unsigned int cols)
{

	double  ** matrix;
	matrix = new double*[rows];
	//matrix = (double **) calloc(cols, sizeof(double *));
	for(unsigned int i = 0; i < rows; i++)
	{
		matrix[i] = new double[cols]; 
	}
	/*
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			matrix[i][j] = i+j;
			cout<<matrix[i][j]<<"\t";
		}
		cout<<endl;
	}
	*/



	return matrix;

}

double *** GenerateRandom::createDoubleMatrix_3d(unsigned int rows, unsigned int cols, unsigned int depths)
{
	double  *** matrix;
	matrix = new double**[rows];
	//matrix = (double **) calloc(cols, sizeof(double *));
	for(unsigned int i = 0; i < rows; i++)
	{
		matrix[i] =		GenerateRandom::createDoubleMatrix_2d(cols,depths);			
	}
	//cout<<"in 3d matrix"<<endl;

	/*
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			for(unsigned int h = 0; h < depths; h++)
			{
				matrix[i][j][h] = i+j+h;
				cout<<matrix[i][j][h]<<"\t";
			}
			cout<<endl;
		}
		cout<<"\n+++++++++++++++++++++++++++++++++++"<<endl;
	}
	*/
	return matrix;
}


double **** GenerateRandom::createDoubleMatrix_4d(unsigned int rows, unsigned int cols, unsigned int depths, unsigned int dimen_fourth)
{
	double  **** matrix;
	matrix = new double***[rows];
	//matrix = (double **) calloc(cols, sizeof(double *));
	for(unsigned int i = 0; i < rows; i++)
	{
		matrix[i] =		GenerateRandom::createDoubleMatrix_3d(cols,depths, dimen_fourth);			
	}
	
	 
	return matrix;
}


void GenerateRandom::quickSort(double arr[], int left, int right) {

      int i = left, j = right;
      double tmp;
	  //int newindex = int((left + right) / 2);
      double pivot = *(arr+left);
      /* partition */
      while (left < right) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}

void GenerateRandom::quicksort2(double *lp, double *rp){
	//passes++;
	int arraysize = (rp-lp)+1;
	if(arraysize > 1){
		double *pivot = (lp+(arraysize/2));
		double swap = 0;
		double *origl = lp;
		double *origr = rp;
		while (lp != rp){
			while (*lp < *pivot){
				lp++;
			}
			while (*rp > *pivot){
				rp--;
			}
			if (lp == pivot){
				pivot = rp;
			}
			else if (rp == pivot){
				pivot = lp;
			}
			swap = *lp;
			*lp = *rp;
			*rp = swap;
			if((*lp == *pivot) || (*rp == *pivot)){
				break;
			}
		}
		GenerateRandom::quicksort2(origl, pivot-1);
		GenerateRandom::quicksort2(pivot+1, origr);
	}
}



