#include "Port.h"


Port::Port()
{
	attached_ = false;
	active_ = true;
	v_ = 0.0;
}


Port::~Port(void)
{
}

void Port::setClassifier(std::string classifier)
{
	classi_ = classifier;
}

double Port::getValue()
{
	return v_;
}

void Port::setValue(double value)
{
	v_ = value;
}

void Port::addValue(double value)
{
	v_ = v_ + value;
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
	return classi_;
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