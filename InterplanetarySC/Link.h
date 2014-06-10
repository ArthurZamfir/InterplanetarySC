#include <string>
#include "Port.h"

class Link
{
private:
	Port p1,p2;
	Port *ptrp1,*ptrp2;
	std::string classi;
public:
	Link(std::string classifier);
	~Link(void);

	void transfer();
	Port* getPort();
};

