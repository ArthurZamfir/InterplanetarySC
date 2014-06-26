#include "Battery.h"


Battery::Battery(VESSEL3 *vessel,std::string name,double *time,double startCharge,double maxCharge,
		double chargeRate,double dischargeRate,double efficiency,double maxEfficiency,double temp,double minTemp,double maxTemp)
				 :SubSystem(vessel,name,time)
{
	c = startCharge;
	mc = maxCharge;
	cr = chargeRate;
	dcr = dischargeRate;
	eff = efficiency;
	meff = maxEfficiency;
	t = temp;
	minT = minTemp;
	maxT = maxTemp;

	attributes["Energy[J]"] = &c;
	attributes["Efficiency[-]"] = &eff;
	attributes["Temperature[K]"] = &t;

	maxAttributes["Energy[J]"] = maxCharge;
	maxAttributes["Efficiency[-]"] = maxEfficiency;
	maxAttributes["Temperature[K]"] = maxTemp;
}

void Battery::calculateStep()
{
	std::vector<Port*> input = collectAllActiveSubSystemsWithClassifier(inputStreams,"Energy[J]");
	std::vector<Port*> output = collectAllActiveSubSystemsWithClassifier(outputStreams,"Energy[J]");
	double in = getPortValuesSum(input);
	double out = getPortValuesSum(output);

	if(isActive())
	{
		c = c + in - out;
		//TEMPERATUR
		//Energy demand
		if(c<mc){
			writePortValuesEqual(input,cr*(*simTime));
		}
		//No energy demand
		if(c>=mc){
			writePortValuesEqual(input,0.0);
		}
	}
	if(!isActive())
	{
		writePortValuesEqual(input,0.0);
		writePortValuesEqual(output,0.0);
	}
	
}