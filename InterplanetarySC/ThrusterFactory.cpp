#include "ThrusterFactory.h"
#include "Orbitersdk.h"


ThrusterFactory::ThrusterFactory(VESSEL3 &vessel,double maxThrust,double isp,PROPELLANT_HANDLE &defaultProp)
{
	setVessel(vessel);
	setThrust(maxThrust);
	setISP(isp);
	setPropellantTank(defaultProp);
}

void ThrusterFactory::setVessel(VESSEL3 &vessel)
{
	defaultVessel=&vessel;
}

void ThrusterFactory::setThrust(double maxThrust)
{
	defaultMaxThrust = maxThrust;
}

void ThrusterFactory::setISP(double isp)
{
	defaultISP = isp;
}

void ThrusterFactory::setPropellantTank(PROPELLANT_HANDLE &prop)
{
	defaultPropTank = &prop;
}

THRUSTER_HANDLE ThrusterFactory::GenerateThruster(const VECTOR3 &pos, const VECTOR3 &dir, double maxth0,
		PROPELLANT_HANDLE hp, double isp0)
{
	return defaultVessel->CreateThruster(pos,dir,maxth0,hp,isp0);
}

THRUSTER_HANDLE ThrusterFactory::GenerateThruster(const VECTOR3 &pos, const VECTOR3 &dir)
{
	return defaultVessel->CreateThruster(pos,dir,defaultMaxThrust,defaultPropTank,defaultISP);
}