#include <string>
#include "Port.h"

class Link
{
private:
	Port p1,p2;
	SubSystem *s1,*s2;
	std::string classi;
public:
	Link(std::string classifier);
	~Link(void);

	void transfer();
	int attach(SubSystem *ss);
};

