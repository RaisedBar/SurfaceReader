// ActionPickerMac.cpp
// Implementation of the ActionPicker class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifdef __WXOSX_COCOA__

#include "ActionPickerMac.h"


ActionPicker::ActionPicker(const wxString & title, const std::wstring  &wstrControlName, boost::shared_ptr <AppConfig> pAppConfig, boost::shared_ptr <SurfaceProtocol> pProtocol, boost::shared_ptr <RBSpeech> pSpeech)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	pMyAppConfig( new AppConfig()),
	pMyProtocol( new SurfaceProtocol ()),
	pMySpeech( new RBSpeech())
{
	pMyAppConfig = pAppConfig;
	pMyProtocol = pProtocol;
	pMySpeech = pSpeech;

wstrMyControl = wstrControlName;
myOldActiveProduct =myActiveProduct; 	
  pMySpeech->GetActiveProduct( myActiveProduct, myDolphinProduct);
    
wxPanel *myPanel = new wxPanel(this, -1);
    BuildDataView( myPanel);
	m_actions_model->Resort();

      const wxSizerFlags border = wxSizerFlags().DoubleBorder();

wxBoxSizer *hBox1 = new wxBoxSizer(wxHORIZONTAL);
hBox1->Add( myDataView, 0, wxEXPAND);

// Buttons 
btnActionProperties = new wxButton(myPanel, ID_PROPERTIES_BUTTON, wstrPropertiesButtonName, wxDefaultPosition, wxSize(70, 30));
btnOK = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize(70, 30));
btnOK->SetDefault();  
btnCancel = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize(70, 30));

  wxBoxSizer *hBox2 = new wxBoxSizer(wxHORIZONTAL);
hBox2->Add(btnActionProperties, 0, wxEXPAND);     
  hBox2->Add(btnOK, 0, wxEXPAND);
  hBox2->Add(btnCancel, 0, wxEXPAND);

  wxSizer *myPanelSz = new wxBoxSizer( wxVERTICAL );
myDataView->SetMinSize(wxSize(-1, 200));
    myPanelSz->Add(myDataView, 1, wxGROW|wxALL, 5);
        myPanelSz->Add(hBox1);
        myPanelSz->Add(hBox2);
myPanel->SetSizerAndFit(myPanelSz);

    	// Enable/disable buttons appropriately
TogglePropertiesButton( myDataView->GetCurrentItem());
}


ActionPicker::~ActionPicker()
{}


void ActionPicker::BuildDataView( wxPanel* parent, unsigned long style)
{
// | wxDV_NO_HEADER
	myDataView  = new wxDataViewCtrl( parent, ID_ACTION_TREE, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE );
                        
myDataView ->Connect(wxEVT_CHAR, wxKeyEventHandler(ActionPicker::OnDataViewChar), NULL, this);
m_actions_model = new ActionPickerModel( wstrMyControl);
	myDataView ->AssociateModel( m_actions_model.get() );            
                        	
            // column 0 of the view control (Control)
wxDataViewTextRenderer *tr0 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *ControlColumn = new wxDataViewColumn( "Control", tr0, SELECTED_CONTROL_LEVEL, 200, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE );
            myDataView ->AppendColumn( ControlColumn);

			// column 1 of the view control: Action Type
wxDataViewTextRenderer *tr1 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *TypesColumn = new wxDataViewColumn( "Types", tr1, SURFACE_ACTION_TYPE_LEVEL, 150, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE );
                        myDataView ->AppendColumn( TypesColumn);
}

            
bool ActionPicker::IsValidSurfaceAction()
{
	if (m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()) >= NoAction)
{
return true;
}
else
{
return false;
}
}


SurfaceActionType ActionPicker::GetDefinedSurfaceActionType()
{
	return m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem());
}


SurfaceAction ActionPicker::GetSurfaceAction()
	{
		myNewSurfaceAction.SetSurfaceActionType( m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()));
return myNewSurfaceAction;
}


int ActionPicker::GetChildNumber( wxDataViewItem wxtIDCurrentNode)
{
	wxDataViewItem myParentNode = m_actions_model->GetParent( wxtIDCurrentNode);
	wxDataViewItem myNode = wxtIDCurrentNode;
int nChildNumber = -1;
wxDataViewItemArray myChildren;
	m_actions_model->GetChildren( myParentNode, myChildren);

	for (unsigned int i = 0; i < myChildren.GetCount(); i++)
{
	if (myChildren.Item( i) == myNode) 
	{
	nChildNumber = i;
	break;
	}
	}  // end for

return nChildNumber;
}


