#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "TexturePositions.h"

class Bar {
public:
	Bar (std::string attribute,DWORD x,DWORD y,double* val,double max);
	bool draw (SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy);
private:
	std::string label_;
	double* value_;
	double maxValue_;
	int lastPercentageValue_;
	DWORD xPos_,yPos_;
	bool firstDraw_;
};