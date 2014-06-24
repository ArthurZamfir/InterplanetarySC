#include "Radiator.h"

Radiator::Radiator(VESSEL3 *vessel,std::string name,double *time,double startHeat,double maxHeat):SubSystem(vessel,name,time)
{
	heat = startHeat;
	mHeat = maxHeat;
	attributes["Heat[J]"] = &heat;
	maxAttributes["Heat[J]"] = mHeat;

}

void Radiator::calculateStep()
{
	double in = getPortValuesSum(collectAllActiveSubSystemsWithClassifier(inputStreams,"Heat[J]"));
	double out = getPortValuesSum(collectAllActiveSubSystemsWithClassifier(outputStreams,"Heat[J]"));

	if(operationMode == ACTIVE)
	{
		heat = heat + in - out;
		resetAllPortValues();
	}
}


