// ==============================================================
//                ORBITER MODULE: DeltaGlider
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2008 Martin Schweiger
//                   All rights reserved
//
// Instrument.cpp
// Base class for panel and VC instrument visualisations
// ==============================================================

#include "Instrument.h"
#include "Orbitersdk.h"

PanelElement::PanelElement (VESSEL3 *v)
{
	vessel = v;
	grp = 0;
	vtxofs = 0;
	mesh = 0;
	gidx = 0;
}

PanelElement::~PanelElement ()
{
}

void PanelElement::Reset2D ()
{
}

bool PanelElement::Redraw2D (SURFHANDLE tgt, SURFHANDLE src)
{
	return false;
}

bool PanelElement::RedrawVC (DEVMESHHANDLE hMesh, SURFHANDLE surf)
{
	return false;
}

bool PanelElement::ProcessMouse2D (int event, int mx, int my)
{
	return false;
}

bool PanelElement::ProcessMouseVC (int event, VECTOR3 &p)
{
	return false;
}

// ==============================================================

void PanelElement::AddGeometry (MESHHANDLE hMesh, DWORD grpidx, const NTVERTEX *vtx, DWORD nvtx, const WORD *idx, DWORD nidx)
{
	mesh = hMesh;
	gidx = grpidx;
	grp  = oapiMeshGroup (hMesh, grpidx);
	vtxofs = grp->nVtx;
	oapiAddMeshGroupBlock (hMesh, grpidx, vtx, nvtx, idx, nidx);
}

// ==============================================================

char *PanelElement::DispStr (double dist, int precision)
{
	static char strbuf[32];
	double absd = fabs (dist);
	if (absd < 1e4) {
		if      (absd < 1e3)  sprintf (strbuf, "% 6.*f ", precision-3, dist);
		else                  sprintf (strbuf, "% 0.*fk", precision-1, dist*1e-3);
	} else if (absd < 1e7) {
		if      (absd < 1e5)  sprintf (strbuf, "% 0.*fk", precision-2, dist*1e-3);
		else if (absd < 1e6)  sprintf (strbuf, "% 0.*fk", precision-3, dist*1e-3);
		else                  sprintf (strbuf, "% 0.*fM", precision-1, dist*1e-6);
	} else if (absd < 1e10) {
		if      (absd < 1e8)  sprintf (strbuf, "% 0.*fM", precision-2, dist*1e-6);
		else if (absd < 1e9)  sprintf (strbuf, "% 0.*fM", precision-3, dist*1e-6);
		else                  sprintf (strbuf, "% 0.*fG", precision-1, dist*1e-9);
	} else {
		if      (absd < 1e11) sprintf (strbuf, "% 0.*fG", precision-2, dist*1e-9);
		else if (absd < 1e12) sprintf (strbuf, "% 0.*fG", precision-3, dist*1e-9);
		else                  strcpy (strbuf, "--.--");
	}
	return strbuf;
}
