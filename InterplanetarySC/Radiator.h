#pragma once

#include "SubSystem.h"

class Radiator: public SubSystem
{
private:
	double heat;
	double mHeat;
	double radRate;
public:
	Radiator(VESSEL3 *vessel,std::string name,double *time,double startHeat,double maxHeat,double radiationRate);
	void calculateStep();
};