#pragma once
#include "Orbitersdk.h"

class ThrusterFactory
{
private:
	VESSEL3 *defaultVessel;
	double defaultMaxThrust;
	double defaultISP;
	PROPELLANT_HANDLE *defaultPropTank;
public:
	ThrusterFactory(VESSEL3 &vessel,double maxThrust,double isp,PROPELLANT_HANDLE &defaultProp);

	void setVessel(VESSEL3 &vessel);
	void setThrust(double maxThrust);
	void setISP(double isp);
	void setPropellantTank(PROPELLANT_HANDLE &prop);

	THRUSTER_HANDLE *GenerateThruster(const VECTOR3 &pos, const VECTOR3 &dir);
	THRUSTER_HANDLE *GenerateThruster(const VECTOR3 &pos, const VECTOR3 &dir, double maxth0,
		PROPELLANT_HANDLE hp, double isp0);
	
};