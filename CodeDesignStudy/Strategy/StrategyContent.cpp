#include "stdafx.h"
#include "StrategyContent.h"


CStrategyContent::CStrategyContent(CStrategy * pStrategy)
{
	m_pStrategy = pStrategy;
}


CStrategyContent::~CStrategyContent()
{
}


void CStrategyContent::StrategyContentInface()
{
	return m_pStrategy->algorithmInterface();
}