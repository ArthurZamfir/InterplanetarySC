#pragma once
#include <string>

/*
Diese Port-Klasse definiert sozusagen die den Stecker/Anschluss eines Links
an ein Subsystem. Auf diesen Port können vom angschlossenen Subsystem 
anschließend neue Werte geschrieben werden. Zudem kann abgefragt werden,
ob der Port bereits belegt ist oder nicht.
*/
class Port {

public:
  Port();
  ~Port(void);

  void setClassifier(std::string classifier);
  double getValue();
  void setValue(double value);
  bool isAttached();
  void setAttached();
  bool isActive();
  void activate();
  void deactivate();
  std::string getClassifier();

private:
  bool attached_;
  bool active_;
  double value_;
  std::string classifier_;
};