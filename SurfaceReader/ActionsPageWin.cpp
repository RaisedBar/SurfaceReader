// ActionsPageWin.cpp
// Implementation of the ActionsPage class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "ActionsPageWin.h"


ActionsPage::ActionsPage(wxWizard * wizParent, const wxString &title, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache)
       : wxWizardPageSimple( wizParent),
	   pMyAppConfig( new AppConfig()),
pMyProtocol( new SurfaceProtocol()),
pMySpeech( new RBSpeech()),
pMyJawsCache( new JawsCacheType())
{
  pMyAppConfig = pAppConfig;
  pMyProtocol = pProtocol;
pMySpeech = pSpeech;
pMyJawsCache = pJawsCache;
myControls = pMyAppConfig->GetHardwareControls();
myOldActiveProduct =myActiveProduct;
   pMySpeech->GetActiveProduct( myActiveProduct, myDolphinProduct);
  
  wxPanel *myPanel = new wxPanel(this, -1);
// Tree of defined actions and parameters
  wxActionsTree = new wxTreeCtrl(myPanel, ID_ACTION_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
   // Create root node
	      wxtIDRootNode = wxActionsTree->AddRoot( pMyProtocol->GetProtocolName().append( wstrProtocolActions));
          
ListControls( wxtIDRootNode);
wxActionsTree->SortChildren( wxtIDRootNode);

if (wxActionsTree->GetChildrenCount( wxtIDRootNode) > 0)
{
// wxActionsTree->Expand( wxtIDRootNode);
// wxActionsTree->SetFocusedItem( wxActionsTree->GetFirstChild( wxtIDRootNode, cookie));
}

wxBoxSizer *HBox1 = new wxBoxSizer(wxHORIZONTAL);
HBox1->Add( wxActionsTree, 0, wxEXPAND);

// Buttons to control the tree
		  btnAdd = new wxButton(myPanel, ID_ADD, wstrAddButtonName, wxDefaultPosition, wxSize(70, 30));
btnRemove = new wxButton(myPanel, ID_DELETE, wstrRemove, wxDefaultPosition, wxSize(70, 30));
btnReplicate = new wxButton(myPanel, ID_Replicate, wstrReplicateButtonName, wxDefaultPosition, wxSize(70, 30));
btnRename = new wxButton(myPanel, ID_RENAME, wstrRenameButtonName, wxDefaultPosition, wxSize(70, 30));
// btnActionProperties = new wxButton(myPanel, ID_PROPERTIES_BUTTON, wstrPropertiesButtonName, wxDefaultPosition, wxSize(70, 30));
btnUp = new wxButton(myPanel, ID_UP, wstrUpButtonName, wxDefaultPosition, wxSize(70, 30));
  btnDown = new wxButton(myPanel, ID_DOWN, wstrDownButtonName, wxDefaultPosition, wxSize(70, 30));

  wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
hBoxButtons->Add( btnRename, 0, wxEXPAND);
hBoxButtons->Add( btnReplicate, 0, wxEXPAND);
  hBoxButtons->Add(btnAdd, 0, wxEXPAND);
hBoxButtons->Add(btnRemove, 0, wxEXPAND);
  // hBoxButtons->Add(btnActionProperties, 0, wxEXPAND);
    hBoxButtons->Add( btnUp, 0, wxEXPAND);
  hBoxButtons->Add( btnDown, 0, wxEXPAND);
    
  wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
vBox->Add(HBox1, 0, wxEXPAND);  
  vBox->Add(hBoxButtons, 0, wxEXPAND);

// Enable/disable buttons appropriately
ToggleButtons();

SetSizerAndFit(vBox);
}


ActionsPage::~ActionsPage()
{}


std::map <std::wstring, MessageDefinition> ActionsPage::GetActions()
{
	return myControls;
}


SurfaceActionType ActionsPage::GetSurfaceActionTypeFromNode( wxTreeItemId myCurrentNode)
{
wxTreeItemId wxtIDCurrentNode = myCurrentNode;
std::wstring strHash = GetControlHash( wxtIDCurrentNode);
	
if (strHash.empty())
	{
		return NoAction;
}

std::map <std::wstring, MessageDefinition> ::iterator it;
MessageDefinition myDefinition;

it = myControls.find( strHash);

if (it == myControls.end())
	{
return NoAction;
}
else
{
	myDefinition = it->second;
}

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentNode );
SurfaceActions myActions; 

try
	{
		myActions = myDefinition.GetSurfaceActions( myProduct);
}
catch( RBException &myException)
	{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

return  NoAction;		
}

if (IsLiveModeNode( wxtIDCurrentNode))
{
	return myActions.GetLiveModeAction( GetChildNumber( wxtIDCurrentNode)).GetSurfaceActionType();
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentNode))
{
	return myActions.GetScreenReaderModeAction( GetChildNumber( wxtIDCurrentNode)).GetSurfaceActionType();
}  // end if ScreenReader mode
	}  // end else for valid modes
	
return NoAction;
}


