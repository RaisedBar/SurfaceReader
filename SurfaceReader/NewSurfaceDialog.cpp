// NewSurfaceDialog.cpp
// Implementation of the NewSurfaceDialog class to provide a custom dialog allowing the user to create a new control surface object
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include <wx/dialog.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/button.h>

#include "NewSurfaceDialog.h"
#include "SurfaceReader.h"


NewSurfaceDialog::NewSurfaceDialog(const wxString & title, boost::shared_ptr <ProtocolCollection> pMyProtocols, bool blnPickProtocol)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	pProtocols = pMyProtocols;
	blnListProtocols = blnPickProtocol;

// wxPanel * myPanel= new wxPanel(this, -1);
  
  // Prompt for a descriptive name for the new surface
wxBoxSizer * hBoxSurface = new wxBoxSizer( wxHORIZONTAL);
  lblSurfaceNamePrompt = new wxStaticText(this, wxID_ANY, wstrSurfaceNamePrompt);
		txtSurfaceName = new wxTextCtrl( this, wxID_ANY);
        hBoxSurface->Add( lblSurfaceNamePrompt, 0, wxEXPAND);  
hBoxSurface->Add( txtSurfaceName, 0, wxEXPAND);

wxBoxSizer * hBoxProtocols = new wxBoxSizer( wxHORIZONTAL);

if (blnListProtocols)
{
// List of available control surface protocols
lblProtocolPrompt = new wxStaticText(this, wxID_ANY, wstrProtocolPrompt);
	lbxSurfaceProtocols = new wxListBox(this, ID_PROTOCOL_LIST_BOX,  wxPoint( -1, -1), wxSize( -1, -1));
		// Populate the list
  ListProtocols();
hBoxProtocols->Add( lblProtocolPrompt, 0, wxEXPAND); 
	hBoxProtocols->Add( lbxSurfaceProtocols, 0, wxEXPAND);  
}  // end if listing protocols  
	
// Buttons
wxBoxSizer * hBoxButtons = new wxBoxSizer( wxHORIZONTAL);
wxButton * HardwareSetupdButton = new wxButton(this, ID_HardwareSetup, wstrHardwareSetupButtonName);
wxButton * DisplaySetupdButton = new wxButton(this, ID_DisplaySetup, wstrDisplaySetupButtonName);
wxButton *AddButton = new wxButton(this, ID_ADD, wstrAddButtonName);
wxButton *NextButton = new wxButton(this, ID_NEXT, wstrNextPageButtonName);
wxButton *CancelButton = new wxButton(this, wxID_CANCEL, wstrCancelButtonName);
  hBoxButtons->Add( HardwareSetupdButton, 0, wxEXPAND);
hBoxButtons->Add( DisplaySetupdButton, 0, wxEXPAND);
hBoxButtons->Add(AddButton, 0, wxEXPAND);
hBoxButtons->Add( NextButton, 0, wxEXPAND);
hBoxButtons->Add(CancelButton, 0, wxEXPAND);
  
		wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBoxSurface);
	
	if (blnListProtocols) 
	{
vBox1->Add( hBoxProtocols);
NextButton->Disable();
		}
else
{
	AddButton->Disable();
}
	
	vBox1->Add( hBoxButtons);
this->SetSizer(vBox1);
	this->Fit();
	vBox1->Fit( this);
	Centre();
}


NewSurfaceDialog::NewSurfaceDialog(const wxString & title, boost::shared_ptr <ProtocolCollection> pMyProtocols, bool blnPickProtocol, MIDISurface * pMySurface)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	pProtocols = pMyProtocols;
	blnListProtocols = blnPickProtocol;
pSurface = pMySurface;

wxPanel * myPanel = new wxPanel(this, -1);
  
  // Prompt for a descriptive name for the new surface
wxBoxSizer * hBoxSurface = new wxBoxSizer( wxHORIZONTAL);
  lblSurfaceNamePrompt = new wxStaticText(myPanel, wxID_ANY, wstrSurfaceNamePrompt);
		txtSurfaceName = new wxTextCtrl( myPanel, wxID_ANY);
        hBoxSurface->Add( lblSurfaceNamePrompt, 0, wxEXPAND);  
