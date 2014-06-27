#include "Radiator.h"

Radiator::Radiator(VESSEL3* vessel,std::string name,double* time,double startHeat,double maxHeat,
				   double radiationRate):SubSystem(vessel,name,time)
{
	heat_ = startHeat;
	mHeat_ = maxHeat;
	radRate_ = radiationRate;

	attributes_["Heat[J]"] = &heat_;
	maxAttributes_["Heat[J]"] = maxHeat;

}

void Radiator::calculateStep()
{
	std::vector<Port*> inputHeat = collectAllActiveSubSystemsWithClassifier(outputStreams_,"Heat[J]");
	double inH = getPortValuesSum(inputHeat);
	if(isActive())
	{
		//Wärme abstrahlen
		heat_ = heat_ + inH - radRate_ * (*simTime_);
		if(heat_ < 0)
			heat_ = 0;
		if(inH > 0){
			//Überprüfen ob die gesamte Wärme aufgenommen werden konnte
			double residualHeat = inH;
			double factor = 1;
			if(heat_ > mHeat_){
				residualHeat = heat_ - mHeat_;
				factor =  - residualHeat/inH;
				heat_ = mHeat_;
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
	if( (mHeat_ - heat_) < 0.1 )
		setWarning();
	//Anzeige von Fehlern
	if( (mHeat_ - heat_) < 0.01 )
		setError();

}