SurfaceAction ActionsPage::GetSurfaceAction()
	{
SurfaceAction myNewSurfaceAction;
myNewSurfaceAction.SetSurfaceActionType( GetSurfaceActionTypeFromNode( wxActionsTree->GetFocusedItem()));
return myNewSurfaceAction;
}


int ActionsPage::GetChildNumber( wxTreeItemId wxtIDCurrentNode)
{
wxTreeItemId myNode = wxtIDCurrentNode;
int nChildNumber = 0;

while (wxActionsTree->GetPrevSibling( myNode).IsOk())
{
	nChildNumber++;
	myNode = wxActionsTree->GetPrevSibling( myNode);
}

return nChildNumber;
}


int ActionsPage::GetLevel( wxTreeItemId myNode)
	{
int nLevel = 0;
wxTreeItemId wxtIDCurrentNode = myNode;
    
while (wxtIDCurrentNode .IsOk())
{
	wxtIDCurrentNode = wxActionsTree->GetItemParent( wxtIDCurrentNode);
	if (wxtIDCurrentNode.IsOk())
	{
nLevel++;
	}
}
				
return nLevel;
}


void ActionsPage::ToggleButtons()
{
	wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem();
	
	// Disable/enable controls, depending on the currently-selected level of the tree
switch (GetLevel( wxtIDCurrentNode))
	{
case PROTOCOL_LEVEL:
{
btnAdd->Disable();
	btnRemove->Disable();
	// btnActionProperties->Disable();
	btnRename->Disable();
	btnReplicate->Disable();
	btnUp->Disable();
	btnDown->Disable();
}
break;

case CONTROL_LEVEL:  // Allow control to be renamed
	{
	btnAdd->Disable();
	btnReplicate->Disable();	
	btnRemove->Disable();
	// btnActionProperties->Disable();
	btnRename->Enable();
	btnUp->Disable();
	btnDown->Disable();
}
	break;

case SPEECH_LEVEL:  // Allow control to be renamed
	{
	btnAdd->Disable();
	btnReplicate->Enable();	
	btnRemove->Disable();
	// btnActionProperties->Disable();
	btnRename->Enable();
	btnUp->Disable();
	btnDown->Disable();
}
	break;

case MODE_LEVEL:    // Can only manipulate actions when focused on the level showing the controls for a selected mode
	{
	btnAdd->Enable();
	btnRemove->Disable();
	// btnActionProperties->Disable();
	btnRename->Disable();
	btnUp->Disable();
	btnDown->Disable();
}
break;

case ACTION_LEVEL:  // Manipulate actions
{
	btnAdd->Enable();
	btnRemove->Enable();

	int nChildNumber = GetChildNumber( wxtIDCurrentNode);
	int nChildCount = wxActionsTree->GetChildrenCount( wxActionsTree->GetItemParent( wxtIDCurrentNode));
	
if (nChildCount >= 2)
	{
if (nChildNumber > 0)
	{
		btnUp->Enable();
}
else
{
	btnUp->Disable();
}

if (nChildNumber < (nChildCount -1))
{
	btnDown->Enable();
}
else
{
btnDown->Disable();
}
}  // end if count > 1
else  // No movement possible
	{
		btnUp->Disable();		
		btnDown->Disable();
}

SurfaceActionType myActionType; 
myActionType = GetSurfaceActionTypeFromNode( wxActionsTree->GetFocusedItem());

switch( myActionType)
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
	// btnActionProperties->Enable();
}
break;

default:
	{
		// btnActionProperties->Disable();
}
	break;
}  // end switch

btnRename->Disable();
	btnUp->Enable();
	btnDown->Enable();
}
break;

case PROPERTIES_LEVEL:  // Manipulate actions
{
	btnAdd->Enable();
	btnRemove->Enable();
	// btnActionProperties->Enable();
btnRename->Disable();
	btnUp->Enable();
	btnDown->Enable();
}
break;

default:
{
btnAdd->Disable();
	btnRemove->Disable();
	btnRename->Disable();
	btnUp->Disable();
	btnDown->Disable();
}
break;
}  // end switch
}


std::wstring ActionsPage::GetControlName( std::wstring wstrName)
{
		std::map <std::wstring, MessageDefinition> ::iterator it;
		
	for (it = myControls.begin(); it != myControls.end(); it++)
{
	if (it->second.GetLabel() == wstrName)
		{
			return it->first;
}  // end if found in map
		}  // end for
		
// Not found
	std::wstring strNotFound;
	strNotFound.clear();
	return strNotFound;
}


std::wstring ActionsPage::GetControlHash( wxTreeItemId myCurrentNode )
{
wxTreeItemId wxtIDCurrentNode = myCurrentNode;
	
if (GetLevel( wxtIDCurrentNode) == PROTOCOL_LEVEL)
{
	std::wstring strNotFound;
	strNotFound.clear();
	return strNotFound;
	}

// find the correct level in order to get the control name
while (GetLevel( wxtIDCurrentNode)> CONTROL_LEVEL)
{
wxtIDCurrentNode = wxActionsTree->GetItemParent( wxtIDCurrentNode);
}

std::wstring wstrControlName = 		wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
// Now find the matching hash value
MessageDefinition myMessageDefinition;

		return GetControlName( wstrControlName);
}


