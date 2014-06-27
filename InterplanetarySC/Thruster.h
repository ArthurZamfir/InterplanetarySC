#pragma once
#include "SubSystem.h"

class Thruster: public SubSystem {
public:
  Thruster(VESSEL3* vessel,std::string name,double* time,VECTOR3 position,
	VECTOR3 direction,double thrust,PROPELLANT_HANDLE propellant,
	double impulse,THGROUP_TYPE thrType[3],int types,double exhaustL,double exhaustW);

  void calculateStep();

private:
  double thr_;
  double isp_;

};

