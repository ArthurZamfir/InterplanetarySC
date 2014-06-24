#ifndef __PORT_H_INCLUDED__
#define __PORT_H_INCLUDED__
/*
Diese Port-Klasse definiert sozusagen die den Stecker/Anschluss eines Links
an ein Subsystem. Auf diesen Port können vom angschlossenen Subsystem 
anschließend neue Werte geschrieben werden. Zudem kann abgefragt werden,
ob der Port bereits belegt ist oder nicht.
*/
#include <string>
class Port
{
private:
	bool attached;
	bool active;
	double v;
	std::string classi;
public:
	Port();
	~Port(void);

	void setClassifier(std::string classifier);
	double getValue();
	void setValue(double value);
	void addValue(double value);
	bool isAttached();
	void setAttached();
	bool isActive();
	void activate();
	void deactivate();
	std::string getClassifier();
};

#endif