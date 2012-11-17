#pragma once
class CoinChanges
{
public:
	CoinChanges(void);
	~CoinChanges(void);
	int number_coins;
	int * coins;
	int target;
	int * coin_changes_array;
	int find_min_coin_chang(int index);
	void run();

};

