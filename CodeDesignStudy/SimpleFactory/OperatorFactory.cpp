#include "stdafx.h"
#include "OperatorFactory.h"
#include "OperatorAdd.h"
#include "OperatorSub.h"
#include "OperatorMul.h"
#include "OperatorDiv.h"

COperatorFactory::COperatorFactory()
{
}


COperatorFactory::~COperatorFactory()
{
}


COperator * COperatorFactory::CreateOpetator(char strOper)
{
	COperator* pOperator = NULL;

	switch (strOper)
	{
	case '+':
		pOperator = new COperatorAdd();
		break;
	case '-':
		pOperator = new COperatorSub();
		break;
	case '*':
		pOperator = new COperatorMul();
		break;
	case '%':
		pOperator = new COperatorDiv();
		break;
	default:
		break;
	}

	return pOperator;
}