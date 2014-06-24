#include "Link.h"


Link::Link(std::string classifier)
{
	classi = classifier;
	p1.setClassifier(classifier);
	p2.setClassifier(classifier);
	ptrp1 = &p1;
	ptrp2 = &p2;
}


Link::~Link(void)
{
}

void Link::transfer()
{
	if(ptrp1->isActive() && ptrp2->isActive())
	{
		double temp = ptrp1->getValue();
		ptrp1->setValue(ptrp2->getValue());
		ptrp2->setValue(temp);
	}else
	{
		ptrp1->setValue(0.0);
		ptrp2->setValue(0.0);
	}

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

