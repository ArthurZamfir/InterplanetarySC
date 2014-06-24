#include "Bar.h"
#include "Logger.h"
#include <math.h>
#include <string>

Bar::Bar (VESSEL3 *v,std::string attribute,DWORD x,DWORD y,double *val,double max)
  : PanelElement (v)
{
	label = attribute;
	xPos = x;
	yPos = y;
	value = val;
	maxValue = max;
	lastRefreshTime = 0;
}

//The source-file should be instruments.dds
bool Bar::Redraw2D(SURFHANDLE tgt,SURFHANDLE src)
{
	//Diese Funktion sorgt dafür, dass nur einmal pro sekunde
	//aktualisiert wird.
	if(!(oapiGetSysTime()-1.0>lastRefreshTime))
	{
		return false;
	}

	lastRefreshTime = oapiGetSysTime();

	int percentage;
	int x,w,i,len;
	
	if(*value<0)
		percentage = 0;
	else if(*value>maxValue)
		percentage = 100;
	else
		percentage = std::floor(*value/maxValue*100 +0.5);//will always round to the nearest

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

	return false;

}