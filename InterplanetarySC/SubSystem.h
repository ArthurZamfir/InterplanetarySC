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
	std::string sName;
	std::map<std::string,double> input;
	std::map<std::string,double> output;
	std::map<SubSystem*,std::string[]> inputSystems;
	std::map<SubSystem*,std::string[]> outputSystems;
	std::map<std::string,double> currentValues;
	STATUS status;

	std::string getStatusAsString();
public:
	SubSystem(std::string name);
	~SubSystem(void);
	std::string getName();
	void activate();
	void deactivate();
	std::string report();
	void connectSubSystemToInput(SubSystem,std::string[]);
	void connectSubSystemToOutput(SubSystem,std::string[]);
	virtual void setInitialData();
	//virtual void calculateStep();
	//virtual void writeConnectedInputs();
	//virtual void writeConnectedOutputs();
};