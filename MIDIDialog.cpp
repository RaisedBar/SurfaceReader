// MIDIDialog.cpp
// Implementation of the MIDIDialog class to provide a custom dialog allowing the user to select MIDI ports
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "MIDIDialog.h"


MIDIDialog::MIDIDialog(const wxString & title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize( MIDI_DIALOG_HEIGHT, MIDI_DIALOG_WIDTH))
{
// Constructor 1
// wxMessageBox( wxT( "1"), wstrErrorTitle, wxOK | wxICON_ERROR);

	// 	myVP = None;
	myVirtualIn = false;
	myVirtualOut = false;
	
/*
 try
{
myMIDIIn = new RtMidiIn();
myMIDIOut = new RtMidiOut();
} 
catch ( RtMidiError &error ) 
{
error.printMessage();
}
*/
	
wxPanel * myPanel = new wxPanel(this, -1);

wxBoxSizer *hBox1 = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hBox2 = new wxBoxSizer(wxHORIZONTAL);  

wxBoxSizer *vBox3 = new wxBoxSizer(wxVERTICAL);
  
  // List of MIDI input ports
  wxStaticBox *lblMIDIInPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIInPrompt);
lbxMIDIIn = new wxListBox(myPanel, ID_MIDI_IN_LIST_BOX);
ListMIDIInPorts();
 
// List of MIDI output ports
wxStaticBox *lblMIDIOutPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIOutPrompt);
lbxMIDIOut = new wxListBox(myPanel, ID_MIDI_OUT_LIST_BOX);
ListMIDIOutPorts();

hBox1->Add(lblMIDIInPrompt, 0, wxEXPAND);  
hBox1->Add(lbxMIDIIn, 0, wxEXPAND);
hBox1->AddSpacer( 25);
hBox1->Add(lblMIDIOutPrompt, 0, wxEXPAND);
hBox1->Add(lbxMIDIOut, 0, wxEXPAND);
    
// Can only provide virtual ports on Mac and Linux
wxPlatformInfo myPlatformInfo;
int nMajor, nMinor;
// wxString wstrOpSys = myPlatformInfo.GetOperatingSystemFamilyName( wxGetOsVersion( &nMajor, &nMinor));
std::wstring wstrOpSys = myPlatformInfo.GetOperatingSystemFamilyName( wxGetOsVersion( &nMajor, &nMinor)).ToStdWstring();

if (wstrOpSys.compare( wstrWindowsOS) != 0)
{
wxButton *AddInputPortButton = new wxButton(myPanel, ID_AddVirtualInput, wstrAddVirtualInputButtonName);
wxButton *AddOutputPortButton = new wxButton(myPanel, ID_AddVirtualOutput, wstrAddVirtualOutputButtonName);
  hBox2->Add( AddInputPortButton, 0, wxEXPAND);
hBox2->Add( AddOutputPortButton, 0, wxEXPAND);
  }

wxButton * okButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName);
okButton->SetDefault();
	  wxButton *CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName);
hBox2->Add(okButton, 0, wxEXPAND);
  hBox2->Add(CancelButton, 0, wxEXPAND);

  vBox3->Add( hBox1);
vBox3->AddSpacer( 25);
  vBox3->Add(hBox2);
myPanel->SetSizer(vBox3);
myPanel->Fit();
	vBox3->Fit( myPanel);
	Centre();
}


MIDIDialog::MIDIDialog(const wxString& title, int nSelectedInput, int nSelectedOutput)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize( MIDI_DIALOG_HEIGHT, MIDI_DIALOG_WIDTH))
{
// Constructor 2
// wxMessageBox( wxT( "2"), wstrErrorTitle, wxOK | wxICON_ERROR);
	
// 	myVP = None;
  
/*
 try
{
myMIDIIn = new RtMidiIn();
myMIDIOut = new RtMidiOut();
} 
catch ( RtMidiError &error ) 
{
 wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
}
*/
wxPanel * myPanel = new wxPanel(this, -1);

wxBoxSizer *hBox1 = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hBox2 = new wxBoxSizer(wxHORIZONTAL);  

wxBoxSizer *vBox3 = new wxBoxSizer(wxVERTICAL);
  
  // List of MIDI input ports
  wxStaticBox *lblMIDIInPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIInPrompt);
