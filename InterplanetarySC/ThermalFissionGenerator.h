#pragma once
#include "SubSystem.h"

class ThermalFissionGenerator: public SubSystem {

public:
  ThermalFissionGenerator(VESSEL3* vessel,std::string name,double* time,
	double temperature,double maxTemperature,double startPower,double maxPower,
	double startRadiation,double maxRadiation);

  void calculateStep();

private:
  double temp_;
  double mTemp_;
  double power_;
  double mPower_;
  double rad_;
  double mRad_;
};