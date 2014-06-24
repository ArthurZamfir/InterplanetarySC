#include "FuelCell.h"

FuelCell::FuelCell(VESSEL3 *vessel,std::string name,double *time,double efficiency,
				   double coolingWater,double maxPower):SubSystem(vessel,name,time)
{
	eff = efficiency;
	cool = coolingWater;
	cPower = 0.0;
	mPower = maxPower;

	attributes["Efficiency[-]"] = &eff;
	attributes["Cooling Water [kg]"] = &cool;
	attributes["Power[W]"]=&cPower;

	maxAttributes["Efficiency[-]"] = efficiency;
	maxAttributes["Cooling Water [kg]"] = coolingWater;
	maxAttributes["Power[W]"]= maxPower;
}

void FuelCell::calculateStep()
{
	std::vector<Port*> activeConsumers = collectAllActiveSubSystemsWithClassifier(outputStreams,"Energy[J]");
		std::vector<Port*> waterTanksIn = collectAllActiveSubSystemsWithClassifier(inputStreams,"H2O");
		std::vector<Port*> waterTanksOut = collectAllActiveSubSystemsWithClassifier(outputStreams,"H2O");
		std::vector<Port*> hydrogenTanksIn = collectAllActiveSubSystemsWithClassifier(inputStreams,"H2");
		std::vector<Port*> oxygenTanksIn = collectAllActiveSubSystemsWithClassifier(inputStreams,"O2");
		std::vector<Port*> radiators = collectAllActiveSubSystemsWithClassifier(outputStreams,"Heat[J]");

	if(operationMode == ACTIVE)
	{
		//Konstanter Eingangswasserstrom
		double waterIntake = 4.0*(*simTime);
		//Die benötigte Energie wird anhand der angschlossenen Verbraucher ermittelt
		double requiredEnergy = getPortValuesSum(activeConsumers);
		double generatedEnergy = requiredEnergy / eff;
		cPower = generatedEnergy/(*simTime);
		//Erfundene Formel für den Massenstrom im verhältnis zur Energie
		double coefficient = 0.01;
		double totalMass = generatedEnergy * coefficient;
		double heat = generatedEnergy;
		//Zuweisen der Variablen
		writePortValuesEqual(activeConsumers,requiredEnergy);
		writePortValuesEqual(waterTanksIn,waterIntake);
		writePortValuesEqual(waterTanksOut,waterIntake + totalMass);
		writePortValuesEqual(hydrogenTanksIn,totalMass*2/10);
		writePortValuesEqual(oxygenTanksIn,totalMass*8/10);
		writePortValuesEqual(radiators,heat);
		

		//Hier weiter machen:
		//Die Masse des Wassertanks wird nicht richtig aktualisiert!
	}

	if(operationMode == PASSIVE)
	{
		writePortValuesEqual(activeConsumers,0.0);
		writePortValuesEqual(waterTanksIn,0.0);
		writePortValuesEqual(waterTanksOut,0.0);
		writePortValuesEqual(hydrogenTanksIn,0.0);
		writePortValuesEqual(oxygenTanksIn,0.0);
		writePortValuesEqual(radiators,0.0);
		cPower = 0.0;
		cool = 0.0;
	}


}
