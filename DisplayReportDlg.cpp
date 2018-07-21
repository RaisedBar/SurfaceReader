// DisplayReportDlg.cpp
// Purpose:  
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "DisplayReportDlg.h"


DisplayReportDlg::DisplayReportDlg(const wxString & title, const std::wstring wstrReport)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	wxPanel * myPanel = new wxPanel(this, -1);

	wxBoxSizer * hBoxReport= new wxBoxSizer( wxHORIZONTAL);
		  rbtReport = new RBTextCtrl( myPanel, wxID_ANY, wstrReport, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
hBoxReport->Add( rbtReport, 0, wxEXPAND);
	
wxBoxSizer * hBoxButtons = new wxBoxSizer( wxHORIZONTAL);
		  btnOK = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnOK->SetDefault();
		  hBoxButtons->Add( btnOK, 0, wxEXPAND);
	
wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxReport);		
vBox1->Add( hBoxButtons);

SetSizerAndFit(vBox1);
}


DisplayReportDlg::~DisplayReportDlg()
{}


// Event handlers

void DisplayReportDlg::OnOK(wxCommandEvent& event)
{
EndModal( wxID_OK);
}


		// We need to implement an event table in which the events received by an ActionPicker are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DisplayReportDlg, wxDialog)
EVT_BUTTON(wxID_OK,  DisplayReportDlg::OnOK)
	    END_EVENT_TABLE()
	
		