#include "stdafx.h"
#include "OperatorDiv.h"


COperatorDiv::COperatorDiv()
{
}


COperatorDiv::~COperatorDiv()
{
}

double COperatorDiv::GetResult()
{
	if (GetNumberB() == 0)
	{
		return 0;
	}

	return GetNumberA() / GetNumberB();
}
