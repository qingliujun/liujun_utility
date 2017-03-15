#ifndef __OPERATOR_H__
#define __OPERATOR_H__

class COperator
{
public:
	COperator();
	~COperator();

	double GetNumberA();
	void SetNumberA(double dNumberA);

	double GetNumberB();
	void SetNumberB(double dNumberB);

	virtual double GetResult() = 0;

private:
	double m_dNumberA;
	double m_dNumberB;
};


#endif
