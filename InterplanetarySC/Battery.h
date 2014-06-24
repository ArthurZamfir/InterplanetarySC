#ifndef __BATTERY_H_INCLUDED__
#define __BATTERY_H_INCLUDED__
#include "SubSystem.h"

class Battery: public SubSystem
{
private:
	double charge;
	double maxCharge;
public:
	Battery(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
};

#endif