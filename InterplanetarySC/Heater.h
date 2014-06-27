#pragma once
#include "SubSystem.h"

class Heater: public SubSystem {
public:
  Heater(VESSEL3* vessel,std::string name,double* time,double maxPower,
	double efficiency,double maxEfficiency);

  void calculateStep();

private:
  double p_;
  double mp_;
  double eff_;
  double meff_;
};