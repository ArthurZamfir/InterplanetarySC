#ifndef __RADIATOR_H_INCLUDED__
#define __RADIATOR_H_INCLUDED__


#include "SubSystem.h"

class Radiator: public SubSystem
{
private:
	double heat;
public:
	Radiator(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
};

#endif