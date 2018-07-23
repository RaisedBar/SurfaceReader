// SysExPage.cpp
// Purpose:  Wizard page to allow the user to create a system exclusive header for a new control surface protocol definition
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "SysExPage.h"


ProtocolSysExPage::ProtocolSysExPage(wxWizard * wizParent, const wxString & title, boost::shared_ptr<SurfaceProtocol> pProtocol, bool EncryptionMode)
       : wxWizardPageSimple( wizParent),
	   pMyProtocol( new SurfaceProtocol())
	   {
pMyProtocol = pProtocol;
	
SetLabel( title);blnEncryptionMode = EncryptionMode;
	
// Initialise the vector for the manufacturer ID
	vManufacturerID.reserve( 3);

	// Prompt for a descriptive name for the new protocol
wxBoxSizer * hBox1 = new wxBoxSizer( wxHORIZONTAL);
  wxStaticText * lblProtocolNamePrompt = new wxStaticText(this, wxID_ANY, wstrProtocolNamePrompt);
		txtProtocolName = new wxTextCtrl( this, wxID_ANY);
		txtProtocolName->SetFocus();        
		hBox1->Add( lblProtocolNamePrompt, 0, wxEXPAND);  
hBox1->Add( txtProtocolName, 0, wxEXPAND);

// Prompt for the system exclusive manufacturer ID bytes:
wxBoxSizer * hBox2 = new wxBoxSizer( wxHORIZONTAL);
wxStaticText * lblManufacturer1Prompt = new wxStaticText(this, wxID_ANY, wstrManufacturerID1Prompt);
txtSysExManufacturerByte1 = new wxTextCtrl( this, wxID_ANY, wstrDefaultSysExManufacturerIDByte1);
wxStaticText * lblManufacturer2Prompt = new wxStaticText(this, wxID_ANY, wstrManufacturerID2Prompt);
txtSysExManufacturerByte2 = new wxTextCtrl( this, wxID_ANY, wstrDefaultExtraSysExManufacturerIDByte);
wxStaticText * lblManufacturer3Prompt = new wxStaticText(this, wxID_ANY, wstrManufacturerID3Prompt);
txtSysExManufacturerByte3 = new wxTextCtrl( this, wxID_ANY, wstrDefaultExtraSysExManufacturerIDByte);

hBox2->Add( lblManufacturer1Prompt, 0, wxEXPAND); 
	hBox2->Add( txtSysExManufacturerByte1, 0, wxEXPAND);  
hBox2->Add( lblManufacturer2Prompt, 0, wxEXPAND); 
	hBox2->Add( txtSysExManufacturerByte2, 0, wxEXPAND);  
hBox2->Add( lblManufacturer3Prompt, 0, wxEXPAND); 
	hBox2->Add( txtSysExManufacturerByte3, 0, wxEXPAND);  

	wxBoxSizer * hBox3 = new wxBoxSizer( wxHORIZONTAL);
	wxStaticText * lblSysExModelIDPrompt = new wxStaticText(this, wxID_ANY, wstrSysExModelIDPrompt);
		txtSysExModelID = new wxTextCtrl( this, wxID_ANY);
		txtSysExModelID->SetValue( wstrDefaultSysExModelID);
hBox3->Add( lblSysExModelIDPrompt, 0, wxEXPAND);
hBox3->Add( txtSysExModelID, 0, wxEXPAND); 
		
wxBoxSizer * hBox4 = new wxBoxSizer( wxHORIZONTAL);
wxStaticText * lblSysExDeviceIDPrompt = new wxStaticText(this, wxID_ANY, wstrSysExDeviceIDPrompt);
txtSysExDeviceID = new wxTextCtrl( this, wxID_ANY);		
txtSysExDeviceID->SetValue( wstrDefaultSysExDeviceID);
hBox4->Add( lblSysExDeviceIDPrompt, 0, wxEXPAND);
hBox4->Add( txtSysExDeviceID, 0, wxEXPAND); 

wxBoxSizer * hBox5 = new wxBoxSizer( wxHORIZONTAL);
  // wxStaticText * lblProtocolEncryptionPrompt = new wxStaticText(this, wxID_ANY, wstrProtocolEncryptionPrompt);
		wxchkEncryptedProtocol = new wxCheckBox( this, ID_ENCRYPTED, wstrProtocolEncryptionPrompt);
		wxchkEncryptedProtocol->SetValue( pMyProtocol->IsEncrypted());
hBox5->Add( wxchkEncryptedProtocol, 0, wxEXPAND); 

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBox1);
	vBox1->Add( hBox2);
	vBox1->Add( hBox3);	
