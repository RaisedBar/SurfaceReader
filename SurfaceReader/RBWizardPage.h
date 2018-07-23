// RBWizardPage.h
// Custom dialog class definition for a page of a multi-page wizard
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef RBWIZARDPAGE_H
#define RBWIZARDPAGE_H

#pragma once

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/sizer.h>

#include "RBEnums.h"
#include "SRConstants.h"
#include <wx/msw/winundef.h> 


class RBWizardPage: public wxDialog
{
protected:
	virtual bool IsValidWizardPage();

void OnBack( wxCommandEvent& event);
	void OnNext( wxCommandEvent& event);
    void OnCancel( wxCommandEvent& event);
void OnFinished( wxCommandEvent& event);

		int nCurrentPageNumber;
		wxPanel * wizPanel;
		
  	// Dialog buttons
wxBoxSizer * hButtonSizer ;
wxButton * BackButton;
wxButton * NextButton;
wxButton * CancelButton;
wxButton * FinishButton;

		DECLARE_EVENT_TABLE()

public:
RBWizardPage(const wxString& title, int nPageNumber, bool blnIsLastPage);
 ~RBWizardPage();

int PageNumber() const;
};


#endif       // RBWIZARDPAGE_H