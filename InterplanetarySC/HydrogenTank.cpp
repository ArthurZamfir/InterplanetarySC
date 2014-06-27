#include "HydrogenTank.h"


HydrogenTank::HydrogenTank(VESSEL3* vessel,std::string name,double* time,double amount,double maxAmount):SubSystem(vessel,name,time)
{
  amount_ = amount;
  maxAmount_ = maxAmount;
  attributes_["Amount[kg]"] = &amount_;
  maxAttributes_["Amount[kg]"] = maxAmount;
}

void HydrogenTank::calculateStep()
{
  std::vector<Port*> input = collectAllActiveSubSystemsWithClassifier(inputStreams_,"H2");
  std::vector<Port*> output = collectAllActiveSubSystemsWithClassifier(outputStreams_,"H2");

  //Constraints
  double in = getPortValuesSum(input);
  double out = getPortValuesSum(output);

  if(isActive()) {
	double tempAmount = amount_ + in - out;
	bool flowPossible = ( (tempAmount > 0) && (tempAmount <= maxAmount_) );
	if(flowPossible) {
	  amount_ = tempAmount;
	  writePortValuesEqual(input,in);
	  writePortValuesEqual(output,out);
	}
	else {
	  writePortValuesEqual(input,0.0);
	  writePortValuesEqual(output,0.0);
	}
  }

  if(!isActive()) {
	writePortValuesEqual(input,0.0);
	writePortValuesEqual(output,0.0);
  }

  setNominal();
  //Warnung anzeigen
  if( (amount_ / maxAmount_) < 0.1 )
	setWarning();
  //Fehler anzeigen
  if( (amount_ / maxAmount_) < 0.001 )
	setError();
}