std::wstring ActionsPage::GetMode( wxTreeItemId myCurrentNode)
{
wxTreeItemId wxtIDCurrentNode = myCurrentNode;

if (GetLevel( wxtIDCurrentNode) > CONTROL_LEVEL)
{
// find the correct level in order to get the control name
while (GetLevel( wxtIDCurrentNode)> MODE_LEVEL)
{
wxtIDCurrentNode = wxActionsTree->GetItemParent( wxtIDCurrentNode);
}

return wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
}
else
	{
		std::wstring wstrNotFound;
		wstrNotFound.clear();
		return wstrNotFound;
}
}


ActiveProduct ActionsPage::GetProductFromNode( wxTreeItemId wxtIDCurrentNode)
{
	ActiveProduct myProduct = ID_NONE;

	if (GetLevel( wxtIDCurrentNode) < SPEECH_LEVEL)
{
return myProduct;
}

	wxTreeItemId myNode = wxtIDCurrentNode;
    
while ((myNode.IsOk()) && (GetLevel( myNode) > SPEECH_LEVEL))
{
	myNode = wxActionsTree->GetItemParent( myNode);
	}  // end while
				
myProduct = GetProductFromName( wxActionsTree->GetItemText( myNode).ToStdWstring());
return myProduct;
}


ActiveProduct ActionsPage::GetProductFromName( std::wstring wstrName)
{
ActiveProduct myProduct = ID_NONE;

if (wstrName.compare( wstrNone) == 0)
{
		return ID_NONE;
	}
	
if (wstrName.compare( wstrJaws) == 0)
{
		return ID_JAWS;
	}
	
if (wstrName.compare( wstrSA) == 0)
{
		return ID_SYSTEMACCESS;
	}
	
if (wstrName.compare( wstrWE) == 0)
{
		return ID_WindowEyes;
	}
	
if (wstrName.compare( wstrNVDA) == 0)
{
		return ID_NVDA;
	}
	
if (wstrName.compare( wstrDolphin) == 0)
{
		return ID_DOLPHIN;
	}
	
if (wstrName.compare( wstrMac) == 0)
{
		return ID_MAC;
	}
	
return myProduct;
}


std::wstring ActionsPage::GetActionParameterDescription( SurfaceAction mySA)
{
std::wstring wstrDescription;
unsigned int nParameters = mySA.GetParameterCount();

for (unsigned int i = 0; i < nParameters; i++)
{
	switch (mySA.GetSurfaceActionType())
		{
	case ReadDisplay:
		case ReadDisplayLine:
		case ReadDisplayStrip:
		case ReadDisplaySection:
		case ReadLEDIfOn:
		case ReadLEDIfOff:
			{
if (i == 0)
	{
		// Get the name of the targeted display
	std::wstring strDisplayHash = mySA.GetParameter(i).second;
		wstrDescription.append( pMyAppConfig->GetDisplayDefinition( strDisplayHash).GetLabel());
}
else
			{
		wstrDescription.append( mySA.GetParameter( i).second);
			}
			}
			break;

default:
	{
		wstrDescription.append( mySA.GetParameter( i).second);
			}
break;
}  // end switch

if (i < (nParameters -1))
	{
wstrDescription.append( wstrSpacedComma);
}
}  // end for
	
return wstrDescription;
}


bool ActionsPage::IsLiveModeNode( wxTreeItemId wxtIDCurrentNode)
{
	bool blnResult = false;

	if (GetLevel(wxtIDCurrentNode) < MODE_LEVEL)
{
return blnResult;
}

	wxTreeItemId myNode = wxtIDCurrentNode;
    
while ((myNode.IsOk()) && (GetLevel( myNode) > MODE_LEVEL))
{
	myNode = wxActionsTree->GetItemParent( myNode);
	}  // end while
				
if (wxActionsTree->GetItemText( myNode).ToStdWstring().compare( wstrLiveMode) == 0)
{
blnResult = true;
}

return blnResult;
}


bool ActionsPage::IsScreenReaderModeNode( wxTreeItemId wxtIDCurrentNode)
{
	bool blnResult = false;

if (GetLevel(wxtIDCurrentNode) < MODE_LEVEL)
{
return blnResult;
}

	wxTreeItemId myNode = wxtIDCurrentNode;
    
while ((myNode.IsOk()) && (GetLevel( myNode) > MODE_LEVEL))
{
	myNode = wxActionsTree->GetItemParent( myNode);
	}  // end while
				
if (wxActionsTree->GetItemText( myNode).ToStdWstring().compare( wstrScreenReaderMode) == 0)
{
blnResult = true;
}

return blnResult;
}


