#ifndef __OPERATOR_ADD_H__
#define __OPERATOR_ADD_H__
#include "Operator.h"

class COperatorAdd :
	public COperator
{
public:
	COperatorAdd();
	~COperatorAdd();

	double GetResult();
};

#endif