#ifndef __THERMAL_FISSION_GENERATOR_H_INCLUDED__
#define __THERMAL_FISSION_GENERATOR_H_INCLUDED__
#include "SubSystem.h"

class ThermalFissionGenerator: public SubSystem
{
private:
	double temp;
	double power;
public:
	ThermalFissionGenerator(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
};

#endif