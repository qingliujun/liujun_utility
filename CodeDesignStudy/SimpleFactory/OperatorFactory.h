#pragma once
#include "Operator.h"

class COperatorFactory
{
public:
	COperatorFactory();
	~COperatorFactory();


	COperator * CreateOpetator(char strOper);
};

