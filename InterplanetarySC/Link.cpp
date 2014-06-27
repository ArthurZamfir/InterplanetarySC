#include "Link.h"


Link::Link(std::string classifier)
{
	classi_ = classifier;
	port1_.setClassifier(classifier);
	port2_.setClassifier(classifier);
	p1_ = &port1_;
	p2_ = &port2_;
}


Link::~Link(void)
{
}

void Link::transfer()
{
	if(p1_->isActive() && p2_->isActive())
	{
		double temp = p1_->getValue();
		p1_->setValue(p2_->getValue());
		p2_->setValue(temp);
	}else
	{
		p1_->setValue(0.0);
		p2_->setValue(0.0);
	}

}

Port* Link::getPort()
{
	if(!port1_.isAttached())
	{
		return p1_;
	}
	else if(!port2_.isAttached())
	{
		return p2_;
	}
	else return nullptr;
}

