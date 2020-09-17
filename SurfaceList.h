// SurfaceList.h
// Derived from wxListBox

#ifndef SURFACE_LIST_H
#define SURFACE_LIST_H

#pragma once

// #include <vector>

#include "DisplayReportDlg.h"
#include "ProtocolCollection.h"
#include "MIDISurface.h"
#include "DisplayDefinition.h"

#include <boost/shared_ptr.hpp>

#include <wx/listbox.h>
#include <wx/msw/winundef.h> 

const int ENTER_KEY_CODE = 13;


class SurfaceList: public wxListBox
{
public:
	SurfaceList( wxWindow* parent, wxWindowID  id, SurfaceVectorType * pSurfaces, boost::shared_ptr<ProtocolCollection> pProtocols, const wxPoint &  pos = wxDefaultPosition, const wxSize &  size = wxDefaultSize, int  n = 0, const wxString choices[ ] = NULL, long  style = 0, const wxValidator &  validator = wxDefaultValidator, const wxString &  name = wxListBoxNameStr) 
:wxListBox( parent, id, pos, size, n, choices, style, validator, name) 
	{
pMySurfaces = pSurfaces;
pMyProtocols = pProtocols;
	};

~SurfaceList()
{};

void ShowSurfaceReport();

private:
void OnEnterKey(wxKeyEvent& event);

SurfaceVectorType * pMySurfaces;
boost::shared_ptr<ProtocolCollection> pMyProtocols;

	DECLARE_EVENT_TABLE()
// end class
};
	#endif

