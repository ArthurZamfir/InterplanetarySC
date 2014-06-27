#include "Heater.h"

Heater::Heater(VESSEL3* vessel,std::string name,double* time,double maxPower,double efficiency,double maxEfficiency)
				 :SubSystem(vessel,name,time)
{
	p_ = 0.0;
	mp_ = maxPower;
	eff_ = efficiency;
	meff_ = maxEfficiency;

	attributes_["Power[J]"] = &p_;
	attributes_["Efficiency[-]"] = &eff_;

	maxAttributes_["Power[J]"] = maxPower;
	maxAttributes_["Efficiency[-]"] = maxEfficiency;
}

void Heater::calculateStep()
{
	std::vector<Port*> inputEnergy = collectAllActiveSubSystemsWithClassifier(inputStreams_,"Energy[J]");
	std::vector<Port*> outputHeat = collectAllActiveSubSystemsWithClassifier(outputStreams_,"Heat[J]");

	double inE = getPortValuesSum(inputEnergy);
	double outH = getPortValuesSum(outputHeat);

	double heat;
	double el = 0;
	//Maximal erzeugbare energie im Zeitschritt
	double maxEnergy = mp_ * (*simTime_);
	if(isActive())
	{
		//Wärmeenergie liefern
		heat = inE * eff_;
		writePortValuesEqual(outputHeat,heat);
		//Elektrische Energie Anfordern
		
		el = outH * 1/eff_;

		if(el > maxEnergy)
			el = maxEnergy;
		writePortValuesEqual(inputEnergy,el);
		//Power berechnen
		p_ = el/(*simTime_);
	}
	if(!isActive())
	{
		writePortValuesEqual(inputEnergy,0.0);
		writePortValuesEqual(outputHeat,0.0);
		p_ = 0.0;
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
