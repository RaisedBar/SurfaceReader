// RBWizardPage.cpp
// Implementation of the RBWizardPage Dialog class 
// Purpose:  A wizard to allow the user to create a new page for a multi-page wizard
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "RBWizardPage.h"


RBWizardPage::RBWizardPage(const wxString & title, int nPageNumber, bool blnIsLastPage)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
nCurrentPageNumber = nPageNumber;
wizPanel = new wxPanel(this, -1);
	// Dialog buttons
hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
BackButton = new wxButton(wizPanel, ID_BACK, wstrPreviousPageButtonName);
NextButton = new wxButton(wizPanel, ID_NEXT, wstrNextPageButtonName);
FinishButton = new wxButton(wizPanel, ID_FINISHED, wstrFinishedWizardButtonName);
CancelButton = new wxButton(wizPanel, wxID_CANCEL, wstrCancelWizardButtonName);

if (nCurrentPageNumber == 1)
{
BackButton->Disable();
}
else
{
	BackButton->Enable();
}

if (blnIsLastPage)
{
NextButton->Disable();
FinishButton->Enable();
FinishButton->SetDefault();
}
else
{
FinishButton->Disable();
NextButton->Enable();
NextButton->SetDefault();
}

hButtonSizer->Add( BackButton, 0, wxEXPAND);
hButtonSizer->Add( NextButton, 0, wxEXPAND);
hButtonSizer->Add( FinishButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);
}


RBWizardPage:: ~RBWizardPage()
{}


bool RBWizardPage::IsValidWizardPage()
{
return true;
}


	int RBWizardPage::PageNumber() const
	{
		return nCurrentPageNumber;
}


// Event handlers

				void RBWizardPage::OnBack(wxCommandEvent& event)
		{
EndModal( ID_BACK);
}


void RBWizardPage::OnNext(wxCommandEvent& event)
		{
EndModal( ID_NEXT);
}


    void RBWizardPage::OnCancel(wxCommandEvent& event)
	{
					EndModal( wxID_CANCEL);
	}


void RBWizardPage::OnFinished( wxCommandEvent& event)
	{
					EndModal( ID_FINISHED);
	}


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(RBWizardPage, wxDialog)
        EVT_BUTTON( ID_BACK, RBWizardPage::OnBack)
	    EVT_BUTTON( ID_NEXT, RBWizardPage::OnNext)
EVT_BUTTON(wxID_CANCEL, RBWizardPage::OnCancel)
	    EVT_BUTTON( ID_FINISHED, RBWizardPage::OnFinished)
	END_EVENT_TABLE()
	