void ActionsPage::GetDisplayParameters()
{
SurfaceAction myNewSurfaceAction;

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
std::wstring strDisplayHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrDisplayHash( strDisplayHash.begin(), strDisplayHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrDisplayHash);
}
break;

case ReadDisplayLine:
{
	std::wstring strDisplayHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrDisplayHash( strDisplayHash.begin(), strDisplayHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrDisplayHash);
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayLine()));
}
break;

case ReadDisplayStrip:
{
	std::wstring strDisplayHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrDisplayHash( strDisplayHash.begin(), strDisplayHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrDisplayHash);
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayStrip()));
}
break;

case ReadDisplaySection:
{
	std::wstring strDisplayHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrDisplayHash( strDisplayHash.begin(), strDisplayHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrDisplayHash);
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplayStartPosition()));
	myNewSurfaceAction.SetParameter(Type_Int, boost::lexical_cast <std::wstring> (myDisplayParamsDlg->GetDisplaySubstringLength()));
}
break;

case ReadLEDIfOn:
	case ReadLEDIfOff:
{
std::wstring strDisplayHash = myDisplayParamsDlg->GetDisplayHash();
std::wstring wstrDisplayHash( strDisplayHash.begin(), strDisplayHash.end());
	myNewSurfaceAction.SetParameter(Type_String, wstrDisplayHash);
}
break;

default:
	{}
	break;
}  // end switch
	}  // end if valid
	}  // end if OK
}


	void ActionsPage::GetScreenReaderFunction()
	{
	SurfaceAction myNewSurfaceAction;
	ScreenReaderFunctionsDlg * myScreenReaderFunctionsDlg = new ScreenReaderFunctionsDlg( wstrScreenReaderFunctionsTitle, myNewSurfaceAction.GetSurfaceActionType(), pMyAppConfig, pMySpeech, pMyJawsCache);	

 	 		if (myScreenReaderFunctionsDlg->ShowModal() == wxID_OK)
{
	if (myScreenReaderFunctionsDlg->IsValidParameterSet())
	{
// populate the parameters here:
		myNewSurfaceAction.SetParameter(Type_WString, myScreenReaderFunctionsDlg->GetFunctionName());
	}  // end if valid
}  // end if OK
	}


bool ActionsPage::OtherScreenReadersContainActions( wxTreeItemId wxtIDCurrentNode)
	{
		wxTreeItemId myNode = wxtIDCurrentNode;

if (GetLevel( myNode) < SPEECH_LEVEL )
{
throw RBException(wstrNotScreenReaderActionNodeError);
}

while (GetLevel( myNode)> CONTROL_LEVEL)
{
myNode = wxActionsTree->GetItemParent( myNode);
}

// Iterate through the screen-readers
myNode = wxActionsTree->GetFirstChild( myNode, cookie);
ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentNode);

while (wxActionsTree->GetNextSibling( myNode).IsOk())
{
if (myProduct != GetProductFromNode( myNode))
{
// Figure out if this node contains any live or screen-reader mode actions
	wxTreeItemId myActionsNode = wxActionsTree->GetFirstChild( myNode, cookie);  // Live mode

	if (wxActionsTree->GetFirstChild( myActionsNode, cookie).IsOk())  
	{
		return true;
	}

myActionsNode = wxActionsTree->GetNextSibling( myActionsNode);  // ScreenReader mode

if (wxActionsTree->GetFirstChild( myActionsNode, cookie).IsOk()) 
	{
		return true;
	}
}

myNode = wxActionsTree->GetNextSibling( myNode);
}
		
return false;
	}


std::vector <std::wstring> ActionsPage::ReplicateActions( wxTreeItemId wxtIDCurrentNode, ActiveProduct myProduct)
{
	std::vector <std::wstring> vFailedActions;		

	// find the correct level in order to get the control names
	wxTreeItemId myNode = wxActionsTree->GetFirstChild( wxtIDRootNode, cookie);

// Iterate through the controls
	while (wxActionsTree->GetNextSibling( myNode).IsOk())
	{
std::wstring strHash = GetControlHash( myNode);		

if (strHash.empty() == false)
{
bool blnActions = false;
std::map <std::wstring, MessageDefinition>::iterator it;
it = myControls.find( strHash);

if (it != myControls.end())
	{
		SurfaceActions mySurfaceActions;
		
		try
			{
				mySurfaceActions = it->second.GetSurfaceActions( myProduct);
		blnActions = true;
		}
		catch( RBException &myException)
		{
		#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif
}

		if (blnActions)
		{
		// Strip out any screen-reader actions
vFailedActions = 		mySurfaceActions.DeleteScreenReaderActionTypes();

		// Iterate through the available products
		wxTreeItemId myProductNode = wxActionsTree->GetFirstChild( myNode, cookie); 

for  (unsigned int i =ID_JAWS; i <= ID_MAC; i++)
{
ActiveProduct myTargetProduct = static_cast<ActiveProduct>( i); 	

if (myTargetProduct != myProduct)
	{
it->second.SetSurfaceActions( myTargetProduct,mySurfaceActions); 
		wxActionsTree->DeleteChildren( myProductNode);
ListModes( myProductNode);
}

myProductNode = wxActionsTree->GetNextSibling( myProductNode);
}  // end for product 
		}  // end if blnActions
}  // end if hash found
}  // end if hash not empty

myNode = wxActionsTree->GetNextSibling( myNode);
	}  // end of while for controls

	wxMessageBox( wstrReplicationComplete, wstrAppTitle, wxOK | wxICON_INFORMATION );
return vFailedActions;
}

	
// Event handlers

