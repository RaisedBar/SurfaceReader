// ActionPickerWin.cpp
// Implementation of the ActionPicker class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #ifdef __WINDOWS__ 

#include "ActionPickerWin.h"



ActionPicker::ActionPicker(const wxString & title, const wxString &wxstrControlName, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyAppConfig( new AppConfig()),
pMySpeech( new RBSpeech()),
pMyJawsCache( new JawsCacheType())
{
    pMyAppConfig = pAppConfig;
pMySpeech = pSpeech;
pMyJawsCache = pJawsCache;
	
myOldActiveProduct =myActiveProduct; 	
  pMySpeech->GetActiveProduct( myActiveProduct, myDolphinProduct);
    
wxPanel *myPanel = new wxPanel(this, -1);
// Tree of defined actions and parameters
  wxtActionPicker = new wxTreeCtrl(myPanel, ID_ACTION_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
   // Create root node
	      wxtIDRootNode = wxtActionPicker->AddRoot( wxstrControlName);

		  // Create a child node for every available action
		  ListSurfaceActionTypes();

if (wxtActionPicker->GetChildrenCount( wxtIDRootNode) > 0)
{
wxtActionPicker->Expand( wxtIDRootNode);
wxtActionPicker->SetFocusedItem( wxtActionPicker->GetFirstChild( wxtIDRootNode, cookie));
}

wxBoxSizer *HBox1 = new wxBoxSizer(wxHORIZONTAL);
HBox1->Add( wxtActionPicker, 0, wxEXPAND);

// Buttons 
btnActionProperties = new wxButton(myPanel, ID_PROPERTIES_BUTTON, wstrPropertiesButtonName, wxDefaultPosition, wxSize(70, 30));
btnOK = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize(70, 30));
btnOK->SetDefault();  
btnCancel = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize(70, 30));

  wxBoxSizer *HBox2 = new wxBoxSizer(wxHORIZONTAL);
HBox2->Add(btnActionProperties, 0, wxEXPAND);     
  HBox2->Add(btnOK, 0, wxEXPAND);
  HBox2->Add(btnCancel, 0, wxEXPAND);

  wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
vBox->Add(HBox1, 0, wxEXPAND);  
  vBox->Add(HBox2, 0, wxEXPAND);

myPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
myPanel->Fit();
vBox->Fit( myPanel);
Centre();
this->Maximize();

TogglePropertiesButton( wxtActionPicker->GetFocusedItem());
}


ActionPicker::~ActionPicker()
{}


bool ActionPicker::IsValidSurfaceAction()
{
if ((myNewSurfaceAction.GetSurfaceActionType() >= NoAction))
{
return true;
}
else
{
return false;
}
}


void ActionPicker::ListSurfaceActionTypes()
{
   	SurfaceAction myAction = SurfaceAction();

	for (unsigned int i =first; i <= last; i++)
{
	SurfaceActionType mySurfaceActionType = static_cast<SurfaceActionType>( i); 
	wxTreeItemId wxtIDNewNode = wxtActionPicker->AppendItem( wxtIDRootNode, myAction.GetSurfaceActionTypeDescription( mySurfaceActionType));
}  // end for
}

	
SurfaceActionType ActionPicker::GetDefinedSurfaceActionType()
{
	return myNewSurfaceAction.GetSurfaceActionType();
}


SurfaceAction ActionPicker::GetSurfaceAction()
	{
myNewSurfaceAction.SetSurfaceActionType( GetSurfaceActionTypeFromNode( wxtActionPicker->GetFocusedItem()));
return myNewSurfaceAction;
}


int ActionPicker::GetChildNumber( wxTreeItemId wxtIDCurrentNode)
{
wxTreeItemId myNode = wxtIDCurrentNode;
int nChildNumber = 0;

while (wxtActionPicker->GetPrevSibling( myNode).IsOk())
{
	nChildNumber++;
	myNode = wxtActionPicker->GetPrevSibling( myNode);
}

return nChildNumber;
}