vBox1->Add( hBox4);	
vBox1->Add( hBox5);	

// Populate values
txtProtocolName->SetValue( pMyProtocol->GetProtocolName());

int nIDSize = pMyProtocol->GetSysExManufacturerID().size(); 

if (nIDSize > 0)
{
txtSysExManufacturerByte1->SetValue( ByteToHex( pMyProtocol->GetSysExManufacturerID().at( 0)));

if (nIDSize == 3)
{
txtSysExManufacturerByte2->SetValue( ByteToHex( pMyProtocol->GetSysExManufacturerID().at( 1)));
txtSysExManufacturerByte3->SetValue( ByteToHex( pMyProtocol->GetSysExManufacturerID().at( 2)));
}
}

if (pMyProtocol->GetSysExModelID() > -1)
{
	txtSysExModelID->SetValue( ByteToHex( pMyProtocol->GetSysExModelID()));
		}

if (pMyProtocol->GetSysExDeviceID() > -1)
{
	txtSysExDeviceID->SetValue( ByteToHex( pMyProtocol->GetSysExDeviceID()));
		}

if (blnEncryptionMode == false)
{
	wxchkEncryptedProtocol->Disable();
}

SetSizerAndFit(vBox1);
}


std::wstring ProtocolSysExPage::GetProtocolName() const
{
	return txtProtocolName->GetValue().ToStdWstring();
}


void ProtocolSysExPage::SetProtocolName( std::wstring wstrProtocolName)
{
	txtProtocolName->SetValue( wstrProtocolName);
}


std::vector <long> ProtocolSysExPage::GetSysExManufacturerID()
{
	std::string strTemp1 = txtSysExManufacturerByte1->GetValue().ToStdString();
	std::string strTemp2 = txtSysExManufacturerByte2->GetValue().ToStdString();
	std::string strTemp3 = txtSysExManufacturerByte3->GetValue().ToStdString();
	vManufacturerID.clear();

	if ( IsSingleHexByteString( strTemp1) == false)
{
	return vManufacturerID;
	}
	
	long lByte1;
	txtSysExManufacturerByte1->GetValue().ToLong( &lByte1, 16);

// Check the other fields
	if (lByte1 > 0)  // Single-byte manufacturer ID,  so other bytes should be blank
{
	if (strTemp2.empty() && strTemp3.empty())
	{
		vManufacturerID.push_back( lByte1);
		return vManufacturerID;
	}
else  // Anything in bytes 2 and 3 is invalid if byte1 > 0, so the overall ID is invalid
{
		return vManufacturerID;
	}  // end of garbage check for values 2 and 3
	}  // end of if first byte > 0 
else    // if byte1 == 0
{
// We need 2 more valid bytes to make up a valid 3-byte extended ID
if ((IsSingleHexByteString( strTemp2)) && ( IsSingleHexByteString( strTemp3)))
{
	long lByte2, lByte3;
	txtSysExManufacturerByte2->GetValue().ToLong( &lByte2, 16);
	txtSysExManufacturerByte3->GetValue().ToLong( &lByte3, 16);
	vManufacturerID.push_back( lByte1);
vManufacturerID.push_back( lByte2);
	vManufacturerID.push_back( lByte3);
	return vManufacturerID;
	}  // end of if 1 or 3 byte ID
else  // bytes 2 and/or 3 not valid hex
{
	vManufacturerID.clear();
}  // end bytes 2/3 validation
	}  // end if 1st value is a valid number	

return vManufacturerID;
}


void ProtocolSysExPage::SetSysExManufacturerID( std::vector <long> vNewManufacturerID)
{
	int nSize = vNewManufacturerID.size();

	if ((nSize == 1) || (nSize == 3))
	{
		txtSysExManufacturerByte1->SetValue( ByteToHex( vNewManufacturerID.at( 0)));

		if (nSize == 3)
		{
			txtSysExManufacturerByte2->SetValue( ByteToHex( vNewManufacturerID.at( 1)));
			txtSysExManufacturerByte3->SetValue( ByteToHex( vNewManufacturerID.at( 2)));
	}
	}
}

	
	long ProtocolSysExPage::GetSysExModelID() const
{
	std::string strTemp = txtSysExModelID->GetValue().ToStdString();
	
	if ((strTemp.empty())
|| (IsSingleHexByteString( strTemp) == false))
	{
		return -1;
	}
	else
	{
long lTemp;
txtSysExModelID->GetValue().ToLong( &lTemp, 16);
		return lTemp;
	}	
	}


	void ProtocolSysExPage::SetSysExModelID( long lModelID)
	{
		if (lModelID < 0)
			{
				txtSysExModelID->SetValue( wxEmptyString);
		}
		else
				{
std::string strModelID = ByteToHex( lModelID);
										txtSysExModelID->SetValue( strModelID);
		}
	}
	
	
	long ProtocolSysExPage::GetSysExDeviceID() const
{
			std::string strTemp = txtSysExDeviceID->GetValue().ToStdString();
			
if ((strTemp.empty())
|| (IsSingleHexByteString( strTemp) == false))
{
	return -1;
	}
else
{
	long lTemp;
txtSysExDeviceID->GetValue().ToLong( &lTemp, 16);
	return lTemp;
		}
}


