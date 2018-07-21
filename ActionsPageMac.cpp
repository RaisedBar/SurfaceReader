// ActionsPageMac.cpp
// Implementation of the Apple Mac version of the ActionsPage class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifdef __WXOSX_COCOA__   
#include "ActionsPageMac.h"


ActionsPage::ActionsPage(wxWizard * wizParent, const wxString & title, boost::shared_ptr <AppConfig> pAppConfig, boost::shared_ptr <SurfaceProtocol> pProtocol, boost::shared_ptr <RBSpeech> pSpeech)
       : wxWizardPageSimple( wizParent),
	pMyAppConfig( new AppConfig ()),
	pMyProtocol( new SurfaceProtocol ()),
	pMySpeech( new RBSpeech ())
{
	pMyAppConfig = pAppConfig;
	pMyProtocol = pProtocol;
	pMySpeech = pSpeech;

	myControls = pMyAppConfig->GetHardwareControls();
myOldActiveProduct =myActiveProduct;
   pMySpeech->GetActiveProduct( myActiveProduct, myDolphinProduct);
      
  wxPanel *myPanel = new wxPanel(this, -1);
    BuildDataView( myPanel);
	
      const wxSizerFlags border = wxSizerFlags().DoubleBorder();

// Buttons to control the tree
		  btnAdd = new wxButton(myPanel, ID_ADD, wstrAddButtonName, wxDefaultPosition, wxSize(70, 30));
btnRemove = new wxButton(myPanel, ID_DELETE, wstrRemove, wxDefaultPosition, wxSize(70, 30));
btnReplicate = new wxButton(myPanel, ID_Replicate, wstrReplicateButtonName, wxDefaultPosition, wxSize(70, 30));
btnRename = new wxButton(myPanel, ID_RENAME, wstrRenameButtonName, wxDefaultPosition, wxSize(70, 30));
// btnActionProperties = new wxButton(myPanel, ID_PROPERTIES_BUTTON, wxstrPropertiesButtonName, wxDefaultPosition, wxSize(70, 30));
btnUp = new wxButton(myPanel, ID_UP, 
					 wstrUpButtonName, wxDefaultPosition, wxSize(70, 30));
  btnDown = new wxButton(myPanel, ID_DOWN, wstrDownButtonName, wxDefaultPosition, wxSize(70, 30));

  wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
hBoxButtons->Add( btnRename, 0, wxEXPAND);
  hBoxButtons->Add( btnReplicate, 0, wxEXPAND);
  hBoxButtons->Add(btnAdd, 0, wxEXPAND);
hBoxButtons->Add(btnRemove, 0, wxEXPAND);
  // hBoxButtons->Add(btnActionProperties, 0, wxEXPAND);
    hBoxButtons->Add( btnUp, 0, wxEXPAND);
  hBoxButtons->Add( btnDown, 0, wxEXPAND);
    
  wxSizer *myPanelSz = new wxBoxSizer( wxVERTICAL );
myDataView->SetMinSize(wxSize(-1, 200));
    myPanelSz->Add(myDataView, 1, wxGROW|wxALL, 5);
        myPanelSz->Add(hBoxButtons);
        myPanel->SetSizerAndFit(myPanelSz);

    	// Enable/disable buttons appropriately
		  ToggleButtons();
 }


ActionsPage::~ActionsPage()
{}


