#include "Radiator.h"

Radiator::Radiator(VESSEL3 *vessel,std::string name,double *time,double startHeat,double maxHeat,
				   double radiationRate):SubSystem(vessel,name,time)
{
	heat = startHeat;
	mHeat = maxHeat;
	radRate = radiationRate;

	attributes["Heat[J]"] = &heat;
	maxAttributes["Heat[J]"] = maxHeat;

}

void Radiator::calculateStep()
{
	std::vector<Port*> inputHeat = collectAllActiveSubSystemsWithClassifier(outputStreams,"Heat[J]");
	double inH = getPortValuesSum(inputHeat);
	if(isActive())
	{
		//Wärme abstrahlen
		heat = heat + inH - radRate * (*simTime);
		if(heat < 0)
			heat = 0;
		if(inH > 0){
			//Überprüfen ob die gesamte Wärme aufgenommen werden konnte
			double residualHeat = inH;
			double factor = 1;
			if(heat > mHeat){
				residualHeat = heat - mHeat;
				factor =  - residualHeat/inH;
				heat = mHeat;
			}
			//Beschreiben der Ports
			//Die Werte werden wieder an die Ports zurück gesendet, damit
			//diese auch wissen welche Wärmemenge abgestrahlt werden konnte.
			//Der Faktor verteilt die restliche Wärmemenge 
			std::vector<Port*>::iterator it;
			for (it = inputHeat.begin(); it!=inputHeat.end(); ++it){
				double v = (*it)->getValue();
				(*it)->setValue(v * factor);
			}
		}else{
			writePortValuesEqual(inputHeat,0.0);
		}
	}
		
	if(!isActive())
	{
		writePortValuesEqual(inputHeat,0.0);
	}

	//Nominal
	setNominal();
	//Anzeige von Warnungen
	if( (mHeat - heat) < 0.1 )
		setWarning();
	//Anzeige von Fehlern
	if( (mHeat - heat) < 0.01 )
		setError();

}
