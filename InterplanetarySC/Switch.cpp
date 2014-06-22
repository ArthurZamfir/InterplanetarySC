#include "Switch.h"
#include "Logger.h"

const int MFD_font_xpos[256]= { // MFD label font: character x-offsets
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,642/*+*/,0,661/*-*/,0,596/* / */,
	492/*0*/,501/*1*/,510/*2*/,520/*3*/,529/*4*/,538/*5*/,547/*6*/,556/*7*/,565/*8*/,575/*9*/,627/*:*/,621/*;*/,602/*<*/,652/*=*/,612/*>*/,0,
	0,1/*A*/,11/*B*/,21/*C*/,32/*D*/,43/*E*/,54/*F*/,63/*G*/,75/*H*/,86/*I*/,92/*J*/,101/*K*/,111/*L*/,120/*M*/,132/*N*/,143/*O*/,
	155/*P*/,165/*Q*/,176/*R*/,187/*S*/,198/*T*/,207/*U*/,218/*V*/,229/*W*/,242/*X*/,253/*Y*/,263/*Z*/,0,0,0,0,0,
	0,273/*a*/,282/*b*/,291/*c*/,299/*d*/,309/*e*/,318/*f*/,324/*g*/,333/*h*/,342/*i*/,347/*j*/,353/*k*/,362/*l*/,367/*m*/,380/*n*/,389/*o*/,
	398/*p*/,407/*q*/,416/*r*/,423/*s*/,431/*t*/,438/*u*/,447/*v*/,456/*w*/,466/*x*/,475/*y*/,483/*z*/,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
const int MFD_font_width[256] =  { // MFD label font: character widths
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,6/*+*/,0,4/*-*/,0,4/* / */,
	6/*0*/,6/*1*/,6/*2*/,6/*3*/,6/*4*/,6/*5*/,6/*6*/,6/*7*/,6/*8*/,6/*9*/,2/*:*/,2/*;*/,6/*<*/,6/*=*/,6/*>*/,0,
	0,8/*A*/,7/*B*/,7/*C*/,7/*D*/,6/*E*/,6/*F*/,8/*G*/,7/*H*/,2/*I*/,5/*J*/,7/*K*/,6/*L*/,8/*M*/,7/*N*/,8/*O*/,
	6/*P*/,8/*Q*/,7/*R*/,7/*S*/,6/*T*/,7/*U*/,8/*V*/,11/*W*/,7/*X*/,8/*Y*/,7/*Z*/,0,0,0,0,0,
	0,6/*a*/,6/*b*/,6/*c*/,6/*d*/,6/*e*/,4/*f*/,6/*g*/,6/*h*/,2/*i*/,3/*j*/,5/*k*/,2/*l*/,8/*m*/,6/*n*/,6/*o*/,
	6/*p*/,6/*q*/,4/*r*/,6/*s*/,4/*t*/,6/*u*/,6/*v*/,9/*w*/,6/*x*/,6/*y*/,6/*z*/,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
const int MFD_font_ypos = 22;
const int MFD_font_height = 12;

MFDButtonCol::MFDButtonCol (VESSEL3 *v, DWORD _lr)
  : PanelElement (v)
{
  lr = _lr;
  xcnt = 214+150 + lr*360;  // adjust according to geometry
  ytop = 32;          // same here
  dy   = 60;           // same here
}

bool MFDButtonCol::Redraw2D (SURFHANDLE surf)
{
 const int btnw = 16; // button label area width
  const int btnh = MFD_font_height; // button label area height
  int btn, x, len, i, w;
  const char *label;

  int  blank_btn_x = 171;
  int blank_btn_y = 70;

  for (btn = 0; btn < 6; btn++) {
    // blank buttons
    oapiBlt (surf, surf, xcnt-btnw/2, ytop+dy*btn,
      blank_btn_x, blank_btn_y, blank_btn_x+btnw, blank_btn_y+btnh);

    // write labels
    if (label = oapiMFDButtonLabel (MFD_LEFT, btn+lr*6)) {
      len = strlen(label);
      for (w = i = 0; i < len; i++)
        w += MFD_font_width[label[i]];
      for (i = 0, x = xcnt-w/2; i < len; i++) {
        w = MFD_font_width[label[i]];
        if (w) {
          oapiBlt (surf, surf, x, ytop+dy*btn, MFD_font_xpos[label[i]],
            MFD_font_ypos, w, MFD_font_height);
            x += w;
        }
      }
    } else break;
  }
  return false;
}

bool MFDButtonCol::ProcessMouse2D (int event, int mx, int my)
{
	Logger  l("MFD.txt");
	std::string s = std::to_string(event)  +" - " + std::to_string(mx)  +" - "+ std::to_string(my);
	l.logLine(std::to_string(event));

  if (my % dy < 12) {
    int btn = my/dy + lr*6;
    oapiProcessMFDButton (MFD_LEFT, btn, event);
    return true;
  } else
    return false;
}