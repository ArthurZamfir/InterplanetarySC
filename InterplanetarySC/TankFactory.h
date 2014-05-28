#pragma once
#include "Orbitersdk.h"

class TankFactory
{
private:
	VESSEL3 *defaultVessel;
	double defaultAmount;
public:
	TankFactory(VESSEL3 &vessel,double amount);
	void setVessel(VESSEL3 &vessel);
	void setAmount(double amount);

	PROPELLANT_HANDLE  GenerateTank();
	PROPELLANT_HANDLE  GenerateTank(double amount);
};

