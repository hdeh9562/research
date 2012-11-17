#pragma once
class longest_inc_sub_seq2d
{
public:
	int M;
	int N;
	int max_x;
	int max_y;
	int **arr; // given array.
	int **temp;
	longest_inc_sub_seq2d(void);
	longest_inc_sub_seq2d(int iM,int iN);
	~longest_inc_sub_seq2d(void);
	int  fill(int R,int C);
	void run_calculation();
	void print2dArray(int **,int R,int C);
	void findsolution();
};