void ActionPicker::GetDisplayParameters()
{
	DisplayParamsDlg * myDisplayParamsDlg = new DisplayParamsDlg( wstrDefineDisplayParametersTitle, m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()), pMyAppConfig);

 	 		if (myDisplayParamsDlg->ShowModal() == wxID_OK)
{
	if (myDisplayParamsDlg->IsValidParameterSet())
	{
// populate the parameters here:
		switch (m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()))
{
case ReadDisplay:
{
	std::string strDisplayHash = myDisplayParamsDlg->GetDisplayHash();
	std::wstring wstrDisplayHash( strDisplayHash.begin(), strDisplayHash.end());
		myNewSurfaceAction.SetParameter(Type_String, wstrDisplayHash);
}
break;

case ReadDisplayLine:
{
	wxString wxstrDisplayHash = myDisplayParamsDlg->GetDisplayHash();
	myNewSurfaceAction.SetParameter(Type_String, wxstrDisplayHash.ToStdWstring());
	std::wstring wstrLines  = boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayLine());

	myNewSurfaceAction.SetParameter(Type_Int, wstrLines);
}
break;

case ReadDisplayStrip:
{
	wxString wxstrDisplayHash = myDisplayParamsDlg->GetDisplayHash();
	myNewSurfaceAction.SetParameter(Type_String, wxstrDisplayHash.ToStdWstring());
	myNewSurfaceAction.SetParameter(Type_Int, wxString::Format( "%d", myDisplayParamsDlg->GetDisplayStrip()).ToStdWstring());
}
break;

case ReadDisplaySection:
{
	wxString wxstrDisplayHash = myDisplayParamsDlg->GetDisplayHash();
	myNewSurfaceAction.SetParameter(Type_String, wxstrDisplayHash.ToStdWstring());
	myNewSurfaceAction.SetParameter(Type_Int, wxString::Format( "%d", myDisplayParamsDlg->GetDisplayStartPosition()).ToStdWstring());
	myNewSurfaceAction.SetParameter(Type_Int, wxString::Format( "%d", myDisplayParamsDlg->GetDisplaySubstringLength()).ToStdWstring());
}
break;

default:
	{}
	break;
}  // end switch
	}  // end if valid
	}  // end if OK
}


	void ActionPicker::GetScreenReaderFunction()
	{
		std::wstring wstrDialogTitle;

// Adjust the dialog title
if (m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()) == RunHotSpot)
		{
wstrDialogTitle = wstrHotSpotsTitle;
		}
		else
		{
wstrDialogTitle = wstrScreenReaderFunctionsTitle;
		}

ScreenReaderFunctionsDlg * myScreenReaderFunctionsDlg = new ScreenReaderFunctionsDlg( wstrDialogTitle, m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()), pMyAppConfig, pMySpeech);

 	 		if (myScreenReaderFunctionsDlg->ShowModal() == wxID_OK)
{
	if (myScreenReaderFunctionsDlg->IsValidParameterSet())
	{
// populate the parameters here:
		myNewSurfaceAction.SetParameter(Type_WString, myScreenReaderFunctionsDlg->GetTargetFileName());
		myNewSurfaceAction.SetParameter(Type_WString, myScreenReaderFunctionsDlg->GetFunctionName());
}  // end if valid
}  // end if OK
		}


		bool ActionPicker::GetMessageString()
	{
std::wstring wstrMsg;
RBInputBox * MsgDlg = new RBInputBox( wstrMessageStringPrompt, this->GetLabel());
		
		if (MsgDlg ->ShowModal() == wxID_CANCEL)
		{
			return false;
		}
				
		wstrMsg = MsgDlg->GetText();

		if (wstrMsg.empty())
			{
wxMessageBox( wstrNoStringError,                wstrAppTitle, wxOK | wxICON_INFORMATION );
return false;
		}
		
		myNewSurfaceAction.SetParameter(Type_String, wstrMsg);
	return true;
	}


void ActionPicker::TogglePropertiesButton( wxDataViewItem wxtIDCurrentNode)
	{
switch (m_actions_model->GetLevel( wxtIDCurrentNode))
	{
case SURFACE_ACTION_TYPE_LEVEL:
	{
SurfaceActionType myActionType; 
myActionType = m_actions_model->GetSurfaceActionType( wxtIDCurrentNode);

switch (myActionType)
{
case ReadDisplay:
case ReadDisplayLine:
case ReadDisplayStrip:
case ReadDisplaySection:
// case PressKeyCombination:
case SpeakString:
case RunHotSpot:
case RunScreenReaderFunction:
	{
		btnActionProperties->Enable();
	}
	break;

default:
	{
		btnActionProperties->Disable(); 	
}
	break;
}  // end switch
}
break;

default:
	{}
	break;
}  // end switch
	}


	// Event handlers

void ActionPicker::OnSelectionChanged( wxDataViewEvent &event )
{
	TogglePropertiesButton( event.GetItem());
	
	wxString title = m_actions_model->GetTitle( event.GetItem() );
    
	if (title.empty())
        {
			title = "None";
	}

    // wxLogMessage( "wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, First selected Item: %s", title );
}


