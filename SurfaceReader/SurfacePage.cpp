// SurfacePage.cpp
// Implementation of the SurfacePage class to provide a custom dialog allowing the user to create a new control surface object
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "SurfacePage.h"


SurfacePage::SurfacePage( wxWizard * wizParent, const wxString & title, bool blnPickProtocol, boost::shared_ptr<SurfaceParameters> pSurfaceParameters, boost::shared_ptr<ProtocolCollection> pProtocols)
: wxWizardPageSimple( wizParent),
pMySurfaceParameters( new SurfaceParameters()),
pMyProtocols( new ProtocolCollection())
{
	SetLabel( title);
	    SetExtraStyle( wxWIZARD_EX_HELPBUTTON);
blnListProtocols = blnPickProtocol;
pMySurfaceParameters = pSurfaceParameters;  
pMyProtocols = pProtocols;

pMySurfaceParameters->SetHardwareInID( -1);
pMySurfaceParameters->SetHardwareOutID( -1);
pMySurfaceParameters->SetDisplayInID( -1);
pMySurfaceParameters->SetDisplayOutID( -1);

// Prompt for a descriptive name for the new surface
wxBoxSizer * hBoxSurface = new wxBoxSizer( wxHORIZONTAL);
  wxStaticText * lblSurfaceNamePrompt = new wxStaticText(this, wxID_ANY, wstrSurfaceNamePrompt);
		txtSurfaceName = new wxTextCtrl( this, wxID_ANY);
		txtSurfaceName->SetValue( pMySurfaceParameters->GetSurfaceName());
		
		hBoxSurface->Add( lblSurfaceNamePrompt, 0, wxEXPAND);  
hBoxSurface->Add( txtSurfaceName, 0, wxEXPAND);

wxBoxSizer * hBoxProtocols = new wxBoxSizer( wxHORIZONTAL);

if (blnListProtocols)
{
// List of available control surface protocols
lblProtocolPrompt = new wxStaticText(this, wxID_ANY, wstrProtocolPrompt);
	lbxSurfaceProtocols = new wxListBox(this, ID_PROTOCOL_LIST_BOX,  wxDefaultPosition, wxSize( 100, 200), 0, NULL, wxLB_SORT, wxDefaultValidator, wxT( ""));
		// Populate the list
ListProtocols();
hBoxProtocols->Add( lblProtocolPrompt, 0, wxEXPAND); 
	hBoxProtocols->Add( lbxSurfaceProtocols, 0, wxEXPAND);  
}  // end if listing protocols  
	
// Buttons
wxBoxSizer * hBoxButtons = new wxBoxSizer( wxHORIZONTAL);
wxButton * HardwareSetupdButton = new wxButton(this, ID_HardwareSetup, wstrHardwareSetupButtonName);
wxButton * DisplaySetupdButton = new wxButton(this, ID_DisplaySetup, wstrDisplaySetupButtonName);// wxButton *AddButton = new wxButton(this, ID_ADD, wstrAddButtonName);
hBoxButtons->Add( HardwareSetupdButton, 0, wxEXPAND);
hBoxButtons->Add( DisplaySetupdButton, 0, wxEXPAND);
  
wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxSurface);
	vBox1->AddSpacer( 25);
	
	if (blnListProtocols) 
	{
vBox1->Add( hBoxProtocols);
		vBox1->AddSpacer( 25);
}
	
	vBox1->Add( hBoxButtons);
        SetSizerAndFit(vBox1);
}


std::wstring SurfacePage::GetSurfaceName()
{
	return txtSurfaceName->GetValue().ToStdWstring();
}


std::wstring SurfacePage::GetProtocolID()
{
	std::wstring strProtocolID;
	strProtocolID.clear();

	if (blnListProtocols)
		{
			int nSelection = lbxSurfaceProtocols->GetSelection();

	if (nSelection> -1)
{	
std::wstring wstrProtocolName = lbxSurfaceProtocols->GetString( nSelection).ToStdWstring();
strProtocolID = pMyProtocols->FindIDForProtocolName( wstrProtocolName);
}
	}
	else  // not listing protocols = new protocol, or editing an existing one
	{
		if (pMySurfaceParameters->GetProtocolID().empty())
		{
			// New protocol
			SurfaceProtocol myProtocol;
strProtocolID = myProtocol.GetProtocolID();
	}
		else
		{
// Editing an existing protocol
			strProtocolID = pMySurfaceParameters->GetProtocolID();
		}
	}	
		
	return strProtocolID;
}


bool SurfacePage::IsValidPortID( int nPortID)
{
return (nPortID >= 0);
}


bool SurfacePage::IsValidSurfaceName()
{
	return (! txtSurfaceName->GetValue().IsEmpty());
}


bool SurfacePage::IsValidProtocol()
{
return (lbxSurfaceProtocols->GetSelection() >= 0);
}


bool SurfacePage::IsValidSurface()
{
bool blnResult = true;
std::wstring wstrError;
bool blnValidName = IsValidSurfaceName();
bool blnValidDisplayIn = IsValidPortID( nDisplay_VirtualMIDIIn);
bool blnValidDisplayOut = IsValidPortID( nDisplay_MIDIOut);
bool blnValidHwIn  = IsValidPortID( nHardware_MIDIIn);
bool blnValidHwOut = IsValidPortID( nHardware_VirtualMIDIOut);

if (blnListProtocols == false)
	{
if (blnValidName == false) 
{
blnResult = false;
wstrError.append( wstrInvalidSurfaceNameError);
}

if (blnValidDisplayIn == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidDisplayInError);
}

	if (blnValidDisplayOut == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidDisplayOutError);
}

