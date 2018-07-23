// RBTextCtrl.h
// Text control that allows itself to show up in the Tab order when using the read-only style
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef RBTextCtrl_H
#define RBTextCtrl_H

#pragma once

#include <wx/textctrl.h>
#include <wx/msw/winundef.h> 

class RBTextCtrl: public wxTextCtrl
{
public:
	RBTextCtrl( wxWindow * parent, wxWindowID id, const wxString &value = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &  size = wxDefaultSize, long  style = wxTE_MULTILINE, const wxValidator &  validator = wxDefaultValidator, const wxString &  name = wxTextCtrlNameStr)
	:wxTextCtrl( parent, id, value, pos, size, style,   validator, name)
	{};

	
	~RBTextCtrl()
	{};
	
bool AcceptsFocusFromKeyboard  (  ) const 
{
return true;
}
};


#endif       // RBTextCtrl_H