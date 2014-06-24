#ifndef __FUELCELL_H_INCLUDED__
#define __FUELCELL_H_INCLUDED__
#include "SubSystem.h"

class FuelCell: public SubSystem
{
private:
	double efficiency;
	double coolingWaterPerSecond;
	double currentPower;
public:
	FuelCell(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
};

#endif