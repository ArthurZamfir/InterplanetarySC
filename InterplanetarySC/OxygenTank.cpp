#include "OxygenTank.h"


OxygenTank::OxygenTank(VESSEL3 *vessel,std::string name,double *time,double amount,double maxAmount):SubSystem(vessel,name,time)
{
	val = amount;
	maxVal = maxAmount;
	attributes["Amount[kg]"] = &val;
	maxAttributes["Amount[kg]"] = maxAmount;
}

void OxygenTank::calculateStep()
{
	std::vector<Port*> input = collectAllActiveSubSystemsWithClassifier(inputStreams,"O2");
	std::vector<Port*> output = collectAllActiveSubSystemsWithClassifier(outputStreams,"O2");
	double in = getPortValuesSum(input);
	double out = getPortValuesSum(output);

	if(operationMode == ACTIVE)
	{
		val = val + in - out;
	}
	if(operationMode == PASSIVE)
	{
		resetAllPortValues();
	}
}
