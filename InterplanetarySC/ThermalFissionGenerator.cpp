#include "ThermalFissionGenerator.h"


ThermalFissionGenerator::ThermalFissionGenerator(VESSEL3 *vessel,std::string name,double *time,
		double temperature,double maxTemperature,double startPower,double maxPower,
		double startRadiation,double maxRadiation):SubSystem(vessel,name,time)
{
	temp = temperature;
	mTemp = maxTemperature;
	power = startPower;
	mPower = maxPower;
	rad = startRadiation;
	mRad = maxRadiation;

	attributes["Temp[K]"] = &temp;
	attributes["Power[W]"] = &power;
	attributes["Radiation[Sv]"] = &rad;

	maxAttributes["Temp[K]"] = maxTemperature;
	maxAttributes["Power[W]"] = maxPower;
	maxAttributes["Radiation[Sv]"] = maxRadiation;
}

void ThermalFissionGenerator::calculateStep()
{
	outputStreams.find("Energy[J]")->second->getValue();

	/*
	Es wird angenommen, dass die Leistung des Generators alle angeschlossenen
	Verbraucher mit Elektrischer ENergie versorgt. Die restliche Energie wird
	in Form von Wärme[J] an die Radiatoren abgegeben.
	*/
	if(operationMode == ACTIVE)
	{
		double elEnergy;
		double heatEnergy;
		elEnergy = getPortValuesSum(collectAllActiveSubSystemsWithClassifier(outputStreams,"Energy[J]"));
		heatEnergy = power * (*simTime) - elEnergy;


		//Hier sollte noch eine Funktion rein, die die Temperatur oder andere
		//charakteristiken des Generators simuliert -> Siehe Systemsimulation Skript!

		resetAllPortValues();
	}
	
}