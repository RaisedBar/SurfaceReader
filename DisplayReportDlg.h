// DisplayReportDlg.h
// Provides a means of displaying the display data for a control surface on-screen
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef DISPLAY_REPORT_DLG_H
#define DISPLAY_REPORT_DLG_H

#pragma once

#include "RBTextCtrl.h"
#include "SRConstants.h"

#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/button.h>

class DisplayReportDlg: public wxDialog
{
public:
DisplayReportDlg(const wxString& title, const std::wstring wstrReport);
~DisplayReportDlg();

private:
void SetDisplayText();

// Event handlers
void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

wxButton * btnOK;
RBTextCtrl * rbtReport;

DECLARE_EVENT_TABLE()
};

#endif       // DISPLAY_REPORT_DLG_H