#include "StdAfx.h"
#include "longest_inc_sub_seq2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;


longest_inc_sub_seq2d::longest_inc_sub_seq2d(void)
{
}
longest_inc_sub_seq2d::longest_inc_sub_seq2d(int iM,int iN)
{
	this->M = (iM>0)?iM:0;		
	this->N = (iN>0)?iN:0;
	this->arr = new int *[M];
	this->temp= new int *[M];
	for (int i=0;i<M;i++)
	{
		this->arr[i] = new int[N];
		this->temp[i] = new int[N];
		for(int j=0;j<N;j++)
		{
			this->arr[i][j]= rand() % 100 +1;
			this->temp[i][j]=0;
		}		
	}
}
void longest_inc_sub_seq2d::run_calculation()
{
	int i;
	int j;
	for (i=0;i<M;i++){
		for(j=0;j<N;j++){
			if(temp[i][j] == 0){
				this->fill(i,j);
			}
		}
	}

	max_x=0,max_y=0;
	for(i=0;i<M;i++){
		for(j=0;j<N;j++){
			if(temp[i][j] > temp[max_x][max_y]){
				max_x = i;
				max_y = j;
			}
		}
	}
	cout<<"solution is\t"<<max_x<<"\t"<<max_y<<"\t"<<temp[max_x][max_y];
	cout<<endl;
	print2dArray(this->temp,this->M,this->N);
	cout<<endl;
	print2dArray(this->arr,this->M,this->N);
	findsolution();

}

void longest_inc_sub_seq2d::findsolution()
{
	int i=max_x, j= max_y ;
	cout<<endl;
	cout<<arr[i][j]<<"\t";
	cout<<i<<"\t"<<j<<endl;
	
	while(temp[i][j] != 1){
		if(i>0)
		{
			if(temp[i-1][j] == temp[i][j] -1){
				i = i-1;
				cout<<arr[i][j]<<"\t";
				cout<<i<<"\t"<<j<<endl;
				continue;
			}
		}
		if(i<M-1)
		{
			if(temp[i+1][j] == temp[i][j] -1){
				i = i+1;
				cout<<arr[i][j]<<"\t";
				cout<<i<<"\t"<<j<<endl;
				continue;
			}
		}
		if(j>0)
		{
			if(temp[i][j-1] == temp[i][j] -1){
				j = j-1;
				cout<<arr[i][j]<<"\t";
				cout<<i<<"\t"<<j<<endl;
				continue;
			}
		}
		if(j<N-1)
		{
			if(temp[i][j+1] == temp[i][j] -1){
				j = j+1;
				cout<<arr[i][j]<<"\t";
				cout<<i<<"\t"<<j<<endl;
				continue;
			}
		}
	}
}
void longest_inc_sub_seq2d::print2dArray(int ** A,int R,int C)
{
	for(int i=0;i<R;i++)
	{
		for(int j=0;j<C;j++)
		{
			cout<<A[i][j]<<"\t";
		}
		cout<<endl;
	}
}
int longest_inc_sub_seq2d::fill(int i,int j)
{
	if(i<0 || j< 0 || i>=M || j>=N)
		return 0;
	if(temp[i][j] != 0)
		return temp[i][j];
	int left=0,right=0,up=0,down=0;
	if(i>0){
		if(arr[i-1][j] < arr[i][j] )
			up = this->fill(i-1,j);
	}
	if(i<M-1)
	{
		if(arr[i+1][j] < arr[i][j] )
			down = fill(i+1,j);
	}
	if(j>0)
	{
		if(arr[i][j-1] < arr[i][j] )
			left = fill(i,j-1);
	}
	if(j<N-1)
	{
		if(arr[i][j+1] < arr[i][j] )
			right = fill(i,j+1);
	}
	temp[i][j] = max(up,max(down,max(left,right))) + 1;
	return temp[i][j];

}
longest_inc_sub_seq2d::~longest_inc_sub_seq2d(void)
{
}
