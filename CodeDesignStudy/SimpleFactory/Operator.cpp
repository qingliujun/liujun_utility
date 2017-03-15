#include "stdafx.h"
#include "Operator.h"


COperator::COperator()
{
}


COperator::~COperator()
{
}


double COperator::GetNumberA()
{
	return m_dNumberA;
}


void COperator::SetNumberA(double dNumberA)
{
	m_dNumberA = dNumberA;
}


double COperator::GetNumberB()
{
	return m_dNumberB;
}


void COperator::SetNumberB(double dNumberB)
{
	m_dNumberB = dNumberB;
}