#pragma once
#include "Operator.h"
class COperatorDiv :
	public COperator
{
public:
	COperatorDiv();
	~COperatorDiv();

	double GetResult();
};

