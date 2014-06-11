#include "Port.h"


Port::Port()
{
	attached = false;
	v = 0.0;
}


Port::~Port(void)
{
}

void Port::setClassifier(std::string classifier)
{
	classi = classifier;
}

double Port::getValue()
{
	return v;
}

void Port::setValue(double value)
{
	v = value;
}

void Port::addValue(double value)
{
	v = v + value;
}

//void Port::setSubSystem(SubSystem *ss)
//{
//	s = ss;
//	attached = true;
//}

//SubSystem *Port::getSubSystem()
//{
//	return s;
//}

bool Port::isAttached()
{
	return attached;
}

void Port::setAttached()
{
	attached = true;
}

std::string Port::getClassifier()
{
	return classi;
}