#pragma once
#include "SubSystem.h"

class FuelCell: public SubSystem {
public:
  FuelCell(VESSEL3 *vessel,std::string name,double *time,double efficiency,double coolingWater,double maxPower);

  void calculateStep();

private:
  double eff_;
  double cool_;
  double coolMax_;
  double power_;
  double mPower_;
};