#ifndef __WATER_TANK_H_INCLUDED__
#define __WATER_TANK_H_INCLUDED__
#include "SubSystem.h"

class WaterTank: public SubSystem
{
	
private:
	double amount;
	double *amountptr;
public:
	WaterTank(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
	double* getAmount();
};

#endif