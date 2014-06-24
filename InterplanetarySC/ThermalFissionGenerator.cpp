#include "ThermalFissionGenerator.h"


void ThermalFissionGenerator::initializeSystem()
{
	temp = 300.0;
	power = 5000000.0;
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

void ThermalFissionGenerator::writeAttributesToMap()
{
	attributes["Temp[K]"] = temp;
	attributes["Power[W]"] = power;
}