hBoxSurface->Add( txtSurfaceName, 0, wxEXPAND);

wxBoxSizer * hBoxProtocols = new wxBoxSizer( wxHORIZONTAL);

if (blnListProtocols)
{
// List of available control surface protocols
lblProtocolPrompt = new wxStaticText(myPanel, wxID_ANY, wstrProtocolPrompt);
	lbxSurfaceProtocols = new wxListBox(this, ID_PROTOCOL_LIST_BOX,  wxPoint( -1, -1), wxSize( -1, -1));
		// Populate the list
  ListProtocols();
hBoxProtocols->Add( lblProtocolPrompt, 0, wxEXPAND); 
	hBoxProtocols->Add( lbxSurfaceProtocols, 0, wxEXPAND);  
}  // end if listing protocols  
	
// Buttons
wxBoxSizer * hBoxButtons = new wxBoxSizer( wxHORIZONTAL);
wxButton * HardwareSetupdButton = new wxButton(myPanel, ID_HardwareSetup, wstrHardwareSetupButtonName);
wxButton * DisplaySetupdButton = new wxButton(myPanel, ID_DisplaySetup, wstrDisplaySetupButtonName);
wxButton *AddButton = new wxButton(myPanel, ID_ADD, wstrAddButtonName);
wxButton *NextButton = new wxButton(myPanel, ID_NEXT, wstrNextPageButtonName);
wxButton *CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName);
  hBoxButtons->Add( HardwareSetupdButton, 0, wxEXPAND);
hBoxButtons->Add( DisplaySetupdButton, 0, wxEXPAND);
hBoxButtons->Add(AddButton, 0, wxEXPAND);
hBoxButtons->Add( NextButton, 0, wxEXPAND);
hBoxButtons->Add(CancelButton, 0, wxEXPAND);
  
		wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBoxSurface);
	
	if (blnListProtocols) 
	{
vBox1->Add( hBoxProtocols);
NextButton->Disable();
		}
else
{
	AddButton->Disable();
}
	
	vBox1->Add( hBoxButtons);
myPanel->SetSizer(vBox1);
	this->Fit();
	vBox1->Fit( myPanel);
	Centre();
}


NewSurfaceDialog::~NewSurfaceDialog()
{}


std::wstring NewSurfaceDialog::SurfaceName() const
{
	return txtSurfaceName->GetValue().ToStdWstring();
}


std::wstring NewSurfaceDialog::ProtocolID()
{
	std::wstring strProtocolID;
	int nSelection = lbxSurfaceProtocols->GetSelection();

	if (nSelection > -1)
	{
		std::wstring wstrProtocolName = lbxSurfaceProtocols->GetString(nSelection).ToStdWstring();
		strProtocolID = pProtocols->FindIDForProtocolName(wstrProtocolName);
	}
	return strProtocolID;
}



bool NewSurfaceDialog::IsValidPortID( int nPortID)
{
return (nPortID >= 0);
}


bool NewSurfaceDialog::IsValidSurfaceName()
{
	return (! txtSurfaceName->GetValue().IsEmpty());
}


bool NewSurfaceDialog::IsValidProtocol()
{
return (lbxSurfaceProtocols->GetSelection() >= 0);
}


bool NewSurfaceDialog::IsValidSurface()
{
if (blnListProtocols == false)
	{
return (IsValidSurfaceName() 
&& IsValidPortID( nDisplay_VirtualMIDIIn) && IsValidPortID( nDisplay_MIDIOut)
&& IsValidPortID( nHardware_MIDIIn) && IsValidPortID( nHardware_VirtualMIDIOut));
}
else
	{
		return (IsValidSurfaceName() && IsValidProtocol()
&& IsValidPortID( nDisplay_VirtualMIDIIn) && IsValidPortID( nDisplay_MIDIOut)
&& IsValidPortID( nHardware_MIDIIn) && IsValidPortID( nHardware_VirtualMIDIOut));
}
}


