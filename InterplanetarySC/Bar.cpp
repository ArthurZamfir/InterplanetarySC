#include "Bar.h"
#include "Logger.h"
#include <math.h>
#include <string>

Bar::Bar (std::string attribute,DWORD x,DWORD y,double *val,double max)
{
	label = attribute;
	xPos = x;
	yPos = y;
	value = val;
	maxValue = max;
	lastPercentageValue = std::floor(*val/max*100 +0.5); //will always round to the nearest
}

bool Bar::draw(SURFHANDLE tgt,SURFHANDLE src)
{
	int percentage;
	int x,w,i,len;
	
	if(*value<0)
		percentage = 0;
	else if(*value>maxValue)
		percentage = 100;
	else
		percentage = std::floor(*value/maxValue*100 +0.5);//will always round to the nearest
	
	//Abfrage ob sich der ganzzahlige Prozentstand im Vergleich zum letzten mal
	//geändert hat
	if(lastPercentageValue == percentage)
		return false;
	lastPercentageValue = percentage;

	//Copy Alpha-Channel totarget location
	oapiBlt(tgt,src,xPos,yPos,alphaX,alphaY,alphaW,alphaH);

	//Overlay with textured map
	oapiBlt(tgt,src,xPos,yPos,texX,texY,percentage,texH);
	
	//Write label with value next to bar
	std::string s = label;
	int v = *value;
	int mv = maxValue;
	s.append("::" + std::to_string(v) + "/" + std::to_string(mv));
	len = s.length();
	for (w = i = 0; i < len; i++)
        w += font_width[s[i]];
	for (i = 0, x = xPos + 105; i < len; i++)
	{
		w = font_width[s[i]];
		oapiBlt(tgt, src, x, yPos + 2, font_xpos[s[i]]+fontXStart,
			fontYStart, w, fontH);
		x += w;
	}

}