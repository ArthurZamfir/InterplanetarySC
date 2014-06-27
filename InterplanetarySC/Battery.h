#pragma once
#include "SubSystem.h"

class Battery: public SubSystem {
public:
  Battery(VESSEL3* vessel,std::string name,double* time,double startCharge,double maxCharge,
	double chargeRate,double dischargeRate,double efficiency,double maxEfficiency,
	double temp,double minTemp,double maxTemp,double heatTransferRate,double heatCapacity,double mass);

  void calculateStep();

private:
  double c_;
  double mc_;
  double cr_;
  double dcr_;
  double eff_;
  double meff_;
  double t_;
  double tOperating_;
  double maxT_;
  double minT_;
  double heatTrans_;
  double heatCap_;
  double m_;
};