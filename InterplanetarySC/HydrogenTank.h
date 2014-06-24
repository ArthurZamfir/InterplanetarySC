#ifndef __HYDROGEN_TANK_H_INCLUDED__
#define __HYDROGEN_TANK_H_INCLUDED__
#include "SubSystem.h"

class HydrogenTank: public SubSystem
{
private:
	double amount;
	double *amountptr;
public:
	HydrogenTank(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
	double* getAmount();
};

#endif