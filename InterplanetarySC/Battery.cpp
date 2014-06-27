#include "Battery.h"


Battery::Battery(VESSEL3* vessel,std::string name,double* time,double startCharge,double maxCharge,
				 double chargeRate,double dischargeRate,double efficiency,double maxEfficiency,double temp,
				 double minTemp,double maxTemp,double heatTransferRate,double heatCapacity,double mass)
				 :SubSystem(vessel,name,time)
{
  c_ = startCharge;
  mc_ = maxCharge;
  cr_ = chargeRate;
  dcr_ = dischargeRate;
  eff_ = efficiency;
  meff_ = maxEfficiency;
  t_ = temp;
  tOperating_ = temp;
  minT_ = minTemp;
  maxT_ = maxTemp;
  heatTrans_ = heatTransferRate;
  heatCap_ = heatCapacity;
  m_ = mass;

  attributes_["Energy[J]"] = &c_;
  attributes_["Efficiency[-]"] = &eff_;
  attributes_["Temperature[K]"] = &t_;

  maxAttributes_["Energy[J]"] = maxCharge;
  maxAttributes_["Efficiency[-]"] = maxEfficiency;
  maxAttributes_["Temperature[K]"] = maxTemp;
}

/*
Wenn Aktiv, dann werden an die Verbrucher am Output die von ihnen benötigten Energien gesendet.
Die Energie die zum aufladen am Input benötigt wird, wird nur teilweise in elektrische Energie
umgesetzt, der rest wird als Wärme an die Radiatoren abgegeben.
*/
void Battery::calculateStep()
{
  std::vector<Port*> inputEnergy = collectAllActiveSubSystemsWithClassifier(inputStreams_,"Energy[J]");
  std::vector<Port*> outputEnergy = collectAllActiveSubSystemsWithClassifier(outputStreams_,"Energy[J]");

  std::vector<Port*> inputHeat = collectAllActiveSubSystemsWithClassifier(inputStreams_,"Heat[J]");
  std::vector<Port*> outputHeat = collectAllActiveSubSystemsWithClassifier(outputStreams_,"Heat[J]");

  double inE = getPortValuesSum(inputEnergy);
  double outE = getPortValuesSum(outputEnergy);

  double inH = getPortValuesSum(inputHeat);
  double outH = getPortValuesSum(outputHeat);


  if(isActive())
  {
	//Neue Ladung der Batterie. Auf- und Entladung sind verlustbehaftet.
	double elecE = inE * eff_ - outE * (1/eff_);
	c_ = c_ + elecE;
	//Energie die als Wärme beim Auf- und Entladen entsteht.
	double heat = elecE * (1 - eff_);
	//Temperaturdiffernz durch Wärme, Heater und Radiatoren
	double deltaT = (heat - (outH) + inH) / (m_ * heatCap_);
	//Daraus ergibt sich die Temperatur
	t_ = t_ + deltaT;
	//Wenn diese Temperatur größer oder kleiner ist als die Maximalwerte,
	//dann soll die Batterie keine Energie mehr liefern und auch nicht mehr
	//aufladen können, was im else Teil der Abfrage geregelt wird.
	if( (t_<maxT_) && (t_>minT_) ){
	  //Energie Anforderung an die Input-Energieströme senden.
	  if(c_<mc_){
		writePortValuesEqual(inputEnergy,cr_*(*simTime_));
	  }else{
		writePortValuesEqual(inputEnergy,0.0);
	  }
	  //Energie entsprechend der entladerate an die Verbraucher senden, wenn noch 
	  if(c_>outE){
		std::vector<Port*>::iterator it;
		for (it = outputEnergy.begin(); it!=outputEnergy.end(); ++it){
		  double v = (*it)->getValue();
		  (*it)->setValue(v*dcr_*(*simTime_));
		}
	  }else{
		writePortValuesEqual(outputEnergy,0.0);
	  }
	}else{
	  //Die Batterie soll nicht aufgeladen werden, wenn sie nicht nominell funktioniert
	  writePortValuesEqual(inputEnergy,0.0);
	  //Es soll keine Energie an die Verbruacher gesendet werden, wenn der
	  //Betrieb nicht nominell ist.
	  writePortValuesEqual(outputEnergy,0.0);
	}
	//Wärme Anforderung an die Heater senden sobald Abweichung -3 Kelvin beträgt
	if(t_<tOperating_-3){
	  double heatDemand = (tOperating_-t_) * m_ * heatCap_;
	  writePortValuesEqual(inputHeat,heatDemand);
	  //Radiation auf Null sezten
	  writePortValuesEqual(outputHeat,0.0);
	}
	//Wärme-Abgabe Anforderung an die Radiatoren senden sobald Abweichung von +3 Kelvin
	if(t_>tOperating_+3){
	  double radDemand = (t_-tOperating_) * m_ * heatCap_;
	  writePortValuesEqual(outputHeat,radDemand);
	  //Heat auf Null setzen
	  writePortValuesEqual(inputHeat,0.0);
	}
	if((t_<tOperating_+3) && (t_>tOperating_-3)){
	  //Heat und Radiation auf Null setzen
	  writePortValuesEqual(inputHeat,0.0);
	  writePortValuesEqual(outputHeat,0.0);
	}
  }
  if(!isActive())
  {
	writePortValuesEqual(inputEnergy,0.0);
	writePortValuesEqual(outputEnergy,0.0);
  }

  //Nominal
  if(((c_/mc_) > 0.15 ) && (t_<maxT_) && (t_>minT_))
	setNominal();
  //Anzeige von Warnungen
  if( ((c_/mc_) < 0.15 )  || (t_>(tOperating_+3)) || (t_<(tOperating_-3)))
	setWarning();

  //Anzeige von Fehlern
  if(((c_/mc_) < 0.00001 ) || (t_>maxT_) || (t_<minT_))
	setError();
}