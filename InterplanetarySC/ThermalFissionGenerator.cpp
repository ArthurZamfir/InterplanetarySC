#include "ThermalFissionGenerator.h"


ThermalFissionGenerator::ThermalFissionGenerator(VESSEL3* vessel,std::string name,double* time,
		double temperature,double maxTemperature,double startPower,double maxPower,
		double startRadiation,double maxRadiation):SubSystem(vessel,name,time)
{
	temp_ = temperature;
	mTemp_ = maxTemperature;
	power_ = startPower;
	mPower_ = maxPower;
	rad_ = startRadiation;
	mRad_ = maxRadiation;

	attributes_["Temp[K]"] = &temp_;
	attributes_["Power[W]"] = &power_;
	attributes_["Radiation[Sv]"] = &rad_;

	maxAttributes_["Temp[K]"] = maxTemperature;
	maxAttributes_["Power[W]"] = maxPower;
	maxAttributes_["Radiation[Sv]"] = maxRadiation;
}

void ThermalFissionGenerator::calculateStep()
{
	outputStreams_.find("Energy[J]")->second->getValue();

	/*
	Es wird angenommen, dass die Leistung des Generators alle angeschlossenen
	Verbraucher mit Elektrischer ENergie versorgt. Die restliche Energie wird
	in Form von Wärme[J] an die Radiatoren abgegeben.
	*/
	if(isActive())
	{
		double elEnergy;
		double heatEnergy;
		elEnergy = getPortValuesSum(collectAllActiveSubSystemsWithClassifier(outputStreams_,"Energy[J]"));
		heatEnergy = power_ * (*simTime_) - elEnergy;


		//Hier sollte noch eine Funktion rein, die die Temperatur oder andere
		//charakteristiken des Generators simuliert -> Siehe Systemsimulation Skript!

		resetAllPortValues();
	}
	
}