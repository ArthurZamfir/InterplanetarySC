#include "SubSystem.h"


SubSystem::SubSystem(std::string name)
{
	sName = name;
	status = ACTIVE;
}

SubSystem::~SubSystem(void)
{
}

std::string SubSystem::getStatusAsString()
{
	switch (status){
case ACTIVE:			return "ACTIVE";
case PASSIVE:			return "PASSIVE";
case ACTIVE_WARNING:	return "ACTIVE_WARNING";
case ACTIVE_ERROR:		return "ACTIVE_ERROR";
case PASSIVE_WARNING:	return "PASSIVE_WARNING";
case PASSIVE_ERROR:		return "PASSIVE_ERROR";
default:				return "NOT DEFINED";
	}
}

std::string SubSystem::getName()
{
	return sName;
}

void SubSystem::activate()
{
	switch (status){
case ACTIVE:		status = ACTIVE;
					break;
case PASSIVE:		status = ACTIVE;
					break;
case ACTIVE_WARNING:status = ACTIVE_WARNING;
					break;
case ACTIVE_ERROR:	status = ACTIVE_ERROR;
					break;
case PASSIVE_WARNING:status = ACTIVE_WARNING;
					break;
case PASSIVE_ERROR:	status = ACTIVE_ERROR;
					break;
default:			break;
	}
}

void SubSystem::deactivate()
{
	switch (status){
case ACTIVE:		status = PASSIVE;
					break;
case PASSIVE:		status = PASSIVE;
					break;
case ACTIVE_WARNING:status = PASSIVE_WARNING;
					break;
case ACTIVE_ERROR:	status = PASSIVE_ERROR;
					break;
case PASSIVE_WARNING:status = PASSIVE_WARNING;
					break;
case PASSIVE_ERROR:	status = PASSIVE_ERROR;
					break;
default:			break;
	}
}

std::string SubSystem::report()
{
	std::string s;
	s.append("==== " + sName + " ====\n");
	s.append("Status:\t");
	s.append(SubSystem::getStatusAsString());
	s.append("\n");

	for (std::map<std::string,double>::iterator it = currentValues.begin();it!=currentValues.end();++it)
	{
		s.append(it->first).append("\t\t").append(std::to_string(it->second)).append("\n");
	}

	return s;
}

void SubSystem::setInitialData()
{
	currentValues["O2"] = 500.0;
	currentValues["H2"] = 1000.0;
	currentValues["H2O"] = 500.0;
}