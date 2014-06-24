#include "WaterTank.h"

void WaterTank::initializeSystem()
{
	amount = 5000.0;
	amountptr = &amount;
}

void WaterTank::calculateStep()
{
	std::vector<Port*> input = collectAllActiveSubSystemsWithClassifier(inputStreams,"O2");
	std::vector<Port*> output = collectAllActiveSubSystemsWithClassifier(outputStreams,"O2");
	double in = getPortValuesSum(input);
	double out = getPortValuesSum(output);

	if(operationMode == ACTIVE)
	{
		amount = amount + in - out;
	}
	if(operationMode == PASSIVE)
	{
		resetAllPortValues();
	}

	
}

void WaterTank::writeAttributesToMap()
{
	attributes["Amount[kg]"] = amount;
}

double* WaterTank::getAmount()
{
	return amountptr;
}