void ActionsPage::OnTreeSelect( wxTreeEvent& event)
{
ToggleButtons();
}


void ActionsPage::OnAdd(wxCommandEvent& event)
{
wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem(); 

// We can only define actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

// find the correct level in order to get the control name
while (GetLevel( wxtIDCurrentNode)> CONTROL_LEVEL)
{
wxtIDCurrentNode = wxActionsTree->GetItemParent( wxtIDCurrentNode);
}

std::wstring wstrControlName = 		wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
MessageDefinition myMessageDefinition;
	ActionPicker * myActionPicker = new ActionPicker( wstrPickActionTitle, wstrControlName, pMyAppConfig, pMySpeech, pMyJawsCache);	

		if (myActionPicker->ShowModal() == wxID_OK)
{
	if (myActionPicker->IsValidSurfaceAction())
	{
		SurfaceAction mySurfaceAction = myActionPicker->GetSurfaceAction();
		
		// Find the control to add the action to:
std::wstring strHash = GetControlName( wstrControlName);

if (strHash.empty() == false)
{
std::map <std::wstring, MessageDefinition>::iterator it;
it = myControls.find( strHash);

if (IsLiveModeNode( wxActionsTree->GetFocusedItem()))
	{
		it->second.SetLiveModeAction( myActiveProduct, mySurfaceAction);
}

if (IsScreenReaderModeNode( wxActionsTree->GetFocusedItem()))
	{
it->second.SetScreenReaderModeAction( myActiveProduct, mySurfaceAction);
}

std::wstring wstrNodeLabel = mySurfaceAction.GetSurfaceActionTypeDescription( mySurfaceAction.GetSurfaceActionType());
wstrNodeLabel.append( GetActionParameterDescription( mySurfaceAction));	
wxtIDCurrentNode = wxActionsTree->GetFocusedItem();
	
while (GetLevel( wxtIDCurrentNode)> MODE_LEVEL)
{
wxtIDCurrentNode = wxActionsTree->GetItemParent( wxtIDCurrentNode);
}

	wxtIDCurrentNode = wxActionsTree->AppendItem( wxtIDCurrentNode, wstrNodeLabel);

if (GetLevel( wxtIDCurrentNode) == MODE_LEVEL)
{
wxActionsTree->Expand( wxtIDCurrentNode);
wxActionsTree->SetFocusedItem( wxtIDCurrentNode); 
}  // if at mode level
}  // end if control found
}  // end if valid action
		}  // end if OK

		wxActionsTree->SetFocus();
			
// Tidy up
		myActionPicker->Destroy();
}


void ActionsPage::OnProperties(wxCommandEvent& event)
    {
// We can only edit actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

SurfaceAction mySurfaceAction;

switch (mySurfaceAction.GetSurfaceActionType())
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

case SpeakString:
{
		wxMessageBox( wstrNotYetImplementedError,                wstrAppTitle, wxOK | wxICON_INFORMATION );
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


void ActionsPage::OnRemove(wxCommandEvent& event)
	{
		wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem(); 

		// We can only edit actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

unsigned int nIndex = GetChildNumber( wxtIDCurrentNode);

if ((nIndex >= wxActionsTree->GetChildrenCount( wxActionsTree->GetItemParent( wxtIDCurrentNode)))
|| (GetLevel( wxtIDCurrentNode) != ACTION_LEVEL))
{
	return;
}

std::wstring strHash = GetControlHash( wxtIDCurrentNode );

if (strHash.empty())
	{
		return;
}

std::map <std::wstring, MessageDefinition> ::iterator it;

it = myControls.find( strHash);

if (it == myControls.end())
	{
return;
}

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentNode );
bool blnResult = false;
SurfaceActions myActions = it->second.GetSurfaceActions( myProduct);

if (IsLiveModeNode( wxtIDCurrentNode))
{
try
	{
blnResult = myActions.DeleteLiveModeAction( nIndex);
}
catch( RBException &myException)
	{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

return;		
}
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentNode))
{
	try
	{
		blnResult = myActions.DeleteScreenReaderModeAction( nIndex);
}
catch( RBException &myException)
	{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

return;		
}
}  // end if ScreenReader mode
	}  // end else for valid modes
	
// Update the controls if the deletion succeeded
if (blnResult)
	{
		it->second.SetSurfaceActions( myProduct, myActions);
		wxActionsTree->Delete( wxtIDCurrentNode);		
}  // end if successful deletion

	wxActionsTree->SetFocus();
			}
		