lbxMIDIIn = new wxListBox(myPanel, ID_MIDI_IN_LIST_BOX);
ListMIDIInPorts( nSelectedInput);
 
// List of MIDI output ports
wxStaticBox *lblMIDIOutPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIOutPrompt);
lbxMIDIOut = new wxListBox(myPanel, ID_MIDI_OUT_LIST_BOX);
ListMIDIOutPorts( nSelectedOutput);
  
wxButton *okButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName);
  wxButton *CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName);

hBox1->Add(lblMIDIInPrompt, 0, wxEXPAND);  
hBox1->Add(lbxMIDIIn, 0, wxEXPAND);
hBox1->AddSpacer( 25);
hBox1->Add(lblMIDIOutPrompt, 0, wxEXPAND);
hBox1->Add(lbxMIDIOut, 0, wxEXPAND);
    
hBox2->Add(okButton, 0, wxEXPAND);
  hBox2->Add(CancelButton, 0, wxEXPAND);

  vBox3->Add( hBox1);
vBox3->AddSpacer( 25);
	vBox3->Add(hBox2);

	myPanel->SetSizer(vBox3);
myPanel->Fit();
	vBox3->Fit( myPanel);
	Centre();
}


MIDIDialog::MIDIDialog(const wxString& title, const bool blnVirtualIn, const bool blnVirtualOut)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize( MIDI_DIALOG_HEIGHT, MIDI_DIALOG_WIDTH))
{
// Constructor 3
// wxMessageBox( wxT( "3"), wstrErrorTitle, wxOK | wxICON_ERROR);
	
// 	myVP = VP;
	myVirtualIn = blnVirtualIn;
	myVirtualOut = blnVirtualOut;
	
	try
	{
myMIDIIn = new RtMidiIn();
		myMIDIOut = new RtMidiOut();
	}
	catch ( RtMidiError &error )
	{
		wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
	}
	
	  wxPanel * myPanel = new wxPanel(this, -1);
wxBoxSizer *hBox1 = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hBox2 = new wxBoxSizer(wxHORIZONTAL);  

wxBoxSizer *vBox3 = new wxBoxSizer(wxVERTICAL);
  
  // List of MIDI input ports
// if ((myVP == OutOnly) || (myVP == None))
if (myVirtualOut)
	{
		wxStaticBox *lblMIDIInPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIInPrompt);
lbxMIDIIn = new wxListBox(myPanel, ID_MIDI_IN_LIST_BOX);
ListMIDIInPorts();
hBox1->Add(lblMIDIInPrompt, 0, wxEXPAND);  
hBox1->Add(lbxMIDIIn, 0, wxEXPAND);
}

// List of MIDI output ports
// if ((myVP == InOnly) || (myVP == None))
if (myVirtualIn)
{
wxStaticBox *lblMIDIOutPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIOutPrompt);
lbxMIDIOut = new wxListBox(myPanel, ID_MIDI_OUT_LIST_BOX);
ListMIDIOutPorts();

hBox1->Add(lblMIDIOutPrompt, 0, wxEXPAND);
hBox1->Add(lbxMIDIOut, 0, wxEXPAND);
}

wxButton * okButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName);
okButton->SetDefault();
	  wxButton *CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName);
hBox2->Add(okButton, 0, wxEXPAND);
  hBox2->Add(CancelButton, 0, wxEXPAND);

  vBox3->Add( hBox1);
vBox3->AddSpacer( 25);
  vBox3->Add(hBox2);
myPanel->SetSizer(vBox3);
myPanel->Fit();
	vBox3->Fit( myPanel);
	Centre();
}


