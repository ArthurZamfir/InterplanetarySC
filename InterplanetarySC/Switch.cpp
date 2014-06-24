#include "Switch.h"


Switch::Switch (VESSEL3 *v): PanelElement (v)
{
  switch_state = 0;
  // this is the base state as shown in the panel background
}

bool Switch::Redraw2D (SURFHANDLE surf)
{
  return false;
}

bool Switch::ProcessMouse2D (int event, int mx, int my)
{
    return false;
}