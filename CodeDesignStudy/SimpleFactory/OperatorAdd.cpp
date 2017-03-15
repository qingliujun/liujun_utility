#include "stdafx.h"
#include "OperatorAdd.h"


COperatorAdd::COperatorAdd()
{
}


COperatorAdd::~COperatorAdd()
{
}


double COperatorAdd::GetResult()
{
	return GetNumberA() + GetNumberB();
}