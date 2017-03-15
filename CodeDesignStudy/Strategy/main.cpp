// Strategy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "StrategyA.h"
#include "StrategyB.h"
#include "StrategyContent.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CStrategyContent *	pStrategyContent;
	CStrategy*			pStrategy;

	pStrategy = new CStrategyA();
	pStrategyContent = new CStrategyContent(pStrategy);
	pStrategyContent->StrategyContentInface();
	delete pStrategy;
	pStrategy = NULL;
	delete pStrategyContent;
	pStrategyContent = NULL;

	pStrategy = new CStrategyB();
	pStrategyContent = new CStrategyContent(pStrategy);
	pStrategyContent->StrategyContentInface();
	delete pStrategy;
	pStrategy = NULL;
	delete pStrategyContent;
	pStrategyContent = NULL;

	return 0;
}

