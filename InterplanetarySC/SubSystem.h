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
#include "Orbitersdk.h"
#include "Port.h"
#include <map>
#include <string>
#include <vector>

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
	VESSEL3* v;
	std::string sName;
	std::vector<Port*> inputStreams;
	std::vector<Port*> outputStreams;
	//std::map<std::string,double> input;
	//std::map<std::string,double> output;
	//std::map<SubSystem*,std::vector<std::string>> inputSystems;
	//std::map<SubSystem*,std::vector<std::string>> outputSystems;
	std::map<std::string,double> attributes;
	STATUS status;

	//Hier sollen die input-map und output-map erzeugt werden.
	//Da diese je nach Subsystem unterschiedlich sind werden sie in den
	//abgeleiteten Klassen definiert.
	virtual void initializeSystem();
public:
	SubSystem(VESSEL3*,std::string);
	~SubSystem(void);
	std::string getName();
	void activate();
	void deactivate();
	std::string report();
	std::string getStatusAsString();
	void connectPortToInput(Port*);
	void connectPortToOutput(Port*);
	/*
	In dieser Funktion werden auf Basis der Attribute und der 
	*/
	//virtual void calculateStep();
	/*
	Wenn sich die Werte der inputs und outputs dieses Subsystems durch die Funktion
	calculateStep() geändert haben,dann sollen die neuen Werte den angeschlossenen Subsystemen
	zugewiesen werden. Dies impliziert aber, dass die Information zwischen den Systemen immer
	nur in eine Richtung fließen darf.
	Somit kann ein Input beispielsweise einen Wert "aufgezwungen" bekommen, oder aber sich
	einen Wert vom angeschlossenen Subsystem holen.
	*/
	virtual void writeConnectedInputs();
	virtual void writeConnectedOutputs();
};