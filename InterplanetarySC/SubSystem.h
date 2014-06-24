#ifndef __SUBSYSTEM_H_INCLUDED__
#define __SUBSYSTEM_H_INCLUDED__
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
enum OPERATION_MODE{
	ACTIVE,PASSIVE,ACTIVE_WARNING,PASSIVE_WARNING,ACTIVE_ERROR,PASSIVE_ERROR
};

class SubSystem
{
protected:
	VESSEL3 *v;


	double *simTime;


	std::string sName;
	//std::vector<Port*> inputStreams;
	//std::vector<Port*> outputStreams;
	std::multimap<std::string,Port*> inputStreams;
	std::multimap<std::string,Port*> outputStreams;
	//std::map<std::string,double> input;
	//std::map<std::string,double> output;
	//std::map<SubSystem*,std::vector<std::string>> inputSystems;
	//std::map<SubSystem*,std::vector<std::string>> outputSystems;
	std::map<std::string,double*> attributes;
	std::map<std::string,double> maxAttributes;
	OPERATION_MODE operationMode;

	//Hier sollen die input-map und output-map erzeugt werden.
	//Da diese je nach Subsystem unterschiedlich sind werden sie in den
	//abgeleiteten Klassen definiert.
	void activateAllPorts();
	void deactivateAllPorts();
	std::vector<Port*> collectAllActiveSubSystemsWithClassifier(std::multimap<std::string,Port*>,std::string);
	double getPortValuesSum(std::vector<Port*>);
	void writePortValuesEqual(std::vector<Port*>,double);
	void resetAllPortValues();
	void resetPortValuesWithClassifier(std::string);
public:
	SubSystem(VESSEL3 *vessel,std::string,double *time);
	~SubSystem(void);
	std::string getName();
	void activate();
	void deactivate();
	std::string report();
	std::string getStatusAsString();
	void connectPortToInput(Port*);
	void connectPortToOutput(Port*);
	std::map<std::string,double*> getAllAttributes();
	/*
	In dieser Funktion werden auf Basis der Attribute und der 
	*/
	virtual void calculateStep(){}
	/*
	Wenn sich die Werte der inputs und outputs dieses Subsystems durch die Funktion
	calculateStep() geändert haben,dann sollen die neuen Werte den angeschlossenen Subsystemen
	zugewiesen werden. Dies impliziert aber, dass die Information zwischen den Systemen immer
	nur in eine Richtung fließen darf.
	Somit kann ein Input beispielsweise einen Wert "aufgezwungen" bekommen, oder aber sich
	einen Wert vom angeschlossenen Subsystem holen.
	*/
};


/*Ein großes Problem ergibt sich, wenn man an ein Subsystem mehrere Ports
hängen will, die aber den selben Key haben, wie z.B. "Power". In einer Map
wird dieser Key dann ersetzt. Man könnte das System auf eine Multimap umstellen,
das bedeutet aber dass man ein paar hübsche und vor allem schnelle Methoden braucht
um allgemein auf den inhalt dieser Multimap zuzugreifen. Zudem muss beim zugriff
eigentlich auch überprüft werden, ob das angeschlossene Subsystem ACTIVE ist oder nicht
und somit ein Strom fließen kann.*/

#endif