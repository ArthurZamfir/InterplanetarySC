#include "Heater.h"

Heater::Heater(VESSEL3 *vessel,std::string name,double *time,double maxPower,double efficiency,double maxEfficiency)
				 :SubSystem(vessel,name,time)
{
	p = 0.0;
	mp = maxPower;
	eff = efficiency;
	meff = maxEfficiency;

	attributes["Power[J]"] = &p;
	attributes["Efficiency[-]"] = &eff;

	maxAttributes["Power[J]"] = maxPower;
	maxAttributes["Efficiency[-]"] = maxEfficiency;
}

void Heater::calculateStep()
{
	std::vector<Port*> inputEnergy = collectAllActiveSubSystemsWithClassifier(inputStreams,"Energy[J]");
	std::vector<Port*> outputHeat = collectAllActiveSubSystemsWithClassifier(outputStreams,"Heat[J]");

	double inE = getPortValuesSum(inputEnergy);
	double outH = getPortValuesSum(outputHeat);

	double heat;
	double el = 0;
	//Maximal erzeugbare energie im Zeitschritt
	double maxEnergy = mp * (*simTime);
	if(isActive())
	{
		//Wärmeenergie liefern
		heat = inE * eff;
		writePortValuesEqual(outputHeat,heat);
		//Elektrische Energie Anfordern
		
		el = outH * 1/eff;

		if(el > maxEnergy)
			el = maxEnergy;
		writePortValuesEqual(inputEnergy,el);
		//Power berechnen
		p = el/(*simTime);
	}
	if(!isActive())
	{
		writePortValuesEqual(inputEnergy,0.0);
		writePortValuesEqual(outputHeat,0.0);
		p = 0.0;
	}

	//Nominal
	setNominal();
	//Anzeige von Warnungen
	if( (el - maxEnergy) < 0.01 )
		setWarning();
	//Anzeige von Fehlern
	//In diesem Fall werden keine Fehler erzeugt
	if(false)
		setError();

}
