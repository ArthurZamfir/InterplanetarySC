#include "Thruster.h"
#include "OrbiterAPI.h"
#include "Logger.h"

/*

*/
Thruster::Thruster(VESSEL3* vessel,std::string name,double* time,VECTOR3 position,
		VECTOR3 direction,double thrust,PROPELLANT_HANDLE propellant,
		double impulse,THGROUP_TYPE thrType[3],int types,double exhaustL,double exhaustW)
	:SubSystem(vessel,name,time) {

	//Erstellung des Thrusters
	THRUSTER_HANDLE t = vessel->CreateThruster(position,direction,thrust,propellant,impulse);
	vessel->AddExhaust(t,exhaustL,exhaustW);
	//Zuweisung in die Gruppen, auch mehrere
	//Um keinen dynamischen speicher allokieren zu müssen, wird die
	//Anzahl der Thruster auf 10 begrenzt
	for (int j = 0; j < types; j++)	{
		if (thrType[j]) {
			THRUSTER_HANDLE thGrp[10];
			THGROUP_HANDLE grp = vessel->GetThrusterGroupHandle(thrType[j]);
			int count = vessel->GetGroupThrusterCount(grp);
			for (int i = 0; i < count; i++)	{
				thGrp[i] = vessel->GetGroupThruster(thrType[j],i);
			}
			thGrp[count] = t;
			vessel->CreateThrusterGroup(thGrp,count + 1,thrType[j]);
			THGROUP_HANDLE debug = vessel->GetThrusterGroupHandle(thrType[j]);
		}
	}

	//Subsystem initialisieren
	thr_ = thrust;
	isp_ = impulse;
	attributes_["Thrust[N]"] =&thr_;
	attributes_["Isp[m/s]"]=&isp_;

	maxAttributes_["Thrust[N]"] = thrust;
	maxAttributes_["Isp[m/s]"] = impulse;
}


void Thruster::calculateStep()
{
	
	std::vector<Port*> hydrogenTanks = collectAllActiveSubSystemsWithClassifier(inputStreams_,"H2");
	std::vector<Port*> oxygenTanks = collectAllActiveSubSystemsWithClassifier(inputStreams_,"O2");

	//Constraints
	double hydroIn = getPortValuesSum(hydrogenTanks);
	double oxyIn = getPortValuesSum(oxygenTanks);

	double totalMass = thr_*(*simTime_)/isp_;
	bool resourcesAvailable = ( (hydroIn > 0) && (oxyIn > 0) );

	// Stoechiometrische verteilung der Masse
	// 2 Teile Wasserstoff auf 8 Teile Sauerstoff
	if(isActive()) {
		if (resourcesAvailable) {
			writePortValuesEqual(hydrogenTanks,totalMass*2/10);
			writePortValuesEqual(oxygenTanks,totalMass*8/10);
		}
	}

	if(!isActive()) {
		totalMass = 0.0;	
		writePortValuesEqual(hydrogenTanks,totalMass*2/10);
		writePortValuesEqual(oxygenTanks,totalMass*8/10);
	}
	
}


