#include "TankFactory.h"
#include "Orbitersdk.h"

TankFactory::TankFactory(VESSEL3 &vessel,double amount)
{
	defaultVessel = &vessel;
	defaultAmount = amount;
}

void TankFactory::setVessel(VESSEL3 &vessel)
{
	defaultVessel=&vessel;
}

void TankFactory::setAmount(double amount)
{
	defaultAmount=amount;
}

PROPELLANT_HANDLE TankFactory::GenerateTank()
{
	return defaultVessel->CreatePropellantResource(defaultAmount);
}

PROPELLANT_HANDLE  TankFactory::GenerateTank(double amount)
{
	return defaultVessel->CreatePropellantResource(amount);
}