void ActionsPage::OnRename(wxCommandEvent& event)
	{
		wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem(); 

if (GetLevel( wxtIDCurrentNode) == CONTROL_LEVEL)
	{
		std::wstring wstrControlName = wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();

// Try to update the associated entry within the map of controls
std::wstring strFoundHash = GetControlName( wstrControlName);			
		
if (strFoundHash.length() >0)
		{
std::map <std::wstring, MessageDefinition>::iterator it;

it = myControls.find( strFoundHash);

if (it != myControls.end())
{
			// Store the control hash and the definition
						MessageDefinition myDefinition = it->second;
			
// Get a new control label
			RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewControlNamePrompt, wstrControlName);

			if (UserLabelDlg->ShowModal() == wxID_OK)
		{
			std::wstring wstrLabel = UserLabelDlg->GetText();
	
			if (wstrLabel.empty())
{
	wxMessageBox( wstrNoControlNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
} // end if label not empty
else
			{
				myDefinition.SetLabel( wstrLabel);				
								
// Update the map, then the tree node
std::pair <std::wstring, MessageDefinition> myDefinitionPair;
myDefinitionPair = std::make_pair( strFoundHash, myDefinition);
myControls.erase( it);
myControls.insert( myDefinitionPair);
wxActionsTree->SetItemText( wxtIDCurrentNode, wstrLabel);
wxActionsTree->SetFocus();
			}   // end label not empty
				} // end if input box returns OK
			// Tidy up
			UserLabelDlg->Destroy(); 
}  // end if hash found in map
}  // end if label found in map
else
{
	wxMessageBox( wstrNoMatchingLabelError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
	}  // end if at correct level
}


void ActionsPage::OnReplicate(wxCommandEvent& event)
{
wxTreeItemId myNode = wxActionsTree->GetFocusedItem(); 

if (GetLevel( myNode) < SPEECH_LEVEL)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
}

bool blnUnsafeReplication;

try
	{
		blnUnsafeReplication = OtherScreenReadersContainActions( myNode);
}
catch( RBException &myException)
{
wxMessageBox( myException.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

if (blnUnsafeReplication)
{
if (wxMessageBox( wstrReplicationCheck,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
return;
}
}
std::vector <std::wstring> vFailedActions = ReplicateActions( myNode, GetProductFromNode( myNode));

if (vFailedActions.size() > 0)
{
std::wstring wstrErrorText = wstrFailedActionsError;

for( unsigned int i = 0; i < vFailedActions.size(); i++)
	{
				wstrErrorText.append( vFailedActions.at( i));
	wstrErrorText.append( wstrNewLine);
}  // end for

wxMessageBox( wstrErrorText, wstrErrorTitle, wxOK | wxICON_ERROR);
					}  // end if vector not empty
}


void ActionsPage::OnUp(wxCommandEvent& event)
{
wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem(); 

// We can only define actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

int nIndex = GetChildNumber( wxtIDCurrentNode);

if ((nIndex == 0)
|| (GetLevel( wxtIDCurrentNode) != ACTION_LEVEL))
{
	return;
}

std::wstring strHash = GetControlHash( wxtIDCurrentNode );

if (strHash.length() ==0)
	{
		return;
}

std::map <std::wstring, MessageDefinition> ::iterator it;
MessageDefinition myDefinition;

it = myControls.find( strHash);

if (it == myControls.end())
	{
return;
}
else
{
	myDefinition = it->second;
}

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentNode );
SurfaceActions myActions; 

try
	{
		myActions = myDefinition.GetSurfaceActions( myProduct);
}
catch( RBException &myException)
	{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

return;		
}

bool blnResult;

if (IsLiveModeNode( wxtIDCurrentNode))
{
	blnResult = myActions.SwapLiveModeActions( (nIndex -1), nIndex);
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentNode))
{
	blnResult = myActions.SwapScreenReaderModeActions( nIndex, (nIndex -1));
}  // end if ScreenReader mode
	}  // end else for valid modes
	
if (blnResult == true)
	{
		if (IsLiveModeNode( wxtIDCurrentNode))
		{
std::wstring wstrAction = wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
			std::wstring wstrTempAction = wxActionsTree->GetItemText( wxActionsTree->GetPrevSibling( wxtIDCurrentNode)).ToStdWstring();

			wxActionsTree->SetItemText( wxActionsTree->GetPrevSibling( wxtIDCurrentNode), wstrAction);
			wxActionsTree->SetItemText( wxtIDCurrentNode, wstrTempAction);			
}  // end if live mode

		if (IsScreenReaderModeNode( wxtIDCurrentNode))
		{
			std::wstring wstrAction = wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
			std::wstring wstrTempAction = wxActionsTree->GetItemText( wxActionsTree->GetPrevSibling( wxtIDCurrentNode)).ToStdWstring();

			wxActionsTree->SetItemText( wxActionsTree->GetPrevSibling( wxtIDCurrentNode), wstrAction);
			wxActionsTree->SetItemText( wxtIDCurrentNode, wstrTempAction);			
}  // end if screen-reader mode
}  // end if swap was successful

	wxActionsTree->SetFocus();
}


void ActionsPage::OnDown(wxCommandEvent& event)
{
wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem(); 

// We can only define actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

int nIndex = GetChildNumber( wxtIDCurrentNode);

if ((nIndex == wxActionsTree->GetChildrenCount( wxActionsTree->GetItemParent( wxtIDCurrentNode)) -1)
|| (GetLevel( wxtIDCurrentNode) != ACTION_LEVEL))
{
	return;
}

std::wstring strHash = GetControlHash( wxtIDCurrentNode );

if (strHash.empty())
	{
		return;
}

std::map <std::wstring, MessageDefinition> ::iterator it;
MessageDefinition myDefinition;

it = myControls.find( strHash);

if (it == myControls.end())
	{
return;
}
else
{
	myDefinition = it->second;
}

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentNode );
SurfaceActions myActions; 

try
	{
		myActions = myDefinition.GetSurfaceActions( myProduct);
}
catch( RBException &myException)
	{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

return;		
}

bool blnResult;

if (IsLiveModeNode( wxtIDCurrentNode))
{
	blnResult = myActions.SwapLiveModeActions( nIndex, (nIndex +1));
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentNode))
{
	blnResult = myActions.SwapScreenReaderModeActions( nIndex, (nIndex +1));
}  // end if ScreenReader mode
	}  // end else for valid modes
	
