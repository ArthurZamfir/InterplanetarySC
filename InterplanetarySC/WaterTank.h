#ifndef __WATER_TANK_H_INCLUDED__
#define __WATER_TANK_H_INCLUDED__
#include "SubSystem.h"

class WaterTank: public SubSystem
{
	
private:
	double val;
	double maxVal;
public:
	WaterTank(VESSEL3 *vessel,std::string name,double *time,double amount,double maxAmount);
	void calculateStep();
};

#endif