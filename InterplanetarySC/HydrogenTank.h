#ifndef __HYDROGEN_TANK_H_INCLUDED__
#define __HYDROGEN_TANK_H_INCLUDED__
#include "SubSystem.h"

class HydrogenTank: public SubSystem
{
private:
	double val;
	double maxVal;
public:
	HydrogenTank(VESSEL3 *vessel,std::string name,double *time,double amount,double maxAmount);
	void calculateStep();
};

#endif