// DisplayParamsDlg.cpp


#include "DisplayParamsDlg.h"


DisplayParamsDlg::DisplayParamsDlg(const wxString & title, SurfaceActionType mySAType, boost::shared_ptr<AppConfig> pAppConfig)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
pMyAppConfig( new AppConfig())
{
pMyAppConfig = pAppConfig;
mySurfaceActionType = mySAType;

// List of the available displays
wxBoxSizer * vBoxLists = new wxBoxSizer( wxVERTICAL);
  wxStaticText * lblDisplaysPrompt = new wxStaticText(this, wxID_ANY, wstrDefinedDisplaysPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxDisplayNames = new wxListBox( this, ID_DISPLAYS_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));
	    
		  wxStaticText * lblParam1Prompt = new wxStaticText(this, wxID_ANY, wstrLineNumberPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxDisplayParam1 = new wxListBox( this, ID_DISPLAY_PARAMETER1_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));

wxStaticText * lblParam2Prompt = new wxStaticText(this, wxID_ANY, wstrStringLengthPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxDisplayParam2 = new wxListBox( this, ID_DISPLAY_PARAMETER2_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));

vBoxLists ->Add( lblDisplaysPrompt, 0, wxEXPAND);  
	  	  vBoxLists ->Add( lbxDisplayNames, 0, wxEXPAND);  
vBoxLists ->Add( lblParam1Prompt, 0, wxEXPAND);  
	  	  vBoxLists ->Add( lbxDisplayParam1, 0, wxEXPAND);  
vBoxLists ->Add( lblParam2Prompt, 0, wxEXPAND);  
	  	  vBoxLists ->Add( lbxDisplayParam2, 0, wxEXPAND);  

		  // Buttons 
		  wxBoxSizer * vBoxButtons = new wxBoxSizer( wxVERTICAL);
		  btnOK = new wxButton(this, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnOK->SetDefault();
		  btnCancel = new wxButton(this, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));

// Enable/disable controls appropriately
switch (mySAType)
{
case ReadDisplay:
case ReadLEDIfOn:
case ReadLEDIfOff:
		{
		lblParam1Prompt->Disable();
		lbxDisplayParam1->Disable();
		lblParam2Prompt->Disable();
		lbxDisplayParam2->Disable();
	}
	break;
	
case 	ReadDisplayLine:
	{
	lblParam2Prompt->Disable();
		lbxDisplayParam2->Disable();
		}
	break;

case ReadDisplayStrip:
	{
		lblParam1Prompt->SetLabel( wstrStripNumberPrompt);
lblParam2Prompt->Disable();
		lbxDisplayParam2->Disable();
		}
	break;

case ReadDisplaySection:
{
// Change the prompts and re-interpret the list contents
	lblParam1Prompt->SetLabel( wstrStartPositionPrompt);
	lblParam2Prompt->SetLabel( wstrStringLengthPrompt);
}
break;

default:
	{
	}
break;
}  // end switch

// Add the buttons to a sizer
vBoxButtons->Add( btnOK, 0, wxEXPAND);
vBoxButtons->Add( btnCancel, 0, wxEXPAND);
	
	wxBoxSizer * hBox1 = new wxBoxSizer( wxHORIZONTAL);
	hBox1->Add( vBoxLists);
	hBox1->Add( vBoxButtons);

	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBox1);

	// Populate the list boxes
	ListDisplays();

lbxDisplayNames->SetFocus();
SetSizerAndFit(vBox1);
}


DisplayParamsDlg::~DisplayParamsDlg()
{}


bool DisplayParamsDlg::IsValidParameterSet()
{
bool blnResult = false;

switch (mySurfaceActionType)
{
case ReadDisplay:
case ReadLEDIfOn:
case ReadLEDIfOff:
	{
		return (lbxDisplayNames->GetSelection() > -1);
	}
	break;
	
case 	ReadDisplayLine:
	case ReadDisplayStrip:
	{
return ((lbxDisplayNames->GetSelection() > -1)
	&& (lbxDisplayParam1->GetSelection() > -1));
			}
	break;

case ReadDisplaySection:
{
return ((lbxDisplayNames->GetSelection() > -1)
	&& (lbxDisplayParam1->GetSelection() > -1)	
	&& (lbxDisplayParam2->GetSelection() > -1));
				}
break;

default:
	{
	}
break;
}  // end switch

return blnResult;
}

std::string DisplayParamsDlg::GetDisplayHash()
{
	return pMyAppConfig->GetDisplayHash( lbxDisplayNames->GetStringSelection().ToStdWstring());
}


int DisplayParamsDlg::GetDisplayLine()
{
	return (lbxDisplayParam1->GetSelection() +1);
}


