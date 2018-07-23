// RBException.h

// #define _SCL_SECURE_NO_WARNINGS

#ifndef SURFACE_EXCEPTION_H
#define SURFACE_EXCEPTION_H

#pragma once


#include <wx/string.h>
#include <wx/msw/winundef.h> 


class RBException
{
public:
    RBException(const wxString & msg) 
		: wxstrMsg( msg) 
	{ }

    const wxChar *what() const 
	{ 
		return wxstrMsg.c_str(); 
	}

private:
    wxString wxstrMsg;
};
#endif