MIDIDialog::MIDIDialog(const wxString& title, const bool blnVirtualIn, int nSelectedPort)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize( MIDI_DIALOG_HEIGHT, MIDI_DIALOG_WIDTH))
{
// Constructor 4
	// wxMessageBox( wxT( "4"), wstrErrorTitle, wxOK | wxICON_ERROR);

// myVP = VP;
	myVirtualIn = blnVirtualIn;
	myVirtualOut = ! blnVirtualIn;

	try
{
myMIDIIn = new RtMidiIn();
myMIDIOut = new RtMidiOut();
} 
catch ( RtMidiError &error ) 
{
	wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
}
	
  wxPanel * myPanel = new wxPanel(this, -1);
wxBoxSizer *hBox1 = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hBox2 = new wxBoxSizer(wxHORIZONTAL);  

wxBoxSizer *vBox3 = new wxBoxSizer(wxVERTICAL);
  
  // List of MIDI input ports
// if ((myVP == OutOnly) || (myVP == None))
if (myVirtualOut)
{
		wxStaticBox *lblMIDIInPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIInPrompt);
lbxMIDIIn = new wxListBox(myPanel, ID_MIDI_IN_LIST_BOX);
ListMIDIInPorts( nSelectedPort);
hBox1->Add(lblMIDIInPrompt, 0, wxEXPAND);
hBox1->Add(lbxMIDIIn, 0, wxEXPAND);
}

// List of MIDI output ports
// if ((myVP == InOnly) || (myVP == None))
if (myVirtualIn)
{
wxStaticBox *lblMIDIOutPrompt = new wxStaticBox(myPanel, wxID_ANY, wstrMIDIOutPrompt);
lbxMIDIOut = new wxListBox(myPanel, ID_MIDI_OUT_LIST_BOX);
ListMIDIOutPorts( nSelectedPort);
hBox1->Add(lblMIDIOutPrompt, 0, wxEXPAND);  
hBox1->Add(lbxMIDIOut, 0, wxEXPAND);
}

wxButton * okButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName);
okButton->SetDefault();
	  wxButton *CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName);
hBox2->Add(okButton, 0, wxEXPAND);
  hBox2->Add(CancelButton, 0, wxEXPAND);

  vBox3->Add( hBox1);
vBox3->AddSpacer( 25);
  vBox3->Add(hBox2);
myPanel->SetSizer(vBox3);
myPanel->Fit();
	vBox3->Fit( myPanel);
	this->Maximize();
	Centre();
}


MIDIDialog::~MIDIDialog()
{
try
{
// delete myMIDIIn;
// delete myMIDIOut;
}
catch ( RtMidiError &error ) 
{
	wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
}
}


void MIDIDialog::SetSelectedInput( int nPortNumber)
{
}


void MIDIDialog::SetSelectedOutput( int nPortNumber)
{
}


int MIDIDialog::GetSelectedInput()
	{		
#ifndef __WINDOWS__ 
				if (myVirtualIn)
{
return VP_PORT_ID;
}
#endif
				
					int nSelection = lbxMIDIIn->GetSelection();

if (nSelection > -1)
{
	return nSelection;
}
else
{
	return -1;
}
}


std::string MIDIDialog::GetSelectedInputName()
	{		
#ifndef __WINDOWS__ 
				if (myVirtualIn)
	{
std::string strName;
strName.clear();
return strName;
}
#endif
				
					int nSelected = lbxMIDIIn->GetSelection();
if (nSelected >= 0)
	{
		return lbxMIDIIn->GetStringSelection().ToStdString();
	}
else
	{
	std::string strEmpty;
	strEmpty.clear();
	return strEmpty;
	}
}


int MIDIDialog::GetSelectedOutput()
{
#ifndef __WINDOWS__ 
if (myVirtualOut)
{
return VP_PORT_ID;
}
#endif

	int nSelection = lbxMIDIOut->GetSelection();

if (nSelection > -1)
{
	return nSelection;
}
else
{
	return -1;
}
}