int DisplayParamsDlg::GetDisplayStrip()
{
	return (lbxDisplayParam1->GetSelection() +1);
}


int DisplayParamsDlg::GetDisplayStartPosition()
{
	return (lbxDisplayParam1->GetSelection() +1);
}


int DisplayParamsDlg::GetDisplaySubstringLength()
{
	long lTemp;
lbxDisplayParam2->GetStringSelection().ToLong( &lTemp);
return (lTemp +1);
}


void DisplayParamsDlg::ListDisplays()
{
// Clear the list box -necessary to prevent duplication due to the lack of data binding in wxListBox
	lbxDisplayNames->Clear();	

	for (unsigned int i = 0; i < pMyAppConfig->GetDisplayNames().size(); i++)
	{
switch (mySurfaceActionType)
{
case ReadDisplay:
	{
		lbxDisplayNames->Append( pMyAppConfig->GetDisplayNames().at( i));
	}
	break;

case ReadDisplayLine:
case ReadDisplayStrip:
case ReadDisplaySection:
	{
if (pMyAppConfig->GetDisplayDefinitionItem( i).IsLEDLamp() == false)		
		{
			lbxDisplayNames->Append( pMyAppConfig->GetDisplayNames().at( i));
}	
	}
	break;

case ReadLEDIfOn:
case ReadLEDIfOff:
	{
if (pMyAppConfig->GetDisplayDefinitionItem( i).IsLEDLamp())		
		{
			lbxDisplayNames->Append( pMyAppConfig->GetDisplayNames().at( i));
}	
	}
	break;

default:
{
}
break;
}  // end switch
		}  // end for
}


void DisplayParamsDlg::ListParameter1Values( std::string strHash)
{
	lbxDisplayParam1->Clear();	
DisplayDefinition myDisplay = pMyAppConfig->GetDisplayDefinition( strHash);

	switch (mySurfaceActionType)
{
case 	ReadDisplayLine:
	{
for (int nLine = 0; nLine < myDisplay.GetLineCount(); nLine++)
	{
		lbxDisplayParam1->Append( boost::lexical_cast <std::wstring> (nLine + 1));		
		}  // end for
}
	break;

case ReadDisplayStrip:
{
		for (int nStrip = 0; nStrip < myDisplay.GetStripCount(); nStrip++)
	{
		lbxDisplayParam1->Append( boost::lexical_cast <std::wstring> (nStrip + 1));		
		}  // end for
		}
break;

case ReadDisplaySection:
{
			for (int nStart = 1; nStart <= myDisplay.GetDisplayLength(); nStart++)
	{
		lbxDisplayParam1->Append( boost::lexical_cast <std::wstring> (nStart));		
		}  // end for
}
break;

default:
	{
	}
break;
}  // end switch
}


void DisplayParamsDlg::ListLengths()
{
	std::string strHash = pMyAppConfig->GetDisplayHash( lbxDisplayNames->GetStringSelection().ToStdWstring());
	int nStartSelection = lbxDisplayParam1->GetSelection();
	lbxDisplayParam2->Clear();

	if ((strHash.empty() == false)
		&& (nStartSelection > -1))
		{
			int nDisplayLength = pMyAppConfig->GetDisplayDefinition( strHash).GetDisplayLength();
			
			for (int nLength = (nStartSelection + 1); nLength <= nDisplayLength; nLength++)
	{
		lbxDisplayParam2->Append( boost::lexical_cast <std::wstring> ((nDisplayLength - nLength) +1));		
		}  // end for
}  // if nSelection
}


// Event handlers:

void DisplayParamsDlg::OnDisplaySelect( wxCommandEvent& event)
{
			// Refresh the dependent list
	ListParameter1Values( pMyAppConfig->GetDisplayHash( lbxDisplayNames->GetStringSelection().ToStdWstring()));
}


void DisplayParamsDlg::OnParameter1Select( wxCommandEvent& event)
{
if (mySurfaceActionType == ReadDisplaySection)
{
ListLengths();
}
}


void DisplayParamsDlg::OnOK( wxCommandEvent& event)
{
EndModal( wxID_OK);
}


void DisplayParamsDlg::OnCancel( wxCommandEvent& event)
{
EndModal( wxID_OK);
}


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DisplayParamsDlg, wxDialog)
// list box selection
EVT_LISTBOX( ID_DISPLAYS_LIST_BOX, DisplayParamsDlg::OnDisplaySelect)
EVT_LISTBOX( ID_DISPLAY_PARAMETER1_LIST_BOX, DisplayParamsDlg::OnParameter1Select)

// Buttons:
EVT_BUTTON(wxID_OK,  DisplayParamsDlg::OnOK)
	    EVT_BUTTON( wxID_CANCEL,                 DisplayParamsDlg::OnCancel)
END_EVENT_TABLE()