if (blnValidHwIn == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidHwInError);
}

if (blnValidHwOut == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidHwOutError);
}
}  // if not listing protocols
else
	{
		bool blnValidProtocol = IsValidProtocol();

		if (blnValidName == false) 
{
blnResult = false;
wstrError.append( wstrInvalidSurfaceNameError);
}

if (blnValidProtocol == false)
	{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidProtocolError);
}

if (blnValidDisplayIn == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidDisplayInError);
}

	if (blnValidDisplayOut == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidDisplayOutError);
}

if (blnValidHwIn == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidHwInError);
}

if (blnValidHwOut == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidHwOutError);
}
}

if (blnResult == false)
	{
wxMessageBox( wstrError, wstrErrorTitle, wxOK | wxICON_ERROR);
}

return blnResult;
}


// Event handlers

void SurfacePage::OnDisplaySetup(wxCommandEvent& event)
{
nDisplay_VirtualMIDIIn = pMySurfaceParameters->GetDisplayInID();
nDisplay_MIDIOut = pMySurfaceParameters->GetDisplayOutID();
	

#ifdef __WINDOWS__
	MIDIDialog * dlgDisplaySetup = new MIDIDialog(	wstrDisplaySetupTitle);
#else
	MIDIDialog * dlgDisplaySetup = new MIDIDialog(wstrDisplaySetupTitle, true, false);
#endif

if (dlgDisplaySetup->ShowModal() == wxID_OK)
{
	nDisplay_VirtualMIDIIn = dlgDisplaySetup->GetSelectedInput();
	nDisplay_MIDIOut = dlgDisplaySetup->GetSelectedOutput();
	strDisplay_VirtualMIDIInName = dlgDisplaySetup->GetSelectedInputName();
strDisplay_MIDIOutName = dlgDisplaySetup->GetSelectedOutputName();
}  // OK button processing finished

// 	dlgDisplaySetup->Destroy();
}


		void SurfacePage::OnHardwareSetup(wxCommandEvent& event)
{
nHardware_MIDIIn = pMySurfaceParameters->GetHardwareInID();
nHardware_VirtualMIDIOut = pMySurfaceParameters->GetHardwareOutID();

#ifdef __WINDOWS__ 
	MIDIDialog * dlgHardwareSetup = new MIDIDialog( wstrHardwareSetupTitle);
#else
	MIDIDialog * dlgHardwareSetup = new MIDIDialog(wstrHardwareSetupTitle, false, true);
#endif

if (dlgHardwareSetup->ShowModal() == wxID_OK) 
{
nHardware_MIDIIn = dlgHardwareSetup->GetSelectedInput();
nHardware_VirtualMIDIOut = dlgHardwareSetup->GetSelectedOutput();
strHardware_MIDIInName = dlgHardwareSetup->GetSelectedInputName();
strHardware_VirtualMIDIOutName = dlgHardwareSetup->GetSelectedOutputName();
}  // OK button processing finished
// dlgHardwareSetup->Destroy();
}


		int SurfacePage::GetHardware_MIDIIn()
{
return nHardware_MIDIIn;
}


int SurfacePage::GetHardware_VirtualMIDIOut()
{
return nHardware_VirtualMIDIOut;
		}


int SurfacePage::GetDisplay_VirtualMIDIIn()
{
return 	nDisplay_VirtualMIDIIn;
		}


int SurfacePage::GetDisplay_MIDIOut()
{
return nDisplay_MIDIOut;
		}


std::wstring SurfacePage::GetHardware_MIDIInName()
{
	return strHardware_MIDIInName;
}


std::wstring SurfacePage::GetHardware_VirtualMIDIOutName()
{
if (nHardware_VirtualMIDIOut == VP_PORT_ID)
{
	std::wstring strName = NarrowToWideString(txtSurfaceName->GetValue().ToStdString());
	strName.append( strVirtualOutSuffix);
return strName;
}
else
	{
		return strHardware_VirtualMIDIOutName;
}
}


std::wstring SurfacePage::GetDisplay_VirtualMIDIInName()
{
if (nDisplay_VirtualMIDIIn == VP_PORT_ID)
	{
		std::wstring strName = NarrowToWideString(txtSurfaceName->GetValue().ToStdString());
	strName.append( strVirtualInSuffix);
return strName;
}
else
	{
		return 	strDisplay_VirtualMIDIInName;
}
}


std::wstring SurfacePage::GetDisplay_MIDIOutName()
{
return strDisplay_MIDIOutName;
		}
		
void SurfacePage::ListProtocols()
{
		std::map <std::wstring, SurfaceProtocol> ::const_iterator it;

		for (it = pMyProtocols->begin(); it != pMyProtocols->end(); it++)
	{
		lbxSurfaceProtocols->Append( it->second.GetProtocolName());
	}
}


void SurfacePage::OnWizardCancel(wxWizardEvent& event)
    {
		std::wstring wstrMessage;

		if (blnListProtocols)
		{
			wstrMessage = wstrCancelSurfaceCheck;
		}
		else
		{
 wstrMessage = wstrCancelProtocolCheck;
		}

		if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO)
{
            event.Veto();
	}
		}

    
// We need to implement an event table in which the events received by a wxSurfacePage are routed to their respective handler functions 
BEGIN_EVENT_TABLE(SurfacePage, wxWizardPage)
		EVT_WIZARD_CANCEL( wxID_ANY, SurfacePage::OnWizardCancel)

				EVT_BUTTON( ID_DisplaySetup,  SurfacePage::OnDisplaySetup)
	    EVT_BUTTON( ID_HardwareSetup,                 SurfacePage::OnHardwareSetup)
				END_EVENT_TABLE()