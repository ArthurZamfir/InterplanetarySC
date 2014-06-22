#include "Instrument.h"

class MFDButtonCol: public PanelElement {
public:
  MFDButtonCol (VESSEL3 *v, DWORD _lr);
  bool Redraw2D (SURFHANDLE surf);
  bool ProcessMouse2D (int event, int mx, int my);

private:
  DWORD lr;   // left (0) or right (1) button column
  DWORD xcnt; // central axis of button column in texture
  DWORD ytop; // top edge of label in topmost button
  DWORD dy;   // vertical button spacing
};