#include "Port.h"


Port::Port()
{
	attached_ = false;
	active_ = true;
	value_ = 0.0;
}


Port::~Port(void)
{
}

void Port::setClassifier(std::string classifier)
{
  classifier_ = classifier;
}

double Port::getValue()
{
	return value_;
}

void Port::setValue(double value)
{
	value_ = value;
}

bool Port::isAttached()
{
	return attached_;
}

void Port::setAttached()
{
	attached_ = true;
}

std::string Port::getClassifier()
{
	return classifier_;
}

bool Port::isActive()
{
	return active_;
}

void Port::activate()
{
	active_ = true;
}

void Port::deactivate()
{
	active_ = false;
}