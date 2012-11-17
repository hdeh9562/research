#include "StdAfx.h"
#include "CoinChanges.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;


CoinChanges::CoinChanges(void)
{
	int n;
	cout<<"please enter number_coins"<<endl;
	cin>>n;
	this->number_coins=n;
	this->coins = new int[this->number_coins];
	int give_target;
	cout<<"please enter give_target"<<endl;
	cin>>give_target;
	this->target=give_target;
	this->coin_changes_array = new int [this->target];
	this->coin_changes_array[0]=0;
	for(int i =0;i<this->target;i++)
	{
		this->coin_changes_array[i]=-1;
	}
	for(int i =0;i<number_coins;i++)
	{
		//this->coins[i]= rand() % 100 +1;
		this->coins[i]= i*i+1;
		this->coin_changes_array[coins[i]]=1;
		cout<<this->coins[i]<<endl;
	}


}


CoinChanges::~CoinChanges(void)
{
}
int CoinChanges::find_min_coin_chang(int index)
{
	if(index<0)
		return 0;
	if(this->coin_changes_array[index] > 0)
		return this->coin_changes_array[index];
	int min_number = 10000000 ;
	for(int i =0;i<this->number_coins;i++)
	{
		int newindex = index-coins[i]>0?index-coins[i]:0;
		if(newindex >0)
		{
			if(this->coin_changes_array[newindex]<0)
			{
				find_min_coin_chang(newindex);
			}

			if(min_number > this->coin_changes_array[newindex])
			{
				min_number = this->coin_changes_array[newindex];
			}
		}
	}
	this->coin_changes_array[index] = min_number +1;
}


void CoinChanges::run()
{
	
	this->find_min_coin_chang(target);
	for(int i =0;i<this->target;i++)
	{
		cout<<this->coin_changes_array[i]<<"\t";
	}

	char chl;
	cin>>chl;


}