int ActionPicker::GetLevel( wxTreeItemId myNode)
	{
int nLevel = 0;
wxTreeItemId wxtIDCurrentNode = myNode;
    
while (wxtIDCurrentNode .IsOk())
{
	wxtIDCurrentNode = wxtActionPicker->GetItemParent( wxtIDCurrentNode);
	if (wxtIDCurrentNode.IsOk())
	{
	nLevel++;
	}
}
				
return nLevel;
}


SurfaceActionType ActionPicker::GetSurfaceActionTypeFromNode( wxTreeItemId myCurrentNode)
{
wxTreeItemId wxtIDCurrentNode = myCurrentNode;
	
// find the correct level in order to get the control name
while (GetLevel( wxtIDCurrentNode)> SURFACE_ACTION_TYPE_LEVEL)
{
wxtIDCurrentNode = wxtActionPicker->GetItemParent( wxtIDCurrentNode);
}

return static_cast<SurfaceActionType>( GetChildNumber( wxtIDCurrentNode));
}


Mode ActionPicker::GetActionMode( wxTreeItemId myCurrentNode)
{
wxTreeItemId wxtIDCurrentNode = myCurrentNode;
	
// find the correct level in order to get the control name
while (GetLevel( wxtIDCurrentNode)> SURFACE_ACTION_MODE_LEVEL)
{
wxtIDCurrentNode = wxtActionPicker->GetItemParent( wxtIDCurrentNode);
}

switch (GetChildNumber( wxtIDCurrentNode)) 
	{case 0:
{
	return ID_LIVE_MODE;
}
break;

	case 1:
		{
return ID_SCREEN_READER_MODE;
		}
		break;

	default:
		{
		return ID_UNKNOWN_MODE;
		}
		break;
}  // end switch
}


void ActionPicker::GetDisplayParameters()
{
DisplayParamsDlg * myDisplayParamsDlg = new DisplayParamsDlg( wstrDefineDisplayParametersTitle, myNewSurfaceAction.GetSurfaceActionType(), pMyAppConfig);	

 	 		if (myDisplayParamsDlg->ShowModal() == wxID_OK)
{
	if (myDisplayParamsDlg->IsValidParameterSet())
	{
// populate the parameters here:
switch (myNewSurfaceAction.GetSurfaceActionType())
{
case ReadDisplay:
{
std::wstring strHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrHash( strHash.begin(), strHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrHash);
}
break;

case ReadDisplayLine:
{
	std::wstring strHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrHash( strHash.begin(), strHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrHash);
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayLine()));
	}
break;

case ReadDisplayStrip:
{
	std::wstring strHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrHash( strHash.begin(), strHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrHash);
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayStrip()));
}
break;

case ReadDisplaySection:
{
	std::wstring strHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrHash( strHash.begin(), strHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrHash);
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayStartPosition()));
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplaySubstringLength()));
}
break;

