#pragma once
#include "SubSystem.h"

class Radiator: public SubSystem {
public:
  Radiator(VESSEL3* vessel,std::string name,double* time,double startHeat,double maxHeat,double radiationRate);

  void calculateStep();

private:
  double heat_;
  double mHeat_;
  double radRate_;
};