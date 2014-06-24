#pragma once
#include "Instrument.h"

class Switch{
public:
  Switch (DWORD x,DWORD y);
  bool Redraw2D (SURFHANDLE tgt,SURFHANDLE src);

private:
  int switch_state;
  DWORD xPos,yPos;
};



kfmpofjopsdfpocdfpodmfosd