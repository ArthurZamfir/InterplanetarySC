#pragma once
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
enum Status{
  STATUS_ACTIVE,STATUS_PASSIVE,STATUS_ACTIVE_WARNING,STATUS_PASSIVE_WARNING,
  STATUS_ACTIVE_ERROR,STATUS_PASSIVE_ERROR
};

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
class SubSystem {
public:
  SubSystem(VESSEL3* vessel,std::string,double* time);
  ~SubSystem(void);

  std::string getName();
  void activate();
  void deactivate();
  std::string report();
  std::string getStatusAsString();
  void connectPortToInput(Port*);
  void connectPortToOutput(Port*);
  std::map<std::string,double*> getAllAttributes();
  std::map<std::string,double> getAllMaxAttributes();
  Status getStatus();
  void setStatus(Status status);
  void setWarning();
  void setNominal();
  void setError();
  bool isActive();
  virtual void calculateStep(){}
  /*
  Wenn sich die Werte der inputs und outputs dieses Subsystems durch die Funktion
  calculateStep() geändert haben,dann sollen die neuen Werte den angeschlossenen Subsystemen
  zugewiesen werden. Dies impliziert aber, dass die Information zwischen den Systemen immer
  nur in eine Richtung fließen darf.
  Somit kann ein Input beispielsweise einen Wert "aufgezwungen" bekommen, oder aber sich
  einen Wert vom angeschlossenen Subsystem holen.
  */
protected:
  VESSEL3* vessel_;
  double* simTime_;
  std::string name_;
  std::multimap<std::string,Port*> inputStreams_;
  std::multimap<std::string,Port*> outputStreams_;
  std::map<std::string,double*> attributes_;
  std::map<std::string,double> maxAttributes_;
  Status status_;

  std::vector<Port*> collectAllActiveSubSystemsWithClassifier(std::multimap<std::string,Port*>,std::string);
  void activateAllPorts();
  void deactivateAllPorts();
  double getPortValuesSum(std::vector<Port*>);
  void writePortValuesEqual(std::vector<Port*>,double);
  void resetAllPortValues();
  void resetPortValuesWithClassifier(std::string);
};