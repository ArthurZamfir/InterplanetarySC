#pragma once
#include <string>
#include "Port.h"

/*
Diese Klasse definiert, wie zwei Subsysteme miteinander verkn�pft werden k�nnen.
Dabei wird dem Link ein sogenannter "Classifier" als String �bergeben, welcher
definiert welche art von Str�men durch die Verbindung flie�t.
Jeder Link hat zwei Ports, welche an jeweils ein Subsystem angeschlossen werden,
wobei die Reihenfolge keine Rolle spielt. Um den Strom durch den Link zu simulieren
werden einfach die Informationen an den Ports vertauscht. Somit k�nnen Str�me in
beide Richtungen flie�en, je nach dem was das entsprechende Subsystem auf den Port
schreibt.
*/
class Link {
public:
  Link(std::string classifier);
  ~Link(void);

  void transfer();
  Port* getPort();

private:
  Port port1_,port2_;
  Port* p1_;
  Port* p2_;
  std::string classi_;
};