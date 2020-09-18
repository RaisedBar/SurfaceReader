// DefineTranslationTable.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "DefineTranslationTable.h"


DefineTranslationTable::DefineTranslationTable( const wxString& title, std::shared_ptr<SurfaceProtocol> pProtocol)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyProtocol( new SurfaceProtocol())
{  
pMyProtocol = pProtocol;
	strMyTableID.clear();
	
	// Generate a new table ID
boost::uuids::uuid myUUID = 	boost::uuids::random_generator()();
	strMyTableID = boost::lexical_cast <std::string> (myUUID); 	
	myLabels.clear();

	// Populate the table with default ANSI values
	for (unsigned int i = 0; i < 256; i++)
	{
std::pair <unsigned char, std::wstring> myPair;
std::wstring wstrLabel = wxString( char( i)).ToStdWstring();
myPair = std::make_pair( (unsigned char) i, wstrLabel);
myLabels.insert( myPair);
}
	
	wxPanel * myPanel = new wxPanel(this, -1);
	
	// List box of defined characters
wxBoxSizer * hBoxList = new wxBoxSizer( wxHORIZONTAL);
wxStaticText * lblDefinedCharactersPrompt = new wxStaticText(this, wxID_ANY, wstrDefinedCharactersPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxCharacters = new wxListBox( this, ID_CHARACTERS_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxList->Add( lblDefinedCharactersPrompt, 0, wxEXPAND);
hBoxList->Add( lbxCharacters, 0, wxEXPAND);
ListDefinedCharacters();

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * EditButton = new wxButton(myPanel, ID_EDIT, wstrEditCharacter, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxButtons->Add( EditButton, 0, wxEXPAND);
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxList);
vBox1->Add( hBoxButtons);	

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
}


DefineTranslationTable::DefineTranslationTable( const wxString& title, std::string strTableID, std::shared_ptr<SurfaceProtocol> pProtocol)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
strMyTableID = strTableID;
pMyProtocol = pProtocol;
myLabels = pMyProtocol->GetTranslationTable( strMyTableID).second;
	
	wxPanel * myPanel = new wxPanel(this, -1);
	
// List box of defined characters
wxBoxSizer * hBoxList = new wxBoxSizer( wxHORIZONTAL);
wxStaticText * lblDefinedCharactersPrompt = new wxStaticText(this, wxID_ANY, wstrDefinedCharactersPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxCharacters = new wxListBox( this, ID_CHARACTERS_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxList->Add( lblDefinedCharactersPrompt, 0, wxEXPAND);
hBoxList->Add( lbxCharacters, 0, wxEXPAND);
ListDefinedCharacters();

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * AddButton = new wxButton(myPanel, ID_ADD, wstrAddCharacter, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * EditButton = new wxButton(myPanel, ID_EDIT, wstrEditCharacter, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * DeleteButton = new wxButton(myPanel, ID_DELETE, wstrDeleteCharacter, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxButtons->Add( AddButton, 0, wxEXPAND);
hBoxButtons->Add( EditButton, 0, wxEXPAND);
hBoxButtons->Add( DeleteButton, 0, wxEXPAND);
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxList);
vBox1->Add( hBoxButtons);	

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
}


DefineTranslationTable::~DefineTranslationTable()
{}


bool DefineTranslationTable::IsValidTableDefinition()
{
	if ((strMyTableID.empty() == false)
	&& (IsValidCharacterMap()))
	{
	return true;
}
else
{
return false;
}
}


std::string DefineTranslationTable::GetTableID()
{
return strMyTableID;
}


std::map <unsigned char, std::wstring> DefineTranslationTable::GetCharacterMap()
{
				return myLabels;
}


bool DefineTranslationTable::IsValidCharacterMap()
{
return (myLabels.size() > 0);
}


void DefineTranslationTable::ListDefinedCharacters()
{
std::map <unsigned char, std::wstring>::iterator it;
lbxCharacters->Clear();

for (it = myLabels.begin(); it != myLabels.end(); it++)
{
		std::wstring wstrItem = it->second;
		wstrItem.append( wstrSpacedOpenParen);
		
		std::string strHexByte = ByteToHex( it->first);
		wstrItem.append( strHexByte.begin(), strHexByte.end());
		
		wstrItem.append( wstrCloseParen);
	lbxCharacters->Append( wstrItem);
}

lbxCharacters->SetFocus();
}


// Event handlers:

void DefineTranslationTable::OnEditCharacter( wxCommandEvent& event)
{
	int nSelection = lbxCharacters->GetSelection();
	
	if (nSelection > -1)
{
	DefineCharDialog * myCharDlg = new DefineCharDialog( wstrDefineCharTitle, nSelection, myLabels.at(  (unsigned char) nSelection));

if (myCharDlg->ShowModal() == wxID_OK)
{
if (myCharDlg->IsValidCharDefinition())
{
	std::pair <unsigned char, std::wstring> myPair;
	myPair = std::make_pair( (unsigned char) myCharDlg->GetCharIndex(), myCharDlg->GetCharLabel());
	myLabels.erase( (unsigned char) nSelection);
	myLabels.insert( myPair);	
	
	// Refresh the list box
	ListDefinedCharacters();
lbxCharacters->SetSelection( nSelection);
}  // end if valid char
}  // end if OK
}  // end if valid list selection
}

void DefineTranslationTable::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void DefineTranslationTable::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		// We need to implement an event table 

	BEGIN_EVENT_TABLE(DefineTranslationTable, wxDialog)
				EVT_BUTTON( ID_EDIT, DefineTranslationTable::OnEditCharacter)		
EVT_BUTTON( wxID_OK, DefineTranslationTable::OnOK)
	    EVT_BUTTON( wxID_CANCEL, DefineTranslationTable::OnCancel)
END_EVENT_TABLE()
