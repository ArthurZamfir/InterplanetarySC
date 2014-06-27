#include "Bar.h"
#include "Logger.h"
#include <math.h>
#include <string>
#include <sstream>

Bar::Bar (std::string attribute,DWORD x,DWORD y,double* val,double max)
{
	label_ = attribute;
	xPos_ = x;
	yPos_ = y;
	value_ = val;
	maxValue_ = max;
	lastPercentageValue_ = static_cast<int>(std::floor(*val/max*100 +0.5)); //will always round to the nearest
	firstDraw_ = true;
}

bool Bar::draw (SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	int percentage;
	int x,w,i,len;
	
	if(*value_<0)
		percentage = 0;
	else if(*value_>maxValue_)
		percentage = 100;
	else
		percentage = static_cast<int>(std::floor(*value_/maxValue_*100 +0.5));//will always round to the nearest
	
	//Abfrage ob sich der ganzzahlige Prozentstand im Vergleich zum letzten mal
	//geändert hat. Beim ersten aufrufen wird auf jeden fall geschrieben.
	if((lastPercentageValue_ == percentage) && (firstDraw_ == false))
		return false;
	lastPercentageValue_ = percentage;

	// Remove Artifacts from previous draw
	oapiBlt(tgt,src,xPos_+dx-5,yPos_+dy-5,backX0,backY0+12,barBackWidth + 8,barBackHeight + 10);

	//Copy Black background to target location
	oapiBlt(tgt,src,xPos_+dx,yPos_+dy,barBackX0,barBackY0,barBackWidth,barBackHeight);

	//Overlay with textured map
	oapiBlt(tgt,src,xPos_+dx,yPos_+dy,barX0,barY0,percentage,barHeight);
	
	//Write label with value next to bar
	double v = *value_;
	double mv = maxValue_;
	
	std::stringstream formatted;
	formatted.precision(2);
	formatted.flags(std::ios::fixed);
	formatted << label_ << " :: " << v << " / " << mv;
	
	std::string s;
	s = formatted.str();
	len = s.length();
	for (w = i = 0; i < len; i++)
        w += font_width[s[i]];
	for (i = 0, x = xPos_ + barWidth + 5; i < len; i++)
	{
		w = font_width[s[i]];
		oapiBlt(tgt, src, x+dx, yPos_ + 2 +dy, font_xpos[s[i]]+fontXStart,
			fontYStart, w, fontH);
		x += w;
	}

	return true;

}