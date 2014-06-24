#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "TexturePositions.h"

class Bar{
public:
	Bar (std::string attribute,DWORD x,DWORD y,double *val,double max);
	bool draw (SURFHANDLE tgt,SURFHANDLE src);
private:
	std::string label;
	double *value;
	double maxValue;
	int lastPercentageValue;
	DWORD xPos,yPos;
};