#include "Link.h"


Link::Link(std::string classifier)
{
	classi = classifier;
	p1.setClassifier(classifier);
	p2.setClassifier(classifier);
	*ptrp1 = p1;
	*ptrp2 = p2;
}


Link::~Link(void)
{
}

void Link::transfer()
{
	double temp = p1.getValue();
	p1.setValue(p2.getValue());
	p2.setValue(temp);
}

Port* Link::getPort()
{
	if(!p1.isAttached())
	{
		return ptrp1;
	}
	else if(!p2.isAttached())
	{
		return ptrp2;
	}
	else return nullptr;
}

