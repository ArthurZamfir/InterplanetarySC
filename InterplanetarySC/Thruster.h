#ifndef __THRUSTER_H_INCLUDED__
#define __THRUSTER_H_INCLUDED__
#include "SubSystem.h"

class Thruster: public SubSystem
{
private:
	double thrust;
	double isp;
	std::string group;
public:
	Thruster(VESSEL3 *vessel,std::string name,double *time):SubSystem(vessel,name,time){initializeSystem();};
	void initializeSystem();
	void calculateStep();
	void writeAttributesToMap();
};

#endif