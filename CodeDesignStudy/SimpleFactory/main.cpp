// SimpleFactory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OperatorFactory.h"


int _tmain(int argc, _TCHAR* argv[])
{
	double dRes = 0;

	COperatorFactory fac;
	COperator * pOper = fac.CreateOpetator('+');
	pOper->SetNumberA(23);
	pOper->SetNumberB(34);
	dRes = pOper->GetResult();

	return 0;
}

