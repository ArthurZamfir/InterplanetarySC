#include "SubSystem.h"
class Thruster:public SubSystem
{
public:

	Thruster(VESSEL3* vessel,std::string name,double thrust,double isp,std::string group);

};

