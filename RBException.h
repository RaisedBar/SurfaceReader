// RBException.h

#ifndef SURFACE_EXCEPTION_H
#define SURFACE_EXCEPTION_H

#pragma once
#include <string>
#include <wx/msw/winundef.h> 


class RBException
{
public:
    RBException(const std::wstring & msg) 
		: strMsg( msg) 
	{ }

    const wxChar *what() const 
	{ 
		return strMsg.c_str(); 
	}

private:
    std::wstring strMsg;
};
#endif

