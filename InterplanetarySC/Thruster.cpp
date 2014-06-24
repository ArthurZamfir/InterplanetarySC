#include "Thruster.h"

Thruster::Thruster(VESSEL3 *vessel,std::string name,double *time,
				   double thrust,double impulse,std::string thrGroup)
	:SubSystem(vessel,name,time)
{
	thr = thrust;
	isp = impulse;
	group = thrGroup;
	attributes["Thrust[N]"] =&thr;
	attributes["Isp[m/s]"]=&isp;

	maxAttributes["Thrust[N]"] = thrust;
	maxAttributes["Isp[m/s]"] = impulse;
}

void Thruster::calculateStep()
{
	//Stoechiometrische verteilung der Masse
	//2 Teile Wasserstoff auf 8 Teile Sauerstoff
	std::vector<Port*> hydrogenTanks = collectAllActiveSubSystemsWithClassifier(inputStreams,"H2");
	std::vector<Port*> oxygenTanks = collectAllActiveSubSystemsWithClassifier(inputStreams,"O2");
	double hydro = getPortValuesSum(hydrogenTanks);
	double oxy = getPortValuesSum(oxygenTanks);

	double totalMass = thr*(*simTime)/isp;;

	if(operationMode == ACTIVE)
	{
		if(hydro>0 && oxy>0)
		{
			writePortValuesEqual(hydrogenTanks,totalMass*2/10);
			writePortValuesEqual(oxygenTanks,totalMass*8/10);
		}


	}

	//Hier muss noch dran gearbeitet werden, sodass die Tanks auch passiv geschaltet
	//werden kann und sich das System entsprechend verhält. Vielleicht indem der Vorherige
	//wert zwischengespeichert wird.
		
	if(operationMode == PASSIVE)
	{
		totalMass = 0.0;	
		writePortValuesEqual(hydrogenTanks,totalMass*2/10);
		writePortValuesEqual(oxygenTanks,totalMass*8/10);
	}

		
}