if (blnResult == true)
	{
		if (IsLiveModeNode( wxtIDCurrentNode))
		{
std::wstring wstrAction = wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
std::wstring wstrTempAction = wxActionsTree->GetItemText( wxActionsTree->GetNextSibling( wxtIDCurrentNode)).ToStdWstring();

wxActionsTree->SetItemText( wxActionsTree->GetNextSibling( wxtIDCurrentNode), wstrAction);
			wxActionsTree->SetItemText( wxtIDCurrentNode, wstrTempAction);			
}  // end if live mode

		if (IsScreenReaderModeNode( wxtIDCurrentNode))
		{
			std::wstring wstrAction = wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
			std::wstring wstrTempAction = wxActionsTree->GetItemText( wxActionsTree->GetNextSibling( wxtIDCurrentNode)).ToStdWstring();

			wxActionsTree->SetItemText( wxActionsTree->GetNextSibling( wxtIDCurrentNode), wstrAction);
			wxActionsTree->SetItemText( wxtIDCurrentNode, wstrTempAction);			
}  // end if screen-reader mode
}  // end if swap was successful

	wxActionsTree->SetFocus();
}


		void ActionsPage::OnWizardPageChanging(wxWizardEvent& event)
    {
		pMyAppConfig->SetHardwareControls( myControls);
		}


