#pragma once
#include "Strategy.h"

class CStrategyContent
{
public:
	CStrategyContent(CStrategy * pStrategy);
	~CStrategyContent();

	void StrategyContentInface();
private:
	CStrategy *m_pStrategy;
};

