#include "Thruster.h"
#include "OrbiterAPI.h"
#include "Logger.h"

Thruster::Thruster(VESSEL3 *vessel,std::string name,double *time,VECTOR3 position,
		VECTOR3 direction,double thrust,PROPELLANT_HANDLE propellant,
		double impulse,THGROUP_TYPE thrType[3],int types,double exhaustL,double exhaustW)
	:SubSystem(vessel,name,time)
{
	Logger l("thruster.txt");

	l.logLine("A");
	//Erstellung des Thrusters
	THRUSTER_HANDLE t = vessel->CreateThruster(position,direction,thrust,propellant,impulse);
	l.logLine("X: " + std::to_string(position.x));
	l.logLine("Y: " + std::to_string(position.y));
	l.logLine("Z: " + std::to_string(position.z));
	l.logLine("DX: " + std::to_string(direction.x));
	l.logLine("DY: " + std::to_string(direction.y));
	l.logLine("DZ: " + std::to_string(direction.z));
	l.logLine("Impulse: " + std::to_string(impulse));
	vessel->AddExhaust(t,exhaustL,exhaustW);
	//Zuweisung in die Gruppen, auch mehrere
	//Um keinen dynamischen speicher allokieren zu müssen, wird die
	//Anzahl der Thruster auf 10 begrenzt
	for (int j = 0; j < types; j++)
	{
		if(thrType[j]){
			THRUSTER_HANDLE thGrp[10];
			THGROUP_HANDLE grp = vessel->GetThrusterGroupHandle(thrType[j]);
			l.logLine("Thruster Type: " + std::to_string(thrType[j]));
			int count = vessel->GetGroupThrusterCount(grp);
			l.logLine("Thruster count: " + std::to_string(count));
			for (int i = 0; i < count; i++)
			{
				thGrp[i] = vessel->GetGroupThruster(thrType[j],i);
			}
			thGrp[count] = t;
			l.logLine("Array size: " + std::to_string(sizeof(thGrp)/sizeof(THRUSTER_HANDLE)));
			l.logLine("Thruster Group deleted " + std::to_string(vessel->DelThrusterGroup(grp)));
			vessel->CreateThrusterGroup(thGrp,count + 1,thrType[j]);
			THGROUP_HANDLE debug = vessel->GetThrusterGroupHandle(thrType[j]);
			l.logLine("Thruster count: " + std::to_string(vessel->GetGroupThrusterCount(debug)));
			l.logLine("Group Defined: " + std::to_string(vessel->ThrusterGroupDefined(thrType[j])));
		}


	}
	//THRUSTER_HANDLE thGrp[10];
	//THGROUP_HANDLE grp = vessel->GetThrusterGroupHandle(thrType);
	//l.logLine("Thruster Type: " + std::to_string(thrType));
	//int count = vessel->GetGroupThrusterCount(grp);
	//l.logLine("Thruster count: " + std::to_string(count));
	//for (int i = 0; i < count; i++)
	//{
	//	thGrp[i] = vessel->GetGroupThruster(thrType,i);
	//}
	//thGrp[count] = t;
	//l.logLine("Array size: " + std::to_string(sizeof(thGrp)/sizeof(THRUSTER_HANDLE)));
	//l.logLine("Thruster Group deleted " + std::to_string(vessel->DelThrusterGroup(grp)));
	//vessel->CreateThrusterGroup(thGrp,count + 1,thrType);
	//THGROUP_HANDLE debug = vessel->GetThrusterGroupHandle(thrType);
	//l.logLine("Thruster count: " + std::to_string(vessel->GetGroupThrusterCount(debug)));
	//l.logLine("Group Defined: " + std::to_string(vessel->ThrusterGroupDefined(thrType)));


	//Subsystem initialisieren
	thr = thrust;
	isp = impulse;
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


