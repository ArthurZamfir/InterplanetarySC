#include "FuelCell.h"

FuelCell::FuelCell(VESSEL3* vessel,std::string name,double* time,double efficiency,
				   double coolingWater,double maxPower):SubSystem(vessel,name,time)
{
  eff_ = efficiency;
  cool_ = coolingWater;
  coolMax_ = coolingWater;
  power_ = 0.0;
  mPower_ = maxPower;

  attributes_["Efficiency[-]"] = &eff_;
  attributes_["Cooling Water [kg/s]"] = &cool_;
  attributes_["Power[W]"]=&power_;

  maxAttributes_["Efficiency[-]"] = efficiency;
  maxAttributes_["Cooling Water [kg/s]"] = coolingWater;
  maxAttributes_["Power[W]"]= maxPower;
}

void FuelCell::calculateStep()
{
  std::vector<Port*> activeConsumers = collectAllActiveSubSystemsWithClassifier(outputStreams_,"Energy[J]");
  std::vector<Port*> waterTanksIn = collectAllActiveSubSystemsWithClassifier(inputStreams_,"H2O");
  std::vector<Port*> waterTanksOut = collectAllActiveSubSystemsWithClassifier(outputStreams_,"H2O");
  std::vector<Port*> hydrogenTanksIn = collectAllActiveSubSystemsWithClassifier(inputStreams_,"H2");
  std::vector<Port*> oxygenTanksIn = collectAllActiveSubSystemsWithClassifier(inputStreams_,"O2");
  std::vector<Port*> radiators = collectAllActiveSubSystemsWithClassifier(outputStreams_,"Heat[J]");

  //Constraints
  double requiredEnergy = getPortValuesSum(activeConsumers);

  //Variables
  double waterIn = getPortValuesSum(waterTanksIn);
  double waterOut = getPortValuesSum(waterTanksIn);
  double hydroIn = getPortValuesSum(hydrogenTanksIn);
  double oxyIn = getPortValuesSum(oxygenTanksIn);
  double heatOut = getPortValuesSum(radiators);

  setNominal();

  if(isActive())
  {
	cool_ = waterIn / (*simTime_);
	//Die benötigte Energie wird anhand der angschlossenen Verbraucher ermittelt
	double generatedEnergy = requiredEnergy / eff_;
	double tempPower = generatedEnergy/(*simTime_);
	if( (tempPower < mPower_) && !(requiredEnergy < 0.0001) ) {
	  power_ = tempPower;
	  //Erfundene Formel für den Massenstrom im verhältnis zur Energie
	  double coefficient = 0.01;
	  double totalMass = generatedEnergy * coefficient;
	  double heat = generatedEnergy;
	  //Berechnung der Variablen
	  waterIn = coolMax_*(*simTime_);
	  waterOut = waterIn + totalMass;
	  hydroIn = totalMass*2/10;
	  oxyIn = totalMass*8/10;
	  heatOut = heat;
	  //Zuweisen der Constraints
	  writePortValuesEqual(activeConsumers,requiredEnergy);
	  //Zuweisen der Variablen
	  writePortValuesEqual(waterTanksIn,waterIn);
	  writePortValuesEqual(waterTanksOut,waterOut);
	  writePortValuesEqual(hydrogenTanksIn,hydroIn);
	  writePortValuesEqual(oxygenTanksIn,oxyIn);
	  writePortValuesEqual(radiators,heatOut);
	} else {
	  power_ = 0.0;
	  //Soll weiterhin gekühlt werden solange Aktiv
	  waterIn = coolMax_*(*simTime_);
	  waterOut = waterIn;
	  writePortValuesEqual(activeConsumers,0.0);
	  writePortValuesEqual(waterTanksIn,waterIn);
	  writePortValuesEqual(waterTanksOut,waterOut);
	  writePortValuesEqual(hydrogenTanksIn,0.0);
	  writePortValuesEqual(oxygenTanksIn,0.0);
	  writePortValuesEqual(radiators,0.0);
	  setError();
	}
  }

  if(!isActive())
  {
	writePortValuesEqual(activeConsumers,0.0);
	writePortValuesEqual(waterTanksIn,0.0);
	writePortValuesEqual(waterTanksOut,0.0);
	writePortValuesEqual(hydrogenTanksIn,0.0);
	writePortValuesEqual(oxygenTanksIn,0.0);
	writePortValuesEqual(radiators,0.0);
	power_ = 0.0;
	cool_ = 0.0;
  }


}