case ReadLEDIfOn:
case ReadLEDIfOff:
{
std::wstring strHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrHash( strHash.begin(), strHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrHash);
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
		if (myNewSurfaceAction.GetSurfaceActionType() == RunHotSpot)
		{
wstrDialogTitle = wstrHotSpotsTitle;
		}
		else
		{
wstrDialogTitle = wstrScreenReaderFunctionsTitle;
		}

#ifdef __WINDOWS__
		ScreenReaderFunctionsDlg * myScreenReaderFunctionsDlg = new ScreenReaderFunctionsDlg( wstrDialogTitle, myNewSurfaceAction.GetSurfaceActionType(), pMyAppConfig, pMySpeech, pMyJawsCache);	
#else
ScreenReaderFunctionsDlg * myScreenReaderFunctionsDlg = new ScreenReaderFunctionsDlg( wstrDialogTitle, myNewSurfaceAction.GetSurfaceActionType(), pMyAppConfig, pMySpeech);	
#endif

 	 		if (myScreenReaderFunctionsDlg->ShowModal() == wxID_OK)
{
	if (myScreenReaderFunctionsDlg->IsValidParameterSet())
	{
// populate the parameters here:
		myNewSurfaceAction.SetParameter(Type_WString, myScreenReaderFunctionsDlg->GetTargetFileName());
		myNewSurfaceAction.SetParameter(Type_WString, myScreenReaderFunctionsDlg->GetFunctionName());
		
if (myNewSurfaceAction.GetSurfaceActionType() == RunHotSpot)
		{
myNewSurfaceAction.SetScreenReaderActionType( ID_FUNCTION);
}
else
	{
		try
			{
				myNewSurfaceAction.SetScreenReaderActionType( myScreenReaderFunctionsDlg->GetActionTypeFromAction( myScreenReaderFunctionsDlg->GetAction( myScreenReaderFunctionsDlg->GetFunctionName())));
		}
catch( ...)
{
myNewSurfaceAction.SetScreenReaderActionType( ID_FUNCTION);
}
}
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


	void ActionPicker::TogglePropertiesButton( wxTreeItemId wxtIDCurrentNode)
	{
int nLevel = GetLevel( wxtIDCurrentNode);
	
switch (nLevel)
	{
case SURFACE_ACTION_TYPE_LEVEL:
	{
SurfaceActionType myActionType; 
myActionType = GetSurfaceActionTypeFromNode( wxtActionPicker->GetFocusedItem());

switch (myActionType)
{
case ReadDisplay:
case ReadDisplayLine:
case ReadDisplayStrip:
case ReadDisplaySection:
case SpeakString:
case RunHotSpot:
case RunScreenReaderFunction:
case ReadLEDIfOn:
case ReadLEDIfOff:
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

void ActionPicker::OnTreeSelect( wxTreeEvent& event)
{
	wxTreeItemId wxtIDCurrentNode = wxtActionPicker->GetFocusedItem();
TogglePropertiesButton( wxtIDCurrentNode);

if (GetLevel( wxtIDCurrentNode) == SURFACE_ACTION_TYPE_LEVEL)
{
	myNewSurfaceAction.SetSurfaceActionType( static_cast<SurfaceActionType>( GetChildNumber( wxtIDCurrentNode)));
}
}


void ActionPicker::OnActionProperties(wxCommandEvent& event)
{
	switch (myNewSurfaceAction.GetSurfaceActionType())
{
case ReadDisplay:
case ReadDisplayLine:
case ReadDisplayStrip:
case ReadDisplaySection:
case ReadLEDIfOn:
case ReadLEDIfOff:
		{
		GetDisplayParameters();
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

case SpeakString:
{
bool blnSuccess = GetMessageString();
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
	

		void ActionPicker::ListScreenReaderActions( SurfaceActionType mySAType, wxTreeItemId wxtIDCurrentNode)
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

	for (unsigned int i =ReadControlLabel; i <= MIDIInfo; i++)
{
	SurfaceActionType myActionType = static_cast<SurfaceActionType>( i); 
   wxtActionPicker->AppendItem( wxtIDRootNode, myAction.GetTypeDescription( myActionType));
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
	for(ActionInfoType CurrentAction : myActionInformation.second);
	{
		it2 =CurrentAction.find(nField);
		if (it2 !=CurrentAction.end())
		{
std::wstring wstrFieldValue;

        if ( IsWString( it2->second))
        {
			wstrFieldValue = AsWString( it2->second);
}

	wxTreeItemId wxtIDCurrentNode = wxtActionPicker->GetFocusedItem(); 
	wxtActionPicker->AppendItem( wxtIDCurrentNode, wstrFieldValue);
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
EVT_TREE_SEL_CHANGED( ID_ACTION_TREE, ActionPicker::OnTreeSelect)
	    
// Buttons
EVT_BUTTON(ID_PROPERTIES_BUTTON,  ActionPicker::OnActionProperties)
EVT_BUTTON(wxID_OK,  ActionPicker::OnOK)
	    EVT_BUTTON( wxID_CANCEL,                 ActionPicker::OnCancel)
END_EVENT_TABLE()
	// #endif

		