void ActionPicker	::OnDataViewChar(wxKeyEvent& event)
{
    if ( event.GetKeyCode() == WXK_DELETE )
        {
			event.Skip();
	// DeleteSelectedItems();
	}
	else
        {
			event.Skip();
	}
	}


void ActionPicker::OnProperties(wxCommandEvent& event)
{
	switch ( m_actions_model->GetSurfaceActionType( myDataView->GetCurrentItem()))
{
case ReadDisplay:
case ReadDisplayLine:
case ReadDisplayStrip:
case ReadDisplaySection:
{
		GetDisplayParameters();
}  
break;

/*
 case PressKeyCombination:
{
		wxMessageBox( wstrNotYetImplementedError,                wstrAppTitle, wxOK | wxICON_INFORMATION );
}
	break;
*/
		
case SpeakString:
	{
	bool blnSuccess = GetMessageString();
}
	break;

case ChangeSurfaceMode:
{
wxMessageBox( wstrNotYetImplementedError,                wstrAppTitle, wxOK | wxICON_INFORMATION );
}
	break;

case RunHotSpot:
case RunScreenReaderFunction:
{
	GetScreenReaderFunction();
			}
			break;

default:
	{
		}
	break;
}  // end switch
}

			
	void ActionPicker::OnOK(wxCommandEvent& event)
{
EndModal( wxID_OK);
}

	
void ActionPicker::OnCancel(wxCommandEvent& event)
	{
	EndModal( wxID_CANCEL);
}
	

		void ActionPicker::ListScreenReaderActions( SurfaceActionType mySAType, wxDataViewItem wxtIDCurrentNode)
{
switch (myActiveProduct)
{
case ID_NONE:
	{
		return;
		}
	break;

case ID_JAWS:
	{
ListJawsActions();
	}
	break;

case ID_SYSTEMACCESS:
	{
		ListSAActions();
	}
	break;

case ID_WindowEyes:
	{
		ListWindowEyesActions();
	}
	break;

case ID_NVDA:
	{
		ListNVDAActions();
	}
	break;

case ID_DOLPHIN:
{
		ListDolphinActions();
	}
	break;

default:
{
		return;
	}
	break;
}  // end switch
}

	
	void ActionPicker::ListScreenReaderParameters()
{
/*
SurfaceAction myAction = SurfaceAction();

	for (unsigned int i =ReadControlLabel; i <= RunScreenReaderFunction; i++)
{
	SurfaceActionType myActionType = static_cast<SurfaceActionType>( i); 
   myDataView->AppendItem( wxtIDRootNode, myAction.GetTypeDescription( myActionType));
   }  // end for
*/
	}

	
void ActionPicker::ListJawsActions()
{
AvailableActionsType myActionInformation;

HRESULT hr = pMySpeech->GetAvailableActions( myActionInformation);	

// if (SUCCEEDED( hr))
if (hr == S_OK)
{
AvailableActionFieldsIterator it;
it = myActionInformation.first.find( wstrNameField);

if (it != myActionInformation.first.end())
{
	int nField = it->second;
//iterate through the vector of actions, the second item in the ActionInfoType pair.
	ActionInfoTypeIterator it2;
	ActionInfoType CurrentAction;
	BOOST_FOREACH(CurrentAction, myActionInformation.second);
	{
		it2 =CurrentAction.find(nField);
		if (it2 !=CurrentAction.end())
		{
			std::wstring wstrFieldValue;

        if (IsWString( it2->second))
        {
			wstrFieldValue = AsWString( it2->second);
}

		wxDataViewItem wxtIDCurrentNode = myDataView->GetCurrentItem(); 
	// myDataView->AppendItem( wxtIDCurrentNode, wxstrFieldValue);
	}
	} //end foreach.
}  // end if the name field is present
else // not found
{
return;
}
}
else  // didn't succeed
{
return;
	}  // end if succeeded
}


		
void ActionPicker::ListSAActions()
{}

	
void ActionPicker::ListWindowEyesActions()
{}


void ActionPicker::ListNVDAActions()
{}


		void ActionPicker::ListDolphinActions()
		{}


		// We need to implement an event table in which the events received by an ActionPicker are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ActionPicker, wxDialog)
// DataView handling
	    EVT_DATAVIEW_SELECTION_CHANGED(ID_ACTION_TREE, ActionPicker::OnSelectionChanged)
  
// Buttons
EVT_BUTTON(ID_PROPERTIES_BUTTON,  ActionPicker::OnProperties)
EVT_BUTTON(wxID_OK,  ActionPicker::OnOK)
	    EVT_BUTTON( wxID_CANCEL,                 ActionPicker::OnCancel)
END_EVENT_TABLE()
#endif  // if Mac
