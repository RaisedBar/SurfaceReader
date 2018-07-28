// ScreenReaderFunctionsDlg.cpp

#include "ScreenReaderFunctionsDlg.h"
#include "RBPathFuncs.h"
using namespace RaisedBar::PathFunctions;

#ifdef __WINDOWS__


ScreenReaderFunctionsDlg::ScreenReaderFunctionsDlg(const wxString & title, SurfaceActionType mySAType, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
pMyAppConfig( new AppConfig()),
pMySpeech( new RBSpeech()),
pMyJawsCache( new JawsCacheType())
{
	mySurfaceActionType = mySAType;
pMyAppConfig = pAppConfig;
pMySpeech = pSpeech;
pMyJawsCache = pJawsCache;

wstrArguments.clear();
wstrTargetFileName.clear();

// List of the available options
wxBoxSizer * hBoxLists = new wxBoxSizer( wxHORIZONTAL);
  wxStaticText * lblActionsPrompt = new wxStaticText(this, wxID_ANY, wstrAvailableScreenReaderActionsPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxActionNames = new wxListBox( this, ID_ACTIONS_LIST_BOX, wxPoint( ACTIONS_LIST_LEFT, ACTIONS_TOP_MARGIN), wxSize( ACTIONS_WIDTH, -1));
	    
		  hBoxLists ->Add( lblActionsPrompt, 0, wxEXPAND);  
	  	  hBoxLists ->Add( lbxActionNames, 0, wxEXPAND);  

		  // Buttons 
		  wxBoxSizer * hBoxButtons = new wxBoxSizer( wxHORIZONTAL);
		  btnProperties =new wxButton(this, ID_PROPERTIES_BUTTON, wstrPropertiesButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
			  btnOK = new wxButton(this, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
			  btnOK->SetDefault();
			  btnCancel = new wxButton(this, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxButtons->Add( btnProperties, 0, wxEXPAND);
hBoxButtons->Add( btnOK, 0, wxEXPAND);
hBoxButtons->Add( btnCancel, 0, wxEXPAND);
	
	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBoxLists);
	vBox1->Add( hBoxButtons);

	// Populate the list box
	ListAvailableActions( mySAType);
	lbxActionNames->SetFocus();
// Adjust button visibility
	ToggleButtons();

SetSizerAndFit(vBox1);
}

#else

ScreenReaderFunctionsDlg::ScreenReaderFunctionsDlg(const wxString & title, SurfaceActionType mySAType, boost::shared_ptr <AppConfig> pAppConfig, boost::shared_ptr <RBSpeech> pSpeech)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
pMyAppConfig( new AppConfig()),
pMySpeech( new RBSpeech())
{
	mySurfaceActionType = mySAType;
pMyAppConfig = pAppConfig;
pMySpeech = pSpeech;
	
		wstrArguments.clear();
wstrTargetFileName.clear();

// List of the available options
wxBoxSizer * hBoxLists = new wxBoxSizer( wxHORIZONTAL);
  wxStaticText * lblActionsPrompt = new wxStaticText(this, wxID_ANY, wstrAvailableScreenReaderActionsPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxActionNames = new wxListBox( this, ID_ACTIONS_LIST_BOX, wxPoint( ACTIONS_LIST_LEFT, ACTIONS_TOP_MARGIN), wxSize( ACTIONS_WIDTH, -1));
	    
		  hBoxLists ->Add( lblActionsPrompt, 0, wxEXPAND);  
	  	  hBoxLists ->Add( lbxActionNames, 0, wxEXPAND);  

		  // Buttons 
		  wxBoxSizer * hBoxButtons = new wxBoxSizer( wxHORIZONTAL);
		  btnProperties =new wxButton(this, ID_PROPERTIES_BUTTON, wstrPropertiesButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
			  btnOK = new wxButton(this, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
			  btnOK->SetDefault();
			  btnCancel = new wxButton(this, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxButtons->Add( btnProperties, 0, wxEXPAND);
hBoxButtons->Add( btnOK, 0, wxEXPAND);
hBoxButtons->Add( btnCancel, 0, wxEXPAND);
	
	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBoxLists);
	vBox1->Add( hBoxButtons);

	// Populate the list box
	ListAvailableActions( mySAType);
	lbxActionNames->SetFocus();
// Adjust button visibility
	ToggleButtons();

SetSizerAndFit(vBox1);
}
#endif


ScreenReaderFunctionsDlg::~ScreenReaderFunctionsDlg()
{}


bool ScreenReaderFunctionsDlg::IsValidParameterSet()
{
bool blnResult = false;

switch (mySurfaceActionType)
{
case RunHotSpot:
	{
		return (lbxActionNames->GetSelection() > -1);
	}
	break;
	
case 	RunScreenReaderFunction:
		{
return (lbxActionNames->GetSelection() > -1);
	}
	break;

default:
	{
	}
break;
}  // end switch

return blnResult;
}


ActionInfoType ScreenReaderFunctionsDlg::GetAction( std::wstring wstrFunctionName)
{
                ActionInfoType CurrentAction; 
ActionCollectionType Actions;
//Actions equals the second of the cache.
for(ActionInfoType CurrentAction : Actions)
{
	// if (CurrentAction[ 0].As <std::wstring>().compare( wstrFunctionName) == 0)
std::wstring wstrActionString = AsWString( CurrentAction[ 0]); 	
	
if (wstrActionString.compare( wstrFunctionName) == 0)
return CurrentAction;
}  // end for

// Not found
throw RBException( wstrActionNotFoundError);
}


ScreenReaderActionType ScreenReaderFunctionsDlg::GetActionTypeFromAction(ActionInfoType Action)
{
	return ScreenReaderActionType( AsInt( Action[ 6]));
}


std::wstring ScreenReaderFunctionsDlg::GetFunctionName()
{
	if (lbxActionNames->GetSelection() > -1)
		{
			return lbxActionNames->GetStringSelection().append( wstrArguments).ToStdWstring();
	}
	else
	{
		std::wstring wstrEmpty;
		wstrEmpty.clear();
		return wstrEmpty;
	}
}


std::wstring ScreenReaderFunctionsDlg::GetJawsJSDFileName()
{
wxFileDialog myFileDialog( this, wstrJawsJSDPrompt, wxEmptyString, wxEmptyString, JSD_FILE_TEMPLATE, wxFD_OPEN|wxFD_FILE_MUST_EXIST);

			myFileDialog.CentreOnParent();
			myFileDialog.SetDirectory(ProtocolPath().generic_wstring());

    if (myFileDialog.ShowModal() == wxID_OK)
    {
		return myFileDialog.GetPath().ToStdWstring();
	}
	else
	{
		std::wstring wstrEmpty;
		wstrEmpty.clear();
		return wstrEmpty;
	}
}


std::wstring ScreenReaderFunctionsDlg::GetHSCFileName()
{
wxFileDialog myFileDialog( this, wstrHSCPrompt, wxEmptyString, wxEmptyString, HSC_FILE_TEMPLATE, wxFD_OPEN|wxFD_FILE_MUST_EXIST);

			myFileDialog.CentreOnParent();
			myFileDialog.SetDirectory(ProtocolPath().generic_wstring());

    if (myFileDialog.ShowModal() == wxID_OK)
    {
		return myFileDialog.GetPath().ToStdWstring();
	}
	else
	{
		std::wstring wstrEmpty;
		wstrEmpty.clear();
		return wstrEmpty;
	}
}


void ScreenReaderFunctionsDlg::ListAvailableActions( SurfaceActionType mySAType)
{
switch (mySAType)
{
case RunHotSpot:
	{
		ListHotSpots();
	}
	break;
	
case 	RunScreenReaderFunction:
	{
		ListScreenReaderActions();
	}
	break;

default:
	{
	}
break;
}  // end switch
}


void ScreenReaderFunctionsDlg::ListHotSpots()
{
#ifdef __WINDOWS__
	std::wstring wstrHSCFileName = GetHSCFileName();

	if (wstrHSCFileName.empty())
{
wxMessageBox( wstrNoHotSpotBindingError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

wstrTargetFileName = wstrHSCFileName;
std::vector<std::wstring> Hotspots;
pMySpeech->ClearHscFile();
pMySpeech->SetHscFile(wstrHSCFileName);
HRESULT hr =pMySpeech->ListHotSpotsInSet(wstrHSCFileName, Hotspots);

if (hr == S_FALSE)
{
	wxMessageBox( wstrHSCParsingError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

lbxActionNames->Clear();

// Populate the list here...
for(std::wstring spot : Hotspots)
lbxActionNames->Append(spot);
#endif
}


void ScreenReaderFunctionsDlg::ListScreenReaderActions()
{
ActiveProduct myProduct;
DolphinProduct myDolphinProduct;

	lbxActionNames->Clear();
pMySpeech->GetActiveProduct( myProduct, myDolphinProduct);

switch (myProduct)
{
case ID_NONE:
	{
		return;
		}
	break;

#ifdef __WINDOWS__
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
#endif
default:
{
		return;
	}
	break;
}  // end switch
}


ActionCollectionType ScreenReaderFunctionsDlg::GetCachedJawsActionsListItem( std::wstring wstrAppName, std::wstring strProtocolID)
{
#ifdef __WINDOWS__
	JawsCacheType::iterator it;
	std::pair < std::wstring, std::wstring> myPair;
	myPair = std::make_pair( wstrAppName, strProtocolID);
it = pMyJawsCache->find( myPair);

	if (it != pMyJawsCache->end())
	{
		return it->second;
	}
	else
	{
throw RBException( wstrInfoNotCachedError);
return it->second;
	}
#endif
}


void ScreenReaderFunctionsDlg::ToggleButtons()
	{
if (mySurfaceActionType == RunHotSpot)
{
	btnProperties->Disable();
}
else
{
	btnProperties->Enable();
}
}


	#ifdef __WINDOWS__

// std::vector<JAWSParameter> ScreenReaderFunctionsDlg::GetJawsParameters(std::wstring wstrAction)
boost::shared_ptr <JawsParametersType> ScreenReaderFunctionsDlg::GetJawsParameters(std::wstring wstrAction)
{
	std::vector<JAWSParameter> JawsParameters;	
	ActionCollectionType myActionCollection;
try
	{
	ActionInfoType CurrentAction;
		myActionCollection = GetCachedJawsActionsListItem( pMyAppConfig->GetAppName(), pMyAppConfig->GetProtocolID());
ActionInfoTypeIterator it2;
std::wstring wstrName;
	
for(ActionInfoType CurrentAction : myActionCollection)
{
			it2 =CurrentAction.find(0);
		
			if (it2 !=CurrentAction.end())
		{
			if ( IsWString( it2->second))
			{
				wstrName = AsWString( it2->second);
				
				if (wstrName.compare( wstrAction) == 0)
{
				it2 =CurrentAction.find(6);
if (it2 !=CurrentAction.end())
								{
									if (IsInt( it2->second))
			{
									int Type =AsInt( it2->second);
													
									if (Type ==ID_TYPE_FUNCTION)
				{
					it2 =CurrentAction.find(4);
	        if (it2 !=CurrentAction.end())
			{
			if (IsJawsParameterVector( it2->second))
			{
				JawsParameters =AsJawsParameterVector( it2->second);			
			} //end using the parameters vector.
			}
													}
									} //end type.
									} //end using type.
				} //end using name.
			} //end type check.
			}
} //end foreach.
} //end try.
catch( RBException &myException)   // Get the user to define the Jaws script file to be processed
{
//display error here.
			// #ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					// #endif
}
return boost::shared_ptr <JawsParametersType>( new JawsParametersType (Parameters));
	}
#endif


	std::wstring ScreenReaderFunctionsDlg::GetTargetFileName()
	{
		return wstrTargetFileName;
	}


	// Event handlers:

void ScreenReaderFunctionsDlg::OnActionSelect( wxCommandEvent& event)
{
	// myActionCollection.at( 0).
}


void ScreenReaderFunctionsDlg::OnProperties( wxCommandEvent& event)
{
#ifdef __WINDOWS__
	if (lbxActionNames->GetSelection() > -1)
		{
			ScreenReaderParametersDlg * myScreenReaderParamsDlg = new ScreenReaderParametersDlg( wstrScreenReaderParametersTitle,  GetJawsParameters( lbxActionNames->GetStringSelection().ToStdWstring()));

if (myScreenReaderParamsDlg->ShowModal() == wxID_OK)
{
	wstrArguments = myScreenReaderParamsDlg->GetParameterString();
}  // if OK

myScreenReaderParamsDlg->Destroy();
	}  // end if valid selection
#endif   // if windows
}


void ScreenReaderFunctionsDlg::OnOK( wxCommandEvent& event)
{
EndModal( wxID_OK);
}


void ScreenReaderFunctionsDlg::OnCancel( wxCommandEvent& event)
{
EndModal( wxID_OK);
}

#ifdef __WINDOWS__

void ScreenReaderFunctionsDlg::ListJawsActions()
{
		AvailableActionsType myActionType;
AvailableActionFieldsIterator it;	
int nField =-1;
ActionInfoTypeIterator it2;
	ActionInfoType CurrentAction;
	
// Check to see if we've already processed the required information
try
	{
		myActionCollection = GetCachedJawsActionsListItem( pMyAppConfig->GetAppName(), pMyAppConfig->GetProtocolID());
nField =0;
}
catch( RBException &myException)   // Get the user to define the Jaws script file to be processed
{
OutputDebugString( myException.what());

	wstrJSDFileName = GetJawsJSDFileName();

	if (wstrJSDFileName.empty())
{
wxMessageBox( wstrNoJawsJSDBindingError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

wstrTargetFileName = wstrJSDFileName;
pMySpeech->ClearJsdFile();
pMySpeech->SetFirstJsdFile(wstrJSDFileName);
HRESULT hr = pMySpeech->GetAvailableActions(myActionType);	

// if (SUCCEEDED( hr) == false)
if (hr == S_FALSE)
{
return;
}

it = myActionType.first.find( wstrNameField);

if (it != myActionType.first.end())
{
	myActionCollection = myActionType.second;
	
	// Update the cache for next time
std::pair< std::wstring, std::wstring> myIndexPair;
	myIndexPair = std::make_pair( pMyAppConfig->GetAppName(), pMyAppConfig->GetProtocolID());
	std::pair <std::pair <std::wstring, std::wstring>, ActionCollectionType> myPair;
	myPair = std::make_pair( myIndexPair, myActionCollection);
	pMyJawsCache->insert( myPair);

	// Now we need to populate the list box
	nField = it->second;
} //end finding name field.
}  // end catch

//iterate through the vector of actions, the second item in the ActionInfoType pair.
for(ActionInfoType CurrentAction : myActionCollection)
	{
			it2 =CurrentAction.find(nField);
		
			if (it2 !=CurrentAction.end())
		{
std::wstring wstrFieldValue;

        if (IsWString( it2->second))
        {
			wstrFieldValue = AsWString( it2->second);
}

		lbxActionNames->Append(wstrFieldValue); 
	}
			
			it2 =CurrentAction.find(6);

			if (it2 !=CurrentAction.end())
			{
			if ( IsInt( it2->second))
			{
				int Type =AsInt( it2->second);
				
				if (Type ==ID_TYPE_FUNCTION)
				{
					it2 =CurrentAction.find(4);

	        if (it2 !=CurrentAction.end())
			{
			if ( IsJawsParameterVector( it2->second))
			{
				Parameters.clear();
Parameters =AsJawsParameterVector( it2->second);			
//edit.
			}
			}
				}
			}
			} //end finding type.
			} //end foreach.
}  
		

void ScreenReaderFunctionsDlg::ListSAActions()
		{
			return;
		}

void ScreenReaderFunctionsDlg::ListWindowEyesActions()
	{
		return;
	}

void ScreenReaderFunctionsDlg::ListNVDAActions()
{
	return;
}

void ScreenReaderFunctionsDlg::ListDolphinActions()
		{
			return;
		}
#endif  // end of Windows-specific handlers


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ScreenReaderFunctionsDlg, wxDialog)
// list box selection
EVT_LISTBOX( ID_ACTIONS_LIST_BOX, ScreenReaderFunctionsDlg::OnActionSelect)

// Buttons:
EVT_BUTTON(ID_PROPERTIES_BUTTON,  ScreenReaderFunctionsDlg::OnProperties)
	    EVT_BUTTON(wxID_OK,  ScreenReaderFunctionsDlg::OnOK)
EVT_BUTTON( wxID_CANCEL,                 ScreenReaderFunctionsDlg::OnCancel)
END_EVENT_TABLE()