std::string MIDIDialog::GetSelectedOutputName()
{		
#ifndef __WINDOWS__ 
if (myVirtualOut)
{
std::string strName;
strName.clear();
return strName;
}
#endif

	int nSelected = lbxMIDIOut->GetSelection();
if (nSelected >= 0)
	{
		return lbxMIDIOut->GetStringSelection().ToStdString();
	}
else
	{
	std::string strEmpty;
	strEmpty.clear();
	return strEmpty;
	}
}


void MIDIDialog::ListMIDIInPorts()
{
	RtMidiIn * MIDIInputs = 0;

	try
	{
MIDIInputs = new RtMidiIn();
	}
	catch ( RtMidiError &error )
	{
		wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}

for (unsigned int nID = 0; nID < MIDIInputs->getPortCount(); nID++)
			{
lbxMIDIIn->Append( MIDIInputs->getPortName( nID));
}  // end for

	delete MIDIInputs;
}


void MIDIDialog::ListMIDIInPorts( int nDeviceID)
{
 RtMidiIn * MIDIInputs = 0;
	
	try
	{
		MIDIInputs = new RtMidiIn();
	}
	catch ( RtMidiError &error )
	{
wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}
	
for (unsigned int nID = 0; nID < MIDIInputs->getPortCount(); nID++)
			{
				lbxMIDIIn->Append( MIDIInputs->getPortName( nID));

if (nID == nDeviceID)
	{
		lbxMIDIIn->SetSelection( lbxMIDIIn->GetCount() -1);
}
}  // end for
	// 	delete MIDIInputs;
}


void MIDIDialog::ListMIDIOutPorts()
{
 RtMidiOut * MIDIOutputs = 0;
	
	try
	{
		MIDIOutputs  = new RtMidiOut();
	}
	catch ( RtMidiError &error )
	{
		wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}

	for (unsigned int nID = 0; nID < MIDIOutputs->getPortCount(); nID++)
			{
				lbxMIDIOut->Append( MIDIOutputs->getPortName( nID));
			}  // end for
// 	delete MIDIOutputs;
}


void MIDIDialog::ListMIDIOutPorts( int nDeviceID)
{
	RtMidiOut * MIDIOutputs = 0;
	
	try
	{
		MIDIOutputs = new RtMidiOut();
	}
	catch ( RtMidiError &error )
	{
wxMessageBox( error.getMessage(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}
	
	for (unsigned int nID = 0; nID < MIDIOutputs->getPortCount(); nID++)
			{
				lbxMIDIOut->Append( MIDIOutputs->getPortName( nID));

if (nID == nDeviceID)
	{
lbxMIDIOut->SetSelection( lbxMIDIOut->GetCount() -1);
}
}  // end for
	delete MIDIOutputs;
}


// Event handlers


void MIDIDialog::OnAddVirtualInput( wxCommandEvent& event)
{
// 	myMIDIIn->openVirtualPort();
ListMIDIInPorts();
}

	
	void MIDIDialog::OnAddVirtualOutput( wxCommandEvent& event)
{
// 	myMIDIOut->openVirtualPort();
ListMIDIOutPorts();	
	}

	
void MIDIDialog::OnOK(wxCommandEvent& event)
{
	EndModal( wxID_OK);
}


    void MIDIDialog::OnCancel(wxCommandEvent& event)
	{
					EndModal( wxID_CANCEL);
	}


	// We need to implement an event table in which the events received by a MIDIDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(MIDIDialog, wxDialog)
		EVT_BUTTON(ID_AddVirtualInput,  MIDIDialog::OnAddVirtualInput)
EVT_BUTTON(ID_AddVirtualOutput,  MIDIDialog::OnAddVirtualOutput)
	        		EVT_BUTTON(wxID_OK,  MIDIDialog::OnOK)
	   EVT_BUTTON( wxID_CANCEL,                 MIDIDialog::OnCancel)
	END_EVENT_TABLE()
	
		