#ifndef __FUELCELL_H_INCLUDED__
#define __FUELCELL_H_INCLUDED__
#include "SubSystem.h"

class FuelCell: public SubSystem
{
private:
	double eff;
	double cool;
	double cPower;
	double mPower;
public:
	FuelCell(VESSEL3 *vessel,std::string name,double *time,double efficiency,double coolingWater,double maxPower);
	void calculateStep();
};

#endif