void ProtocolSysExPage::SetSysExDeviceID( long lDeviceID)
{
	if (lDeviceID < 0)
			{
				txtSysExModelID->SetValue( wxEmptyString);
		}
		else
				{
					std::string strDeviceID = ByteToHex( lDeviceID);
					txtSysExDeviceID->SetValue( strDeviceID);
	}
}


bool ProtocolSysExPage::IsValidProtocolName()
{
	if (txtProtocolName->IsEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
		}


bool ProtocolSysExPage::IsValidSysExManufacturerID()
{
	std::string strByte1 = txtSysExManufacturerByte1->GetValue().ToStdString();
	std::string strByte2 = txtSysExManufacturerByte2->GetValue().ToStdString();
	std::string strByte3 = txtSysExManufacturerByte3->GetValue().ToStdString();

	if (IsSingleHexByteString( strByte1) == false)
{
return false;
	}

long lTemp1;
	txtSysExManufacturerByte1->GetValue().ToLong( &lTemp1, 16);
			
	if (lTemp1 == 0)  // we need to validate the other two bytes
{
	if ((IsSingleHexByteString( strByte2)) && (IsSingleHexByteString( strByte3)))
{
return true;
}
else  // byte 2 and/or 3 not valid hex
{
return false;
}   // end if bytes 2 and 3 valid hex}
}
else  // Byte 1 > 0, so bytes 2 and 3 must be blank
{
	if ((strByte2.empty()) && (strByte3.empty()))		
	{
	return true;
	}
	else
		{
			return false;
}  // end if byte 2 or 3 not empty
} // end if byte 1 > 0
}


	bool ProtocolSysExPage::IsValidSysExDeviceID() const
{
	std::string strDeviceID = txtSysExDeviceID->GetValue().ToStdString();

	if (IsSingleHexByteString( strDeviceID))
{
return true;
	}
	else  // if not valid hex, must be blank
	{
		if (strDeviceID.empty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}


	bool ProtocolSysExPage::IsValidSysExModelID()
	{
		std::string strModelID = txtSysExModelID->GetValue().ToStdString();

	if (IsSingleHexByteString( strModelID))
{
return true;
	}
	else  // if not valid hex, must be blank
	{
		if (strModelID.empty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}


		bool ProtocolSysExPage::IsValidWizardPage()
{
bool blnResult = true;
std::wstring wstrError;
bool blnValidName = IsValidProtocolName(); 
bool blnValidManufacturer = IsValidSysExManufacturerID();
bool blnValidModel = IsValidSysExModelID();
bool blnValidDevice = IsValidSysExDeviceID();

if (blnValidName == false) 
{
blnResult = false;
wstrError.append( wstrInvalidProtocolNameError);
}

if (blnValidManufacturer == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidSysExManufacturerIDError);
}

if (blnValidModel == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidSysExModelIDError);
}

if (blnValidDevice == false)
{
blnResult = false;

if (wstrError.empty() == false)
{
	wstrError.append( wstrNewLine);
}

wstrError.append( wstrInvalidSysExDeviceIDError);
}

if (blnResult == false)
	{
wxMessageBox( wstrError, wstrErrorTitle, wxOK | wxICON_ERROR);
}

return blnResult;
}


		void ProtocolSysExPage::OnWizardCancel(wxWizardEvent& event)
    {
		std::wstring wstrMessage = wstrCancelProtocolCheck;
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
            event.Veto();
	}
		}


		void ProtocolSysExPage::OnEncryptionChange(wxCommandEvent& event)
		{
			pMyProtocol->SetEncryptionFlag( wxchkEncryptedProtocol->GetValue());		
		}

		
		// Event table
BEGIN_EVENT_TABLE(ProtocolSysExPage, wxWizardPageSimple)
// Encryption handling
EVT_CHECKBOX( ID_ENCRYPTED, ProtocolSysExPage::OnEncryptionChange)
// Wizard page monitoring
EVT_WIZARD_CANCEL(wxID_ANY, ProtocolSysExPage::OnWizardCancel)
END_EVENT_TABLE()
