#include "Link.h"


Link::Link(std::string classifier)
{
	classi = classifier;
}


Link::~Link(void)
{
}

void Link::transfer()
{
	double temp = p1.getValue;
	p1.getValue = p2.getValue;
	p2.getValue = temp;
}

int Link::attach(SubSystem *ss)
{
	if(!p1.isAttached)
	{
		p1.setSubSystem(ss);
		return 0;
	}
	else if(!p2.isAttached)
	{
		p2.setSubSystem(ss);
		return 0;
	}
	else return 1;
}