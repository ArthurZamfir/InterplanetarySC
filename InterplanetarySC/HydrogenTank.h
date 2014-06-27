#pragma once
#include "SubSystem.h"

class HydrogenTank: public SubSystem {
public:
  HydrogenTank(VESSEL3* vessel,std::string name,double* time,double amount,double maxAmount);

  void calculateStep();

private:
  double amount_;
  double maxAmount_;
};