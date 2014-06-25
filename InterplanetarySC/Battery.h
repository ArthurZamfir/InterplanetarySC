#pragma once
#include "SubSystem.h"

class Battery: public SubSystem
{
private:
	double c;
	double mc;
	double cr;
	double dcr;
	double eff;
	double meff;
	double t;
	double maxT;
	double minT;
public:
	Battery(VESSEL3 *vessel,std::string name,double *time,double startCharge,double maxCharge,
		double chargeRate,double dischargeRate,double efficiency,double maxEfficiency,double temp,double minTemp,double maxTemp);
	void calculateStep();
};