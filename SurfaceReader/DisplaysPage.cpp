// DisplaysPage.cpp
// Implementation of the DisplaysPage class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "DisplaysPage.h"


DisplaysPage::DisplaysPage(wxWizard * wizParent, const wxString & title, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<SurfaceProtocol> pProtocol)
       : wxWizardPageSimple( wizParent),
pMyAppConfig( new AppConfig()),
pMyProtocol( new SurfaceProtocol())
{
  pMyAppConfig = pAppConfig;
  pMyProtocol = pProtocol;
myDisplays = pMyAppConfig->GetDisplays();
  
  wxPanel *myPanel = new wxPanel(this, -1);
// Tree of defined displays and states
  wxDisplayTree = new wxTreeCtrl(myPanel, ID_ACTION_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
   // Create root node
	      wxtIDRootNode = wxDisplayTree->AddRoot( pMyProtocol->GetProtocolName().append( wstrProtocolDisplaysPrompt));
          
ListDisplays( wxtIDRootNode);
wxDisplayTree->SortChildren( wxtIDRootNode);

if (wxDisplayTree->GetChildrenCount( wxtIDRootNode) > 0)
{
// wxDisplayTree->Expand( wxtIDRootNode);
// wxDisplayTree->SetFocusedItem( wxDisplayTree->GetFirstChild( wxtIDRootNode, cookie));
}

wxBoxSizer *HBox1 = new wxBoxSizer(wxHORIZONTAL);
HBox1->Add( wxDisplayTree, 0, wxEXPAND);

// Buttons to control the tree
		  btnRename = new wxButton(myPanel, ID_RENAME, wstrRenameButtonName, wxDefaultPosition, wxSize(70, 30));
btnStates = new wxButton(myPanel, ID_STATES, wstrDefineStates, wxDefaultPosition, wxSize(70, 30));

  wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
hBoxButtons->Add( btnRename, 0, wxEXPAND);
    hBoxButtons->Add(btnStates, 0, wxEXPAND);
    
  wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
vBox->Add(HBox1, 0, wxEXPAND);  
  vBox->Add(hBoxButtons, 0, wxEXPAND);

// Enable/disable buttons appropriately
ToggleButtons();

SetSizerAndFit(vBox);
}


DisplaysPage::~DisplaysPage()
{}


int DisplaysPage::GetChildNumber( wxTreeItemId wxtIDCurrentNode)
{
wxTreeItemId myNode = wxtIDCurrentNode;
int nChildNumber = 0;

while (wxDisplayTree->GetPrevSibling( myNode).IsOk())
{
	nChildNumber++;
	myNode = wxDisplayTree->GetPrevSibling( myNode);
}

return nChildNumber;
}


int DisplaysPage::GetLevel( wxTreeItemId myNode)
	{
int nLevel = 0;
wxTreeItemId wxtIDCurrentNode = myNode;
    
while (wxtIDCurrentNode .IsOk())
{
	wxtIDCurrentNode = wxDisplayTree->GetItemParent( wxtIDCurrentNode);
	if (wxtIDCurrentNode.IsOk())
	{
nLevel++;
	}
}
				
return nLevel;
}


void DisplaysPage::ToggleButtons()
{
	wxTreeItemId wxtIDCurrentNode = wxDisplayTree->GetFocusedItem();
	
	// Disable/enable controls, depending on the currently-selected level of the tree
switch (GetLevel( wxtIDCurrentNode))
	{
case DISPLAY_LEVEL:  // Allow control to be renamed
	{
	btnStates->Disable();
	btnRename->Enable();
	}	
	break;

case STATE_LEVEL:    // Can only manipulate actions when focused on the level showing the controls for a selected mode
	{
		btnStates->Enable();
	btnRename->Disable();
	}
break;

default:
	{
		btnRename->Disable();
	btnStates->Disable();
}
	break;
}  // end switch
}


std::wstring DisplaysPage::GetDisplayName( std::wstring wstrName)
{
	std::map <std::wstring, DisplayDefinition> ::iterator it;
		
	for (it = myDisplays.begin(); it != myDisplays.end(); it++)
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


std::wstring DisplaysPage::GetDisplayHash( wxTreeItemId myCurrentNode )
{
wxTreeItemId wxtIDCurrentNode = myCurrentNode;
	
// find the correct level in order to get the display name
while (GetLevel( wxtIDCurrentNode)> DISPLAY_LEVEL)
{
wxtIDCurrentNode = wxDisplayTree->GetItemParent( wxtIDCurrentNode);
}

std::wstring wstrDisplayName = 		wxDisplayTree->GetItemText( wxtIDCurrentNode).ToStdWstring();
// Now find the matching hash value
DisplayDefinition myDisplayDefinition;

		return GetDisplayName( wstrDisplayName);
}


std::map <std::wstring, DisplayDefinition> DisplaysPage::GetDisplays()
{
	return myDisplays;
}


// Event handlers

void DisplaysPage::OnTreeSelect( wxTreeEvent& event)
{
ToggleButtons();
}


void DisplaysPage::OnStates(wxCommandEvent& event)
    {
}


void DisplaysPage::OnRename(wxCommandEvent& event)
{
	wxTreeItemId wxtIDCurrentNode = wxDisplayTree->GetFocusedItem(); 

if (GetLevel( wxtIDCurrentNode) == DISPLAY_LEVEL)
	{
		std::wstring wstrDisplayName = wxDisplayTree->GetItemText( wxtIDCurrentNode).ToStdWstring();

// Try to update the associated entry within the map of displays
std::wstring strFoundHash = GetDisplayName( wstrDisplayName);			
		
if (strFoundHash.empty() == false)
		{
std::map <std::wstring, DisplayDefinition>::iterator it;

it = myDisplays.find( strFoundHash);

if (it != myDisplays.end())
{
			// Store the display hash and the definition
						DisplayDefinition myDefinition = it->second;
			
// Get a new display label
			RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewDisplayNamePrompt, wstrDisplayName);

			if (UserLabelDlg->ShowModal() == wxID_OK)
		{
			std::wstring wstrLabel = UserLabelDlg->GetText();
	
			if (wstrLabel.empty())
{
	wxMessageBox( wstrNoDisplayNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
} // end if label not empty
else
			{
				myDefinition.SetLabel( wstrLabel);				
								
// Update the map, then the tree node
std::pair <std::wstring, DisplayDefinition> myDefinitionPair;
myDefinitionPair = std::make_pair( strFoundHash, myDefinition);
myDisplays.erase( it);
myDisplays.insert( myDefinitionPair);
wxDisplayTree->SetItemText( wxtIDCurrentNode, wstrLabel);
wxDisplayTree->SetFocus();
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


void DisplaysPage::OnWizardCancel(wxWizardEvent& event)
    {
		if (		wxMessageBox( wstrActionsCheck,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
            event.Veto();
	}
		}


		void DisplaysPage::ListDisplays( wxTreeItemId wxtIDCurrentNode)
	{
			  // Create a branch for every available display
std::map <std::wstring, DisplayDefinition>::iterator it;

for (it = myDisplays.begin(); it != myDisplays.end(); it++)
{
	wxTreeItemId wxtIDNewNode = wxDisplayTree->AppendItem( wxtIDCurrentNode, it->second.GetLabel());
	}
}


	// We need to implement an event table in which the events received by an DisplayTree are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DisplaysPage, wxWizardPageSimple)
EVT_TREE_SEL_CHANGED( ID_ACTION_TREE, DisplaysPage::OnTreeSelect)

// Buttons
EVT_BUTTON(ID_RENAME,  DisplaysPage::OnRename)
EVT_BUTTON(ID_STATES,  DisplaysPage::OnStates)

// Wizard page monitoring
EVT_WIZARD_CANCEL(wxID_ANY, DisplaysPage::OnWizardCancel)
END_EVENT_TABLE()
	
		