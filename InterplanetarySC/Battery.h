#ifndef __BATTERY_H_INCLUDED__
#define __BATTERY_H_INCLUDED__
#include "SubSystem.h"

class Battery: public SubSystem
{
private:
	double c;
	double mc;
public:
	Battery(VESSEL3 *vessel,std::string name,double *time,double startCharge,double maxCharge);
	void calculateStep();
};

#endif