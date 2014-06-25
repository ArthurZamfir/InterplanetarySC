#pragma once
#include "SubSystem.h"

class Thruster: public SubSystem
{
private:
	double thr;
	double isp;
	std::string group;
public:
	Thruster(VESSEL3 *vessel,std::string name,double *time,double thrust,double impulse,std::string thrGroup);
	void calculateStep();
};