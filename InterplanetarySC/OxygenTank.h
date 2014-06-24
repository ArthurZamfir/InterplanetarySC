#ifndef __OXYGEN_TANK_H_INCLUDED__
#define __OXYGEN_TANK_H_INCLUDED__
#include "SubSystem.h"

class OxygenTank: public SubSystem
{
private:
	double amount;
	double *amountptr;
public:
	OxygenTank(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
	double* getAmount();
};

#endif