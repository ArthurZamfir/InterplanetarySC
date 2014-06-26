#include "Battery.h"


Battery::Battery(VESSEL3 *vessel,std::string name,double *time,double startCharge,double maxCharge,
		double chargeRate,double dischargeRate,double efficiency,double maxEfficiency,double temp,
		double minTemp,double maxTemp,double heatTransferRate,double heatCapacity,double mass)
				 :SubSystem(vessel,name,time)
{
	c = startCharge;
	mc = maxCharge;
	cr = chargeRate;
	dcr = dischargeRate;
	eff = efficiency;
	meff = maxEfficiency;
	t = temp;
	tOperating = temp;
	minT = minTemp;
	maxT = maxTemp;
	heatTrans = heatTransferRate;
	heatCap = heatCapacity;
	m = mass;

	attributes["Energy[J]"] = &c;
	attributes["Efficiency[-]"] = &eff;
	attributes["Temperature[K]"] = &t;

	maxAttributes["Energy[J]"] = maxCharge;
	maxAttributes["Efficiency[-]"] = maxEfficiency;
	maxAttributes["Temperature[K]"] = maxTemp;
}

/*
Wenn Aktiv, dann werden an die Verbrucher am Output die von ihnen benötigten Energien gesendet.
Die Energie die zum aufladen am Input benötigt wird, wird nur teilweise in elektrische Energie
umgesetzt, der rest wird als Wärme an die Radiatoren abgegeben.
*/
void Battery::calculateStep()
{
	std::vector<Port*> inputEnergy = collectAllActiveSubSystemsWithClassifier(inputStreams,"Energy[J]");
	std::vector<Port*> outputEnergy = collectAllActiveSubSystemsWithClassifier(outputStreams,"Energy[J]");

	std::vector<Port*> inputHeat = collectAllActiveSubSystemsWithClassifier(inputStreams,"Heat[J]");
	std::vector<Port*> outputHeat = collectAllActiveSubSystemsWithClassifier(outputStreams,"Heat[J]");

	double inE = getPortValuesSum(inputEnergy);
	double outE = getPortValuesSum(outputEnergy);

	double inH = getPortValuesSum(inputHeat);
	double outH = getPortValuesSum(outputHeat);


	if(isActive())
	{
		//Neue Ladung der Batterie. Auf- und Entladung sind verlustbehaftet.
		double elecE = inE * eff - outE * (1/eff);
		c = c + elecE;
		//Energie die als Wärme beim Auf- und Entladen entsteht.
		double heat = elecE * (1 - eff);
		//Temperaturdiffernz durch Wärme, Heater und Radiatoren
		double deltaT = (heat - (outH) + inH) / (m * heatCap);
		//Daraus ergibt sich die Temperatur
		t = t + deltaT;
		//Wenn diese Temperatur größer oder kleiner ist als die Maximalwerte,
		//dann soll die Batterie keine Energie mehr liefern und auch nicht mehr
		//aufladen können, was im else Teil der Abfrage geregelt wird.
		if( (t<maxT) && (t>minT) ){
			//Energie Anforderung an die Input-Energieströme senden.
			if(c<mc){
				writePortValuesEqual(inputEnergy,cr*(*simTime));
			}else{
				writePortValuesEqual(inputEnergy,0.0);
			}
			//Energie entsprechend der entladerate an die Verbraucher senden, wenn noch 
			if(c>outE){
				std::vector<Port*>::iterator it;
				for (it = outputEnergy.begin(); it!=outputEnergy.end(); ++it){
					double v = (*it)->getValue();
					(*it)->setValue(v*dcr*(*simTime));
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
		if(t<tOperating-3){
			double heatDemand = (tOperating-t) * m * heatCap;
			writePortValuesEqual(inputHeat,heatDemand);
			//Radiation auf Null sezten
			writePortValuesEqual(outputHeat,0.0);
		}
		//Wärme-Abgabe Anforderung an die Radiatoren senden sobald Abweichung von +3 Kelvin
		if(t>tOperating+3){
			double radDemand = (t-tOperating) * m * heatCap;
			writePortValuesEqual(outputHeat,radDemand);
			//Heat auf Null setzen
			writePortValuesEqual(inputHeat,0.0);
		}
		if((t<tOperating+3) && (t>tOperating-3)){
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
	if(((c/mc) > 0.15 ) && (t<maxT) && (t>minT))
		setNominal();
	//Anzeige von Warnungen
	if( ((c/mc) < 0.15 )  || (t>(tOperating+3)) || (t<(tOperating-3)))
		setWarning();

	//Anzeige von Fehlern
	if(((c/mc) < 0.00001 ) || (t>maxT) || (t<minT))
		setError();
}