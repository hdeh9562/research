#include "StdAfx.h"
#include "mycomparison.h"


mycomparison::mycomparison(void)
{
	this->reverse= false;
}
mycomparison::mycomparison(const bool& revparam)
{
	this->reverse=revparam;
}
mycomparison::~mycomparison(void)
{
}
