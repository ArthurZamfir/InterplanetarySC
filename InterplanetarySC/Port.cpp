#include "Port.h"


Port::Port()
{
	attached = false;
	v = 0.0;
}


Port::~Port(void)
{
}

double Port::getValue()
{
	return v;
}

void Port::setSubSystem(SubSystem *ss)
{
	s = ss;
	attached = true;
}

SubSystem *Port::getSubSystem()
{
	return s;
}

bool Port::isAttached()
{
	return attached;
}