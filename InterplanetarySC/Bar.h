#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "TexturePositions.h"

class Bar{
public:
	Bar (std::string attribute,DWORD x,DWORD y,double *val,double max);
	bool Redraw2D (SURFHANDLE tgt,SURFHANDLE src);
private:
	std::string label;
	double *value;
	double maxValue;
	DWORD xPos,yPos;
};