// Event handlers

		void NewSurfaceDialog::OnDisplaySetup(wxCommandEvent& event)
		{
nDisplay_VirtualMIDIIn = -1;
nDisplay_MIDIOut = -1;
	
	MIDIDialog * dlgDisplaySetup = new MIDIDialog( wstrDisplaySetupTitle);
			if (dlgDisplaySetup->ShowModal() == wxID_OK) 
{
nDisplay_VirtualMIDIIn = dlgDisplaySetup->GetSelectedInput();
nDisplay_MIDIOut = dlgDisplaySetup->GetSelectedOutput();
strDisplay_VirtualMIDIIn = dlgDisplaySetup->GetSelectedInputName();
strDisplay_MIDIOut = dlgDisplaySetup->GetSelectedOutputName();
}  // OK button processing finished
dlgDisplaySetup->Destroy();
		}


		void NewSurfaceDialog::OnHardwareSetup(wxCommandEvent& event)
		{
		    		nHardware_MIDIIn = -1;
nHardware_VirtualMIDIOut = -1;

MIDIDialog * dlgHardwareSetup = new MIDIDialog( wstrHardwareSetupTitle);
			if (dlgHardwareSetup->ShowModal() == wxID_OK) 
{
nHardware_MIDIIn = dlgHardwareSetup->GetSelectedInput();
nHardware_VirtualMIDIOut = dlgHardwareSetup->GetSelectedOutput();
strHardware_MIDIIn = dlgHardwareSetup->GetSelectedInputName();
strHardware_VirtualMIDIOut = dlgHardwareSetup->GetSelectedOutputName();
}  // OK button processing finished
dlgHardwareSetup->Destroy();
}


		void NewSurfaceDialog::OnProtocolSelect(wxCommandEvent& event)
		{
		}


void NewSurfaceDialog::OnAdd(wxCommandEvent& event)
{
	EndModal( wxID_OK);
}

		
	void NewSurfaceDialog::OnNext(wxCommandEvent& event)
{
	EndModal( ID_NEXT);
}


    void NewSurfaceDialog::OnCancel(wxCommandEvent& event)
	{
					EndModal( wxID_CANCEL);
	}


int NewSurfaceDialog::Hardware_MIDIIn() const
{
return nHardware_MIDIIn;
}


int NewSurfaceDialog::Hardware_VirtualMIDIOut() const
{
return nHardware_VirtualMIDIOut;
		}


int NewSurfaceDialog::Display_VirtualMIDIIn() const
{
return 	nDisplay_VirtualMIDIIn;
		}


int NewSurfaceDialog::Display_MIDIOut() const
{
return nDisplay_MIDIOut;
		}


std::wstring NewSurfaceDialog::Hardware_MIDIInName() const
{
return strHardware_MIDIIn;
}


std::wstring NewSurfaceDialog::Hardware_VirtualMIDIOutName() const
{
return strHardware_VirtualMIDIOut;
		}


std::wstring NewSurfaceDialog::Display_VirtualMIDIInName() const
{
return 	strDisplay_VirtualMIDIIn;
		}


std::wstring NewSurfaceDialog::Display_MIDIOutName() const
{
return strDisplay_MIDIOut;
		}


void NewSurfaceDialog::ListProtocols()
{
	std::map <std::wstring, SurfaceProtocol> ::const_iterator it;

		for (it = pProtocols->begin(); it != pProtocols->end(); it++)
	{
		lbxSurfaceProtocols->Append( it->second.GetProtocolName());
	}
}


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(NewSurfaceDialog, wxDialog)
        // List box selection
EVT_LISTBOX( ID_PROTOCOL_LIST_BOX, NewSurfaceDialog::OnProtocolSelect)
		
EVT_BUTTON( ID_DisplaySetup,  NewSurfaceDialog::OnDisplaySetup)
	    EVT_BUTTON( ID_HardwareSetup,                 NewSurfaceDialog::OnHardwareSetup)
EVT_BUTTON(ID_ADD,  NewSurfaceDialog::OnAdd)
EVT_BUTTON(ID_NEXT, NewSurfaceDialog::OnNext)
	    EVT_BUTTON( wxID_CANCEL,                 NewSurfaceDialog::OnCancel)
END_EVENT_TABLE()
	
		