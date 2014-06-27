#include "SubSystem.h"

SubSystem::SubSystem(VESSEL3* vessel,std::string name,double* time)
{
  v_=vessel;
  simTime_ = time;
  sName_=name;
  status_ = STATUS_ACTIVE;
}

SubSystem::~SubSystem(void)
{
  delete v_;
}

std::string SubSystem::getName()
{
  return sName_;
}

void SubSystem::activate()
{
  activateAllPorts();

  switch (status_){
  case STATUS_ACTIVE :
	status_ = STATUS_ACTIVE;
	break;

  case STATUS_PASSIVE :
	status_ = STATUS_ACTIVE;
	break;

  case STATUS_ACTIVE_WARNING :
	status_ = STATUS_ACTIVE_WARNING;
	break;

  case STATUS_ACTIVE_ERROR :	
	status_ = STATUS_ACTIVE_ERROR;
	break;

  case STATUS_PASSIVE_WARNING :
	status_ = STATUS_ACTIVE_WARNING;
	break;

  case STATUS_PASSIVE_ERROR :	
	status_ = STATUS_ACTIVE_ERROR;
	break;

  default:
	break;
  }
}

void SubSystem::deactivate()
{
  deactivateAllPorts();

  switch (status_){
  case STATUS_ACTIVE :		
	status_ = STATUS_PASSIVE;
	break;

  case STATUS_PASSIVE :		
	status_ = STATUS_PASSIVE;
	break;

  case STATUS_ACTIVE_WARNING :
	status_ = STATUS_PASSIVE_WARNING;
	break;

  case STATUS_ACTIVE_ERROR :	
	status_ = STATUS_PASSIVE_ERROR;
	break;

  case STATUS_PASSIVE_WARNING :
	status_ = STATUS_PASSIVE_WARNING;
	break;

  case STATUS_PASSIVE_ERROR :	
	status_ = STATUS_PASSIVE_ERROR;
	break;

  default:
	break;
  }
}

std::string SubSystem::report()
{
  std::string s;
  s.append("==== " + sName_ + " ====\n");
  s.append("Status:\t");
  s.append(SubSystem::getStatusAsString());
  s.append("\n");

  for (std::map<std::string,double*>::iterator it = attributes_.begin();it!=attributes_.end();++it)
  {
	s.append(it->first).append("\t\t").append(std::to_string(*(it->second))).append("\n");
  }

  return s;
}

std::string SubSystem::getStatusAsString()
{
  switch (status_){
  case STATUS_ACTIVE:			
	return "STATUS_ACTIVE";

  case STATUS_PASSIVE:			
	return "STATUS_PASSIVE";

  case STATUS_ACTIVE_WARNING:	
	return "STATUS_ACTIVE_WARNING";

  case STATUS_ACTIVE_ERROR:		
	return "STATUS_ACTIVE_ERROR";

  case STATUS_PASSIVE_WARNING:	
	return "STATUS_PASSIVE_WARNING";

  case STATUS_PASSIVE_ERROR:		
	return "STATUS_PASSIVE_ERROR";

  default:				
	return "NOT DEFINED";
  }
}

void SubSystem::connectPortToInput(Port* port)
{
  inputStreams_.insert(std::pair<std::string,Port*>(port->getClassifier(),port));
  port->setAttached();
  port->activate();

}

void SubSystem::connectPortToOutput(Port* port)
{
  outputStreams_.insert(std::pair<std::string,Port*>(port->getClassifier(),port));
  port->setAttached();
  port->activate();
}

std::map<std::string,double*> SubSystem::getAllAttributes()
{
  return attributes_;
}

std::map<std::string,double> SubSystem::getAllMaxAttributes()
{
  return maxAttributes_;
}


void SubSystem::activateAllPorts()
{
  std::multimap<std::string,Port*>::iterator pos;

  for(pos=inputStreams_.begin();pos!=inputStreams_.end();++pos)
  {
	pos->second->activate();
  }

  for(pos=outputStreams_.begin();pos!=outputStreams_.end();++pos)
  {
	pos->second->activate();
  }
}

void SubSystem::deactivateAllPorts()
{
  std::multimap<std::string,Port*>::iterator pos;

  for(pos=inputStreams_.begin();pos!=inputStreams_.end();++pos)
  {
	pos->second->deactivate();
  }

  for(pos=outputStreams_.begin();pos!=outputStreams_.end();++pos)
  {
	pos->second->deactivate();
  }
}

std::vector<Port*> SubSystem::collectAllActiveSubSystemsWithClassifier(std::multimap<std::string,Port*> map,std::string classifier)
{
  std::vector<Port*> ports;

  std::multimap<std::string,Port*>::iterator pos;

  for(pos=map.begin();pos!=map.end();++pos)
  {
	if(pos->first.compare(classifier) == 0)
	{
	  if(pos->second->isActive())
		ports.push_back(pos->second);
	}

  }

  return ports;

}

double SubSystem::getPortValuesSum(std::vector<Port*> ports)
{
  std::vector<Port*>::iterator pos;
  double sum = 0;

  for(pos=ports.begin();pos!=ports.end();++pos)
  {
	sum = sum + (*pos)->getValue();
  }
  return sum;
}

void SubSystem::writePortValuesEqual(std::vector<Port*> ports,double sum)
{
  std::vector<Port*>::iterator pos;
  int size = ports.size();

  if(size != 0)
  {
	for(pos=ports.begin();pos!=ports.end();++pos)
	{
	  (*pos)->setValue(sum/size);
	}
  }

}


void SubSystem::resetAllPortValues()
{
  std::multimap<std::string,Port*>::iterator pos;

  for(pos=inputStreams_.begin();pos!=inputStreams_.end();++pos)
  {
	pos->second->setValue(0.0);
  }

  for(pos=outputStreams_.begin();pos!=outputStreams_.end();++pos)
  {
	pos->second->setValue(0.0);
  }
}

void SubSystem::resetPortValuesWithClassifier(std::string classifier)
{
  std::multimap<std::string,Port*>::iterator pos;

  for(pos=inputStreams_.begin();pos!=inputStreams_.end();++pos)
  {
	if(pos->first.compare(classifier) == 0)
	  pos->second->setValue(0.0);
  }

  for(pos=outputStreams_.begin();pos!=outputStreams_.end();++pos)
  {
	if(pos->first.compare(classifier) == 0)
	  pos->second->setValue(0.0);
  }
}

Status SubSystem::getStatus()
{
  return status_;
}

bool SubSystem::isActive()
{
  if((status_ == STATUS_ACTIVE) || (status_ == STATUS_ACTIVE_WARNING) || (status_ == STATUS_ACTIVE_ERROR))
	return true;
  else
	return false;
}

void SubSystem::setStatus(Status status)
{
  status_ = status;
}

void SubSystem::setNominal(){
  if(isActive())
	setStatus(STATUS_ACTIVE);
  else
	setStatus(STATUS_PASSIVE);
}

void SubSystem::setWarning(){
  if(isActive())
	setStatus(STATUS_ACTIVE_WARNING);
  else
	setStatus(STATUS_PASSIVE_WARNING);
}

void SubSystem::setError(){
  if(isActive())
	setStatus(STATUS_ACTIVE_ERROR);
  else
	setStatus(STATUS_PASSIVE_ERROR);
}