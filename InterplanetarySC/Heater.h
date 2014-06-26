#pragma once
#include "SubSystem.h"

class Heater: public SubSystem
{
private:
	double p;
	double mp;
	double eff;
	double meff;
public:
	Heater(VESSEL3 *vessel,std::string name,double *time,double maxPower,
		double efficiency,double maxEfficiency);
	void calculateStep();
};