void ActionsPage::BuildDataView( wxPanel* parent, unsigned long style)
{
myDataView  = new wxDataViewCtrl( parent, ID_ACTION_TREE, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxDV_NO_HEADER);
                        
myDataView ->Connect(wxEVT_CHAR, wxKeyEventHandler(ActionsPage::OnDataViewChar), NULL, this);
m_actions_model = new ActionsTreeModel( pMyProtocol->GetProtocolName().append( wstrProtocolActions), &myControls, pMyAppConfig);
	myDataView ->AssociateModel( m_actions_model.get() );            
                        	
            // column 0 of the view control (Protocol)
wxDataViewTextRenderer *tr0 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *ProtocolColumn = new wxDataViewColumn( "Protocol", tr0, PROTOCOL_LEVEL, 200, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE );
            myDataView ->AppendColumn( ProtocolColumn);

			// column 1 of the view control:
wxDataViewTextRenderer *tr1 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *ControlsColumn = new wxDataViewColumn( "Controls", tr1, CONTROL_LEVEL, 150, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );
                        myDataView ->AppendColumn( ControlsColumn);
						m_actions_model->Resort();

						// Enable sorting on this column at startup
#if 0
ControlsColumn->SetAsSortKey();
#endif

// column 2 of the view control:
wxDataViewTextRenderer *tr2 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *ScreenReaderColumn = new wxDataViewColumn( "Screen Reader", tr2, SPEECH_LEVEL, 60, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE );
            myDataView ->AppendColumn( ScreenReaderColumn);

// column 3 of the view control:
            wxDataViewTextRenderer *tr3 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *ModeColumn = new wxDataViewColumn( "Mode", tr3, MODE_LEVEL, 100, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE );
            myDataView ->AppendColumn( ModeColumn);

            // column 4 of the view control:
wxDataViewTextRenderer *tr4 = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
wxDataViewColumn *ActionsColumn = new wxDataViewColumn( "Actions", tr4, ACTION_LEVEL, 100, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE );
myDataView ->AppendColumn( ActionsColumn);
}

            
std::map <std::string, MessageDefinition> ActionsPage::GetActions()
{
	 return myControls;
}


SurfaceActionType ActionsPage::GetSurfaceActionTypeFromNode( wxDataViewItem wxtIDCurrentItem)
{
wxDataViewItem wxtIDItem = wxtIDCurrentItem;
std::string strHash = GetControlHash( wxtIDItem);
	
if (strHash.empty())
	{
		return NoAction;
}

std::map <std::string, MessageDefinition> ::iterator it;
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

ActiveProduct myProduct = GetProductFromNode( wxtIDItem);
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

if (IsLiveModeNode( wxtIDItem))
{
	return myActions.GetLiveModeAction( GetChildNumber( wxtIDItem)).GetSurfaceActionType();
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDItem))
{
	return myActions.GetScreenReaderModeAction( GetChildNumber( wxtIDItem)).GetSurfaceActionType();
}  // end if ScreenReader mode
	}  // end else for valid modes
	
return NoAction;
}


SurfaceAction ActionsPage::GetSurfaceAction()
	{
SurfaceAction myNewSurfaceAction;
// myNewSurfaceAction.SetSurfaceActionType( GetSurfaceActionTypeFromNode( myDataView ->GetFocusedItem()));
return myNewSurfaceAction;
}


int ActionsPage::GetChildNumber( wxDataViewItem wxtIDCurrentItem)
{
wxDataViewItem myParentNode = m_actions_model->GetParent( wxtIDCurrentItem);
	wxDataViewItem myNode = wxtIDCurrentItem;
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


wxDataViewItem ActionsPage::GetFirstChild( wxDataViewItem wxtIDCurrentNode)
{
wxDataViewItemArray myChildren;
	m_actions_model->GetChildren( wxtIDCurrentNode, myChildren);
return myChildren.Item( 0);
}	


wxDataViewItem ActionsPage::GetPrevSibling( wxDataViewItem wxtIDCurrentNode)
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

if (nChildNumber > 0)
	{
		return myChildren.Item( nChildNumber -1);
}
else
	{
		return wxtIDCurrentNode;
}
}


wxDataViewItem ActionsPage::GetNextSibling( wxDataViewItem wxtIDCurrentNode)
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

	if ((nChildNumber > -1)
		&& (nChildNumber < (myChildren.Count() -1)))
	{
		return myChildren.Item( nChildNumber +1);
}
else
	{
		return wxtIDCurrentNode;
}
}


void ActionsPage::ToggleButtons()
{
// Disable/enable controls, depending on the currently-selected level of the tree
	wxDataViewItem wxtIDCurrentItem = myDataView->GetCurrentItem();

		switch( m_actions_model->GetLevel( wxtIDCurrentItem))
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
	btnRemove->Disable();
	// btnActionProperties->Disable();
	btnRename->Enable();
		btnReplicate->Disable();	
btnUp->Disable();
	btnDown->Disable();
}
	break;

case MODE_LEVEL:    // Can only manipulate actions when focused on the level showing the controls for a selected mode
	{
	btnAdd->Enable();
	btnRemove->Disable();
		btnReplicate->Disable();	
btnRename->Disable();
	btnUp->Disable();
	btnDown->Disable();
// btnActionProperties->Disable();
	}