void ActionsPage::OnWizardCancel(wxWizardEvent& event)
    {
		if (		wxMessageBox( wstrActionsCheck,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
            event.Veto();
	}
		}


	ActiveProduct ActionsPage::GetActiveProduct()
	{
return myActiveProduct;
	}


std::wstring ActionsPage::GetActiveProductName( ActiveProduct apID)
	{
switch (apID)
{
case ID_NONE:
	{
		return wstrNone;
	}
	break;

case ID_JAWS:
	{
		return wstrJaws;
	}
	break;

case ID_SYSTEMACCESS:
	{
		return wstrSA;
	}
	break;

case ID_WindowEyes:
	{
		return wstrWE;
	}
	break;

case ID_NVDA:
	{
		return wstrNVDA;
	}
	break;

case ID_DOLPHIN:
{
		return wstrDolphin;
	}
	break;

case ID_MAC:
{
return wstrMac;
}
break;

default:
{
		return wstrNone;
	}
	break;
}  // end switch
	}

	
	bool ActionsPage::IsActiveProductBranch()
	{
	int nLevel = 0;
wxTreeItemId wxtIDCurrentNode = wxActionsTree->GetFocusedItem();

if (GetLevel( wxtIDCurrentNode) < SPEECH_LEVEL) 
{
	return false;
}

while (GetLevel( wxtIDCurrentNode) != SPEECH_LEVEL)
{
	wxtIDCurrentNode = wxActionsTree->GetItemParent( wxtIDCurrentNode);
	}
				
std::wstring wstrProductName = wxActionsTree->GetItemText( wxtIDCurrentNode).ToStdWstring();

if (wstrProductName.compare( GetActiveProductName( myActiveProduct)) == 0)
{
return true;
}
else
{
/*
std::wstring wstrOut = L"Node name = ";
wstrOut.append( wstrDoubleQuotes);
wstrOut.append( wstrProductName);
wstrOut.append( wstrDoubleQuotes).append( wstrNewLine);
wstrOut.append( "Active product = ").append( wstrDoubleQuotes);
wstrOut.append( GetActiveProductName( myActiveProduct)).append( wstrDoubleQuotes);
wxMessageBox( wstrOut, wxT( "Test"), wxOK | wxICON_ERROR);
*/
	return false;
}
}


	void ActionsPage::ListControls( wxTreeItemId wxtIDCurrentNode)
	{
			  // Create a branch for every available hardware control
				std::map <std::wstring, MessageDefinition>::iterator it;

for (it = myControls.begin(); it != myControls.end(); it++)
{
	wxTreeItemId wxtIDNewNode = wxActionsTree->AppendItem( wxtIDCurrentNode, it->second.GetLabel());
	ListScreenReaders( wxtIDNewNode);
}
}


	void ActionsPage::ListScreenReaders( wxTreeItemId wxtIDCurrentNode )
	{
// Create a child node for every available speech product
for  (unsigned int i =ID_JAWS; i <= ID_MAC; i++)
{
ActiveProduct myActiveProduct = static_cast<ActiveProduct>( i); 	
wxTreeItemId wxtIDNewNode = wxActionsTree->AppendItem( wxtIDCurrentNode , GetActiveProductName( myActiveProduct));
	ListModes( wxtIDNewNode);
}
	}

		
	void ActionsPage::ListModes( wxTreeItemId wxtIDCurrentNode )
	{
	wxTreeItemId wxtIDNewNode; 
		// Each control branch has a Live Mode and a Screen-Reader Mode vector of actions, so we need 2 nodes at the next level
wxtIDNewNode = wxActionsTree->AppendItem( wxtIDCurrentNode, wstrLiveMode);
ListActions( wxtIDNewNode);
wxtIDNewNode = wxActionsTree->AppendItem( wxtIDCurrentNode, wstrScreenReaderMode);
ListActions( wxtIDNewNode);
	}


	void ActionsPage::ListActions( wxTreeItemId wxtIDCurrentNode )
	{
std::wstring strHash = GetControlHash( wxtIDCurrentNode );

if (strHash.empty())
	{
		return;
}

std::map <std::wstring, MessageDefinition> ::iterator it;
MessageDefinition myDefinition;

it = myControls.find( strHash);

if (it == myControls.end())
	{
return;
}
else
{
	myDefinition = it->second;
}

// Create a child node for every defined SurfaceAction contained within this hardware control
ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentNode );
SurfaceActions myActions; 

try
	{
		myActions = myDefinition.GetSurfaceActions( myProduct);
}
catch( RBException &myException)
	{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

return;		
}

wxTreeItemId wxtIDNewNode; 

	if (IsLiveModeNode( wxtIDCurrentNode))
{
	for  (int i = 0; i < myActions.GetLiveModeActionCount(); i++)
	{
		try
			{
		std::wstring wstrNodeLabel = myActions.GetLiveModeAction( i).GetSurfaceActionTypeDescription( myActions.GetLiveModeAction( i).GetSurfaceActionType());
wstrNodeLabel.append( GetActionParameterDescription( myActions.GetLiveModeAction( i)));	
wxtIDNewNode = wxActionsTree->AppendItem( wxtIDCurrentNode , wstrNodeLabel);
		}  // end try
catch( ...)
{
break;
}
	}  // end for
		return;
	}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentNode))
{
	for  (int i = 0; i < myActions.GetScreenReaderModeActionCount(); i++)
	{
		try
			{
				std::wstring wstrNodeLabel = myActions.GetScreenReaderModeAction( i).GetSurfaceActionTypeDescription( myActions.GetScreenReaderModeAction( i).GetSurfaceActionType());
				wstrNodeLabel.append( GetActionParameterDescription( myActions.GetScreenReaderModeAction( i)));	
wxtIDNewNode = wxActionsTree->AppendItem( wxtIDCurrentNode , wstrNodeLabel);
		}  // end try
	catch( ...)
	{
break;
	} // end catch
	}  // end for
	
return;
}  // end if ScreenReader mode
	}  // end else for valid modes
	
	return;
	}

		
	// We need to implement an event table in which the events received by an ActionsTree are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ActionsPage, wxWizardPageSimple)
EVT_TREE_SEL_CHANGED( ID_ACTION_TREE, ActionsPage::OnTreeSelect)

// Buttons
EVT_BUTTON(ID_ADD,  ActionsPage::OnAdd)	    
EVT_BUTTON(ID_RENAME,  ActionsPage::OnRename)
EVT_BUTTON(ID_Replicate,  ActionsPage::OnReplicate)
EVT_BUTTON(ID_PROPERTIES_BUTTON,  ActionsPage::OnProperties)
EVT_BUTTON( ID_UP,                 ActionsPage::OnUp)
EVT_BUTTON( ID_DOWN,                 ActionsPage::OnDown)
EVT_BUTTON( ID_DELETE,                 ActionsPage::OnRemove)

// Wizard page monitoring
EVT_WIZARD_CANCEL(wxID_ANY, ActionsPage::OnWizardCancel)
// EVT_WIZARD_PAGE_CHANGED(wxID_ANY, ActionsPage::OnWizardPageChanged)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY, ActionsPage::OnWizardPageChanging)
    END_EVENT_TABLE()
