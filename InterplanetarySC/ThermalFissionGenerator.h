#pragma once
#include "SubSystem.h"

class ThermalFissionGenerator: public SubSystem
{
private:
	double temp;
	double mTemp;
	double power;
	double mPower;
	double rad;
	double mRad;
public:
	ThermalFissionGenerator(VESSEL3 *vessel,std::string name,double *time,
		double temperature,double maxTemperature,double startPower,double maxPower,
		double startRadiation,double maxRadiation);
	void calculateStep();
};