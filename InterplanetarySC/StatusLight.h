#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "SubSystemLib.h"

class StatusLight {
public:
  StatusLight (DWORD x,DWORD y);
  void drawStatus(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,Status status);
  void drawActive(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,bool active);

private:
  DWORD xPos_,yPos_;
};