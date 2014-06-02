#pragma once
#include "SubSystem.h"
#include "Orbitersdk.h"
class Thruster :public SubSystem
{
private:
	VECTOR3		position;
	VECTOR3		direction;
	double		isp;
	std::string group;

public:
	Thruster(void);
	~Thruster(void);
};