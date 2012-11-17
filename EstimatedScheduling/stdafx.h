// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "NodeJobLength.h"
#include <math.h>

#include <stdio.h>
#include <tchar.h>
#define PI 3.14159
#define NEWLINE '\n'
#define PRECISION 4  
#define UNIFORM 1  
#define EQUAL 1
#define ERROR_RANDOM_TOOLS 1
#define TOOL_BY_WIEGHTED_SUM 1
#define THRESHOLDEQUALITY 0.0001
#define REMIANINGTASKTHRESHOLDEQUALITY 0.01
#define EPSILONVALUEOFPARETOTIMEA .005
#define FFD_ALGORITHM 1
#define BLIND_GREEDY_ALGORITHM 2
#define GENERAL_INFORMATION 9

#define ASSIGNED_BUT_NOT_DEPLOYED -1
#define NOT_YET_ASSIGNED -3

#define FINISHED -2
#define  MAX_T_R 1000

bool sortingVector(const NodeJobLength& key1, const NodeJobLength& key2);


//double Mytrunc(double input, int precesion);






// TODO: reference additional headers your program requires here
