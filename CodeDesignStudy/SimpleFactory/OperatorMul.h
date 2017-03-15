#pragma once
#include "Operator.h"
class COperatorMul :
	public COperator
{
public:
	COperatorMul();
	~COperatorMul();

	double GetResult();
};

