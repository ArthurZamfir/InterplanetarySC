/*
Diese header Datei beschreibt den Prinzipiellen Aufbau eines Subsystems.
In Anlehnung an das Beobachter-Muster können sich hier andere Subsysteme
anmelden. Der Unterschied besteht darin, dass ein Subsystem gleichzeitig
Beobachter und Subjekt sein kann, das heißt, dass es sich sowohl bei einem
anderen Subsystem anmelden kann, als auch sich andere Subsysteme bei diesem
anmelden können. Mit dieser Logik können Massen-, Energie- und Informationsströme
auch im Kreis fließen. Eine sich daraus ergebende unendliche Schleife wird 
umgangen, indem auf die aktualisieren() Funktion verzichtet wird und jedes
Subsystem seine (errechneten) Daten in andere Subsysteme speichern kann.
*/
#include <map>
#include <string>

/*
In dieser Auflistung sind die jeweiligen Zustände des Subsystems enthalten.
Je nach dem welchen Zustand das System gerade hat, kann es sich anders verhalten.
Zudem kann damit überprüft werden, ob sich das System Fehlerhaft verhält, bzw
eine Warnung ausgibt, z.B. bei einem geringen Füllstand des Wassertanks.
*/
enum STATUS{
	ACTIVE,PASSIVE,ACTIVE_WARNING,PASSIVE_WARNING,ACTIVE_ERROR,PASSIVE_ERROR
};

class SubSystem
{
private:
	std::string name;
	std::map<std::string,double> input;
	std::map<std::string,double> output;
	std::map<SubSystem,std::string[]> inputSystems;
	std::map<SubSystem,std::string[]> outputSystems;
	std::map<std::string,double> currentValues;
	STATUS status;

public:
	
	SubSystem(std::string name)
	{
		SubSystem::name = name;
	}

	~SubSystem(void)
	{
	}

	std::string getName();

	bool activate();
	bool deactivate();

	std::string report();

	void connectSubSystemToInput(SubSystem,std::string[]);
	void connectSubSystemToOutput(SubSystem,std::string[]);

	virtual void setInitialData();

	virtual void calculateStep();

	virtual void writeConnectedInputs();
	virtual void writeConnectedOutputs();
};

std::string SubSystem::getName()
{
	return name;
}

bool SubSystem::activate()
{
	switch (status){
ACTIVE:				status = ACTIVE;
					break;
PASSIVE:			status = ACTIVE;
					break;
ACTIVE_WARNING:		status = ACTIVE_WARNING;
					break;
ACTIVE_ERROR:		status = ACTIVE_ERROR;
					break;
PASSIVE_WARNING:	status = ACTIVE_WARNING;
					break;
PASSIVE_ERROR:		status = ACTIVE_ERROR;
					break;
	}
}

bool SubSystem::deactivate()
{
	switch (status){
ACTIVE:				status = PASSIVE;
					break;
PASSIVE:			status = PASSIVE;
					break;
ACTIVE_WARNING:		status = PASSIVE_WARNING;
					break;
ACTIVE_ERROR:		status = PASSIVE_ERROR;
					break;
PASSIVE_WARNING:	status = PASSIVE_WARNING;
					break;
PASSIVE_ERROR:		status = PASSIVE_ERROR;
					break;
	}
}

std::string SubSystem::report()
{
	for each (std::string var in currentValues)
	{

	}
}

void SubSystem::connectSubSystemToInput(SubSystem s,std::string val[])
{
	inputSystems.insert(<s,val>);
}

void SubSystem::connectSubSystemToOutput(SubSystem s,std::string val[])
{
	inputSystems.insert(<s,val>);
}