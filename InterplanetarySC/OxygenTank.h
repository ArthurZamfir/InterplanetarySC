#pragma once
#include "SubSystem.h"

class OxygenTank: public SubSystem {
public:
  OxygenTank(VESSEL3* vessel,std::string name,double* time,double amount,double maxAmount);

  void calculateStep();

private:
  double amount_;
  double maxAmount_;
};