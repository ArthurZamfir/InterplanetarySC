#pragma once
#include "SubSystem.h"

class OxygenTank: public SubSystem
{
private:
	double val;
	double maxVal;
public:
	OxygenTank(VESSEL3 *vessel,std::string name,double *time,double amount,double maxAmount);
	void calculateStep();
};