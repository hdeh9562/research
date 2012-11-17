#pragma once
#include "NodeMachine.h"
class mycomparison
{
public:
	 bool reverse;
	mycomparison(void);
	mycomparison(const bool& revparam);   

	~mycomparison(void);
	bool operator() (const NodeMachine& lhs, const NodeMachine&rhs) const
  {
    if (reverse) return (lhs.release_time>rhs.release_time);
	else return (lhs.release_time<rhs.release_time);
  }

};

