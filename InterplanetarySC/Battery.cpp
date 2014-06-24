#include "Battery.h"


Battery::Battery(VESSEL3 *vessel,std::string name,double *time,double startCharge,
				 double maxCharge):SubSystem(vessel,name,time)
{
	c = startCharge;
	mc = maxCharge;
	attributes["Energy[J]"] = &c;
	maxAttributes["Energy[J]"] = maxCharge;
}

void Battery::calculateStep()
{
	std::vector<Port*> input = collectAllActiveSubSystemsWithClassifier(inputStreams,"Energy[J]");
	std::vector<Port*> output = collectAllActiveSubSystemsWithClassifier(outputStreams,"Energy[J]");
	double in = getPortValuesSum(input);
	double out = getPortValuesSum(output);

	if(operationMode == ACTIVE)
	{
		if(c<mc)
		{
			c = c + in;
			writePortValuesEqual(input,1.0);
		}
		if(c>=mc)
		{
			writePortValuesEqual(input,0.0);
		}
	}
	if(operationMode == PASSIVE)
	{
		writePortValuesEqual(input,0.0);
		writePortValuesEqual(output,0.0);
	}
	
}