break;

case ACTION_LEVEL:  // Manipulate actions
{
	btnAdd->Enable();
	btnRemove->Enable();

	int nChildNumber = GetChildNumber( myDataView->GetCurrentItem());
		
	if (myDataView ->GetChildren().GetCount() >= 2)
	{
if (nChildNumber > 0)
	{
		btnUp->Enable();
}
else
{
	btnUp->Disable();
}

if (nChildNumber < (myDataView ->GetChildren().GetCount() -1))
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
myActionType = GetSurfaceActionTypeFromNode( myDataView ->GetCurrentItem());

switch( myActionType)
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


std::string ActionsPage::GetControlName( std::wstring wstrName)
{
		std::map <std::string, MessageDefinition> ::iterator it;
		
	for (it = myControls.begin(); it != myControls.end(); it++)
{
	if (it->second.GetLabel() == wstrName)
		{
			return it->first;
}  // end if found in map
		}  // end for
		
// Not found
	std::string strNotFound;
	strNotFound.clear();
	return strNotFound;
}


std::string ActionsPage::GetControlHash( wxDataViewItem wxtIDCurrentItem)
{
wxDataViewItem wxtIDCurrentNode = wxtIDCurrentItem;
	
	if (m_actions_model->GetLevel( wxtIDCurrentNode ) == PROTOCOL_LEVEL)
{
	std::string strEmpty;
	strEmpty.clear();
	return strEmpty;
	}

// find the correct level in order to get the control name
while (m_actions_model->GetLevel( wxtIDCurrentNode ) > CONTROL_LEVEL)
{
	wxtIDCurrentNode = m_actions_model->GetParent( wxtIDCurrentNode );
}

wxString wxstrControlName = 		m_actions_model->GetTitle( wxtIDCurrentNode);

// Now find the matching hash value
MessageDefinition myMessageDefinition;

		return GetControlName( wxstrControlName.ToStdWstring());
}


std::wstring ActionsPage::GetMode( wxDataViewItem wxtIDCurrentItem)
{
wxDataViewItem myNode = wxtIDCurrentItem;

if (m_actions_model->GetLevel( myNode) < MODE_LEVEL)
{
	return L"";
}

// find the correct level in order to get the control name
	while (m_actions_model->GetLevel( myNode) > MODE_LEVEL)
{
	myNode = m_actions_model->GetParent( myNode); 
}

	return m_actions_model->GetTitle( myNode);
}


ActiveProduct ActionsPage::GetProductFromNode( wxDataViewItem wxtIDCurrentItem)
{
	ActiveProduct myProduct = ID_NONE;

	if (m_actions_model->GetLevel( wxtIDCurrentItem) < SPEECH_LEVEL)
{
return myProduct;
}

	wxDataViewItem myNode = wxtIDCurrentItem;

	while ((myNode.IsOk()) && (m_actions_model->GetLevel( myNode) > SPEECH_LEVEL))
{
	myNode = m_actions_model->GetParent( myNode);
	}  // end while
				
	myProduct = GetProductFromName( m_actions_model->GetTitle( myNode));
return myProduct;
}


ActiveProduct ActionsPage::GetProductFromName( std::wstring  wstrName)
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
	
return myProduct;
}


std::wstring ActionsPage::GetActionParameterDescription( SurfaceAction mySA)
{
	std::wstring  wstrDescription;
unsigned int nParameters = mySA.GetParameterCount();

for (unsigned int i = 0; i < nParameters; i++)
{
	switch (mySA.GetSurfaceActionType())
		{
	case ReadDisplay:
		case ReadDisplayLine:
		case ReadDisplayStrip:
		case ReadDisplaySection:
			{
if (i == 0)
	{
		// Get the name of the targeted display
wxString wxstrDisplayHash = mySA.GetParameter( i).second;
		wstrDescription.append( pMyAppConfig->GetDisplayDefinition( wxstrDisplayHash.ToStdString()).GetLabel());
}
else
	{
		wstrDescription.append( mySA.GetParameter( i).second);
wstrDescription.append( wstrSpacedComma);
}
			}
			break;

default:
	{
		wstrDescription.append( mySA.GetParameter( i).second);

		if (i < (nParameters -1))
	{
wstrDescription.append( L", ");
}
			}
break;
}  // end switch
}  // end for
	
return wstrDescription;
}


bool ActionsPage::IsLiveModeNode( wxDataViewItem wxtIDCurrentItem)
{
	bool blnResult = false;

	if (m_actions_model->GetLevel( wxtIDCurrentItem) < MODE_LEVEL)
{
return blnResult;
}

while ((wxtIDCurrentItem.IsOk()) && (m_actions_model->GetLevel( wxtIDCurrentItem) > MODE_LEVEL))
{
	wxtIDCurrentItem = m_actions_model->GetParent( wxtIDCurrentItem);	
}  // end while
				
if (m_actions_model->GetTitle( wxtIDCurrentItem).compare( wstrLiveMode) == 0)
{
blnResult = true;
}

return blnResult;
}


bool ActionsPage::IsScreenReaderModeNode( wxDataViewItem wxtIDCurrentItem)
{
	bool blnResult = false;

	if (m_actions_model->GetLevel( wxtIDCurrentItem) < MODE_LEVEL)
{
return blnResult;
}

while ((wxtIDCurrentItem.IsOk()) && (m_actions_model->GetLevel( wxtIDCurrentItem) > MODE_LEVEL))
{
	wxtIDCurrentItem = m_actions_model->GetParent( wxtIDCurrentItem);	
}  // end while
				
if (m_actions_model->GetTitle( wxtIDCurrentItem).compare( wstrScreenReaderMode) == 0)
{
blnResult = true;
}

return blnResult;
}


void ActionsPage::GetDisplayParameters()
{
SurfaceAction myNewSurfaceAction;

DisplayParamsDlg * myDisplayParamsDlg = new DisplayParamsDlg( wxT( "Define display parameters"), myNewSurfaceAction.GetSurfaceActionType(), pMyAppConfig);	

 	 		if (myDisplayParamsDlg->ShowModal() == wxID_OK)
{
	if (myDisplayParamsDlg->IsValidParameterSet())
	{
// populate the parameters here:
switch (myNewSurfaceAction.GetSurfaceActionType())
{
case ReadDisplay:
{
	wxString wxstrDisplayHash = myDisplayParamsDlg->GetDisplayHash();
	myNewSurfaceAction.SetParameter(Type_String, wxstrDisplayHash.ToStdWstring());
}
break;

case ReadDisplayLine:
{
	wxString wxstrDisplayHash = myDisplayParamsDlg->GetDisplayHash();
	myNewSurfaceAction.SetParameter(Type_String, wxstrDisplayHash.ToStdWstring());
	myNewSurfaceAction.SetParameter(Type_Int, wxString::Format( "%d", myDisplayParamsDlg->GetDisplayLine()).ToStdWstring());
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


	void ActionsPage::GetScreenReaderFunction()
	{
	SurfaceAction myNewSurfaceAction;
	ScreenReaderFunctionsDlg * myScreenReaderFunctionsDlg = new ScreenReaderFunctionsDlg( wstrScreenReaderFunctionsTitle, myNewSurfaceAction.GetSurfaceActionType(), pMyAppConfig, pMySpeech);

 	 		if (myScreenReaderFunctionsDlg->ShowModal() == wxID_OK)
{
	if (myScreenReaderFunctionsDlg->IsValidParameterSet())
	{
// populate the parameters here:
		myNewSurfaceAction.SetParameter(Type_WString, myScreenReaderFunctionsDlg->GetFunctionName());
	}  // end if valid
}  // end if OK
	}


bool ActionsPage::OtherScreenReadersContainActions( wxDataViewItem wxtIDCurrentNode)
	{
		wxDataViewItem myNode = wxtIDCurrentNode;

if (m_actions_model->GetLevel( myNode) < SPEECH_LEVEL )
{
throw RBException(wstrNotScreenReaderActionNodeError);
}

while (m_actions_model->GetLevel( myNode)> CONTROL_LEVEL)
{
	myNode = m_actions_model->GetParent( myNode);
}

// Iterate through the screen-readers
myNode = GetFirstChild( myNode);
ActiveProduct myProduct = GetProductFromNode( myNode);

while (GetNextSibling( myNode).IsOk())
{
if (myProduct != GetProductFromNode( myNode))
{
// Figure out if this node contains any live or screen-reader mode actions
	wxDataViewItem myActionsNode = GetFirstChild( myNode);  // Live mode

	if (GetFirstChild( myActionsNode).IsOk())  
	{
		return true;
	}

myActionsNode = GetNextSibling( myActionsNode);  // ScreenReader mode

if (GetFirstChild( myActionsNode).IsOk()) 
	{
		return true;
	}
}

myNode = GetNextSibling( myNode);
}
		
return false;
	}


std::vector <std::wstring> ActionsPage::ReplicateActions( wxDataViewItem wxtIDCurrentNode, ActiveProduct myProduct)
{
wxDataViewItem myNode = wxtIDCurrentNode;	
	std::vector <std::wstring> vFailedActions;		

	// find the correct level in order to get the control names
	while (m_actions_model->GetLevel( myNode) > PROTOCOL_LEVEL)
{
	myNode = m_actions_model->GetParent( myNode);
}

	myNode = GetFirstChild( myNode);

// Iterate through the controls
	while (GetNextSibling( myNode).IsOk())
	{
std::string strHash = GetControlHash( myNode);		

if (strHash.empty() == false)
{
bool blnActions = false;
std::map <std::string, MessageDefinition>::iterator it;
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
		wxDataViewItem myProductNode = GetFirstChild( myNode); 

for  (unsigned int i =ID_JAWS; i <= ID_MAC; i++)
{
ActiveProduct myTargetProduct = static_cast<ActiveProduct>( i); 	

if (myTargetProduct != myProduct)
	{
		it->second.SetSurfaceActions( myTargetProduct,mySurfaceActions); 
myProductNode = GetNextSibling( myProductNode);
}
}  // end for product 
		}  // end if blnActions
}  // end if hash found
}  // end if hash not empty

myNode = GetNextSibling( myNode);
	}  // end of while for controls

			// Update/Refresh
// m_actions_model->AddActionNodes();
myDataView->Refresh();
wxMessageBox( wstrReplicationComplete, wstrAppTitle, wxOK | wxICON_INFORMATION );
return vFailedActions;
}

	
// Event handlers


	void ActionsPage::OnSelectionChanged( wxDataViewEvent &event )
{
		ToggleButtons();
	
	wxString title = m_actions_model->GetTitle( event.GetItem() );
    
	if (title.empty())
        {
			title = "None";
	}

    // wxLogMessage( "wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, First selected Item: %s", title );
}


void ActionsPage	::OnDataViewChar(wxKeyEvent& event)
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


void ActionsPage::OnAdd(wxCommandEvent& event)
{
	    		wxDataViewItem wxtIDCurrentItem = myDataView ->GetCurrentItem(); 
					
// We can only define actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

// find the correct level in order to get the control name
while (m_actions_model->GetLevel( wxtIDCurrentItem) > CONTROL_LEVEL)
{
	wxtIDCurrentItem = m_actions_model->GetParent( wxtIDCurrentItem);
}

	std::wstring wstrControlName = m_actions_model->GetTitle( wxtIDCurrentItem);
MessageDefinition myMessageDefinition;
	ActionPicker * myActionPicker = new ActionPicker( wstrPickActionTitle, wstrControlName, pMyAppConfig, pMyProtocol, pMySpeech);

		if (myActionPicker->ShowModal() == wxID_CANCEL)
{
	return;
		}
		
		if (myActionPicker->IsValidSurfaceAction() == false)
	{
		return;
		}
		
		SurfaceAction mySurfaceAction = myActionPicker->GetSurfaceAction();
		
		// Find the control to add the action to:
std::string strHash = GetControlName( wstrControlName);

if (strHash.empty())
{
return;
}

std::map <std::string, MessageDefinition>::iterator it;
it = myControls.find( strHash);

if (it == myControls.end())
{
return;
}

if (IsLiveModeNode( myDataView ->GetCurrentItem()))
	{
int nBefore = 0;
int nAfter = 0;

try
	{
		nBefore = it->second.GetSurfaceActions( myActiveProduct).GetLiveModeActionCount();		
}
catch( RBException strMsg)
	{
}

it->second.SetLiveModeAction( myActiveProduct, mySurfaceAction);

try
	{
		int nAfter = it->second.GetSurfaceActions( myActiveProduct).GetLiveModeActionCount();
}
catch( RBException strMsg)
{
}
}  // End if live mode

if (IsScreenReaderModeNode( myDataView ->GetCurrentItem()))
	{
it->second.SetScreenReaderModeAction( myActiveProduct, mySurfaceAction);
}

wxString wxstrNodeLabel = mySurfaceAction.GetSurfaceActionTypeDescription( mySurfaceAction.GetSurfaceActionType());
wxstrNodeLabel.append( GetActionParameterDescription( mySurfaceAction));	
wxtIDCurrentItem = myDataView ->GetCurrentItem();

while (m_actions_model->GetLevel( wxtIDCurrentItem) > MODE_LEVEL)
{
	wxtIDCurrentItem = m_actions_model->GetParent( wxtIDCurrentItem);
}

if (m_actions_model->AddAction( wxtIDCurrentItem, wxstrNodeLabel))
{
	myDataView->Refresh();
}

if (m_actions_model->GetLevel( wxtIDCurrentItem) == MODE_LEVEL)
{
myDataView ->Expand( wxtIDCurrentItem);
}  

		myDataView ->SetCurrentItem( wxtIDCurrentItem); 
myDataView->Refresh();
		myDataView ->SetFocus();
			
// Tidy up
		// myActionPicker->Destroy();
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
wxDataViewItem wxtIDCurrentItem = myDataView->GetCurrentItem();
		int nPos = m_actions_model->GetLevel( wxtIDCurrentItem);

// We're not focused on an action, so bale out
		if (nPos < ACTION_LEVEL)
			{
				return;
		}
		
		// We can only edit actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

unsigned int nIndex = GetChildNumber( wxtIDCurrentItem);

if (nPos != ACTION_LEVEL)
{
	return;
}
    
std::string strHash = GetControlHash( wxtIDCurrentItem);

if (strHash.empty())
	{
		return;
}

std::map <std::string, MessageDefinition> ::iterator it;

it = myControls.find( strHash);

if (it == myControls.end())
	{
return;
}

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentItem);
bool blnResult = false;
SurfaceActions myActions = it->second.GetSurfaceActions( myProduct);

if (IsLiveModeNode( wxtIDCurrentItem))
{
try
	{
blnResult = myActions.DeleteLiveModeAction( nIndex);
}
catch( RBException &e)
	{
return;		
}
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentItem))
{
	try
	{
		blnResult = myActions.DeleteLiveModeAction( nIndex);
}
catch( RBException &e)
	{
return;		
}
}  // end if ScreenReader mode
	}  // end else for valid modes
	
// Update the tree if the deletion succeeded
if (blnResult)
	{
		it->second.SetSurfaceActions( myProduct, myActions);
				    
		wxDataViewItemArray items;
    int len = myDataView->GetSelections( items );
    
	for( int i = 0; i < len; i ++ )
        {
			if (items[i].IsOk())
				{
					m_actions_model->Delete( items[i] );
			}  // end if OK
	}  // end for
}  // end if successful deletion

myDataView->Refresh();
myDataView ->SetFocus();
			}
		

void ActionsPage::OnRename(wxCommandEvent& event)
	{
		wxDataViewItem wxtIDCurrentItem = myDataView ->GetCurrentItem(); 

if (m_actions_model->GetLevel( wxtIDCurrentItem) == CONTROL_LEVEL)
	{
		std::wstring wstrControlName;
		wstrControlName = m_actions_model->GetTitle( wxtIDCurrentItem);
// Try to update the associated entry within the map of controls
std::string strFoundHash = GetControlName( wstrControlName);			
		
if (strFoundHash != "")
		{
std::map <std::string, MessageDefinition>::iterator it;

it = myControls.find( strFoundHash);

if (it != myControls.end())
{
			// Store the control hash and the definition
						MessageDefinition myDefinition = it->second;
			
// Get a new control label
			RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewControlNamePrompt, wstrControlName);

			if (UserLabelDlg->ShowModal() == wxID_OK)
		{
			std::wstring  wstrLabel = UserLabelDlg->GetText();
	
			if (wstrLabel.empty())
{
	wxMessageBox( wstrNoControlNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
} // end if label not empty
else
			{
				myDefinition.SetLabel( wstrLabel);
								
// Update the map, then the tree node
std::pair <std::string, MessageDefinition> myDefinitionPair;
myDefinitionPair = std::make_pair( strFoundHash, myDefinition);
myControls.erase( it);
myControls.insert( myDefinitionPair);
m_actions_model->SetTitle( wxtIDCurrentItem, wstrLabel);
myDataView->Refresh();
myDataView ->SetFocus();
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
wxDataViewItem myNode = myDataView->GetCurrentItem();

if (m_actions_model->GetLevel( myNode) < SPEECH_LEVEL)
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
	wxDataViewItem wxtIDCurrentItem = myDataView ->GetCurrentItem(); 

// We can only define actions for the active access technology product, so check:
if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

int nIndex = GetChildNumber( wxtIDCurrentItem);

if ((nIndex < 1)
	|| (m_actions_model->GetLevel( wxtIDCurrentItem) != ACTION_LEVEL))
{
	return;
}

std::string strHash = GetControlHash( wxtIDCurrentItem);

if (strHash.empty())
	{
		return;
}

std::map <std::string, MessageDefinition> ::iterator it;
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

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentItem);
SurfaceActions myActions; 

try
	{
		myActions = myDefinition.GetSurfaceActions( myProduct);
}
catch( RBException &e)
	{
return;		
}

bool blnResult;

if (IsLiveModeNode( wxtIDCurrentItem))
{
	blnResult = myActions.SwapLiveModeActions( (nIndex -1), nIndex);
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentItem))
{
	blnResult = myActions.SwapScreenReaderModeActions( nIndex, (nIndex -1));
}  // end if ScreenReader mode
	}  // end else for valid modes
	
if (blnResult == true)
	{
		if (IsLiveModeNode( wxtIDCurrentItem))
		{
std::wstring wstrAction = m_actions_model->GetTitle( wxtIDCurrentItem);
std::wstring wstrTempAction = m_actions_model->GetTitle( GetPrevSibling( wxtIDCurrentItem));
			m_actions_model->SetTitle( GetPrevSibling( wxtIDCurrentItem), wstrAction);
			m_actions_model->SetTitle( wxtIDCurrentItem, wstrTempAction);
}  // end if live mode

		if (IsScreenReaderModeNode( wxtIDCurrentItem))
		{
			std::wstring wstrAction = m_actions_model->GetTitle( wxtIDCurrentItem);
			std::wstring wstrTempAction = m_actions_model->GetTitle( GetPrevSibling( wxtIDCurrentItem));
			m_actions_model->SetTitle( GetPrevSibling( wxtIDCurrentItem), wstrAction);
			m_actions_model->SetTitle( wxtIDCurrentItem, wstrTempAction);
}  // end if screen-reader mode
}  // end if swap was successful

myDataView->Refresh();
myDataView ->SetFocus();
}


void ActionsPage::OnDown(wxCommandEvent& event)
{
	wxDataViewItem wxtIDCurrentItem = myDataView ->GetCurrentItem(); 

// We can only define actions for the active access technology product, so check:
	if (IsActiveProductBranch() == false)
{
wxMessageBox( wstrNotActiveProductError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

int nIndex = GetChildNumber( wxtIDCurrentItem);
wxDataViewItemArray myChildren;
m_actions_model->GetChildren( m_actions_model->GetParent( wxtIDCurrentItem), myChildren);
int nChildCount = myChildren.Count();

if ((nIndex == -1)
	|| (nIndex == nChildCount -1)
|| (m_actions_model->GetLevel( wxtIDCurrentItem) != ACTION_LEVEL))
{
	return;
}

std::string strHash = GetControlHash( wxtIDCurrentItem);

if (strHash.empty())
	{
		return;
}

std::map <std::string, MessageDefinition> ::iterator it;
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

ActiveProduct myProduct = GetProductFromNode( wxtIDCurrentItem);
SurfaceActions myActions; 

try
	{
		myActions = myDefinition.GetSurfaceActions( myProduct);
}
catch( RBException &e)
	{
return;		
}

bool blnResult;

if (IsLiveModeNode( wxtIDCurrentItem))
{
	blnResult = myActions.SwapLiveModeActions( nIndex, (nIndex +1));
}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( wxtIDCurrentItem))
{
	blnResult = myActions.SwapScreenReaderModeActions( nIndex, (nIndex +1));
}  // end if ScreenReader mode
	}  // end else for valid modes
	
if (blnResult == true)
	{
		if (IsLiveModeNode( wxtIDCurrentItem))
		{
			std::wstring  wstrAction = m_actions_model->GetTitle( wxtIDCurrentItem);
			std::wstring  wstrTempAction = m_actions_model->GetTitle( GetNextSibling( wxtIDCurrentItem));
			m_actions_model->SetTitle( GetNextSibling( wxtIDCurrentItem), wstrAction);
			m_actions_model->SetTitle( wxtIDCurrentItem, wstrTempAction);
}  // end if live mode

		if (IsScreenReaderModeNode( wxtIDCurrentItem))
		{
			std::wstring  wstrAction = m_actions_model->GetTitle( wxtIDCurrentItem);
			std::wstring  wstrTempAction = m_actions_model->GetTitle( GetNextSibling( wxtIDCurrentItem));
			m_actions_model->SetTitle( GetNextSibling( wxtIDCurrentItem), wstrAction);
			m_actions_model->SetTitle( wxtIDCurrentItem, wstrTempAction);
}  // end if screen-reader mode
}  // end if swap was successful

myDataView->Refresh();
	myDataView ->SetFocus();
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


	wxString ActionsPage::GetActiveProductName( ActiveProduct apID)
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
	wxDataViewItem wxtIDCurrentItem = myDataView ->GetCurrentItem();

if (m_actions_model->GetLevel( wxtIDCurrentItem) < SPEECH_LEVEL) 
{
	return false;
}

while (m_actions_model->GetLevel( wxtIDCurrentItem) != SPEECH_LEVEL)
{
	wxtIDCurrentItem = m_actions_model->GetParent( wxtIDCurrentItem);
	}
				
wxString wxstrProductName = m_actions_model->GetTitle( wxtIDCurrentItem);

	if (wxstrProductName.IsSameAs( GetActiveProductName( myActiveProduct), false))
{
return true;
}
else
{
/*
wxString wxstrOut = "Node name = ";
wxstrOut.append( wxstrDoubleQuotes);
wxstrOut.append( wxstrProductName);
wxstrOut.append( wxstrDoubleQuotes).append( wxstrNewLine);
wxstrOut.append( "Active product = ").append( wxstrDoubleQuotes);
wxstrOut.append( GetActiveProductName( myActiveProduct)).append( wxstrDoubleQuotes);
wxMessageBox( wxstrOut, wxT( "Test"), wxOK | wxICON_ERROR);
*/
	return false;
}
}


			// We need to implement an event table in which the events received by an ActionsTree are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ActionsPage, wxWizardPageSimple)
// DataView handling
EVT_DATAVIEW_SELECTION_CHANGED(ID_ACTION_TREE, ActionsPage::OnSelectionChanged)

#if wxUSE_DRAG_AND_DROP
    // EVT_DATAVIEW_ITEM_BEGIN_DRAG( ID_ACTION_TREE, ActionsPage::OnBeginDrag )
    // EVT_DATAVIEW_ITEM_DROP_POSSIBLE( ID_ACTION_TREE, ActionsPage::OnDropPossible )
    // EVT_DATAVIEW_ITEM_DROP( ID_ACTION_TREE, ActionsPage::OnDrop )
#endif // wxUSE_DRAG_AND_DROP
  
// Buttons
EVT_BUTTON(ID_ADD,  ActionsPage::OnAdd)	    
EVT_BUTTON(ID_RENAME,  ActionsPage::OnRename)
EVT_BUTTON(ID_PROPERTIES_BUTTON,  ActionsPage::OnProperties)
EVT_BUTTON( ID_UP,                 ActionsPage::OnUp)
EVT_BUTTON( ID_DOWN,                 ActionsPage::OnDown)
EVT_BUTTON( ID_DELETE,                 ActionsPage::OnRemove)

// Wizard page monitoring
EVT_WIZARD_CANCEL(wxID_ANY, ActionsPage::OnWizardCancel)
// EVT_WIZARD_PAGE_CHANGED(wxID_ANY, ActionsPage::OnWizardPageChanged)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY, ActionsPage::OnWizardPageChanging)
    END_EVENT_TABLE()
#endif   // if Mac
