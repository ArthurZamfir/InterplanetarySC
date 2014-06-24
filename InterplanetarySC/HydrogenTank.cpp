#include "HydrogenTank.h"


void HydrogenTank::initializeSystem()
{
	amount = 5000.0;
	amountptr = &amount;
}

void HydrogenTank::calculateStep()
{
	std::vector<Port*> input = collectAllActiveSubSystemsWithClassifier(inputStreams,"H2");
	std::vector<Port*> output = collectAllActiveSubSystemsWithClassifier(outputStreams,"H2");
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

void HydrogenTank::writeAttributesToMap()
{
	//attributes.insert(std::pair<std::string,double>("Amount[kg]",amount));
	attributes["Amount[kg]"] = amount;
}

double* HydrogenTank::getAmount()
{
	return amountptr;
}