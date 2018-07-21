// SurfaceList.cpp

#include "SurfaceList.h"


void SurfaceList::ShowSurfaceReport()
{
int nSelection= this->GetSelection();
	
if (nSelection> -1)
{
unsigned int nDisplayCount; 
std::wstring wstrOut;
nDisplayCount = pMyProtocols->GetProtocol( pMySurfaces->at( nSelection)->GetProtocolID()).GetDisplayCount();

for (unsigned int nDisplayNumber = 0; nDisplayNumber < nDisplayCount; nDisplayNumber++)
	{
DisplayDefinition myDisplay; 
	std::wstring wstrDisplayName; 
int nLineCount = 0;
int nDisplayLength = 0;
int nLineLength = 0;

std::wstring wstrDisplayText; 
myDisplay = pMyProtocols->GetProtocol( pMySurfaces->at( nSelection)->GetProtocolID()).GetDisplayItem( nDisplayNumber);		
		wstrDisplayName = myDisplay.GetLabel();

		if (wstrDisplayName.empty() == false)
{
	wstrOut.append( wstrDisplayName);
	wstrOut.append( wstrSpacedColon);

	if (myDisplay.IsLEDLamp() == false)
		{
			wstrOut.append( wstrNewLine);
			nLineCount = myDisplay.GetLineCount();
nDisplayLength = myDisplay.GetDisplayLength();
nLineLength = nDisplayLength / nLineCount;
	}  // if not LED

wstrDisplayText = pMySurfaces->at( nSelection)->GetDisplayText( wstrDisplayName);
	
if (myDisplay.IsLEDLamp())
	{
		wstrOut.append( pMySurfaces->at( nSelection)->GetCurrentState( wstrDisplayName));
	}
else  // text display
	{
if (wstrDisplayText.empty() == false)
			{
				for (int nLineNumber= 1; nLineNumber<= nLineCount; nLineNumber++)
{
int nStart = (nLineNumber * nLineLength) -1;
std::wstring wstrTemp = wstrDisplayText.substr( nStart, nLineLength);
wstrTemp.append( wstrNewLine);
wstrOut.append( wstrTemp);
}  // end for line processing
		}  // end if display is empty
}  // end if LED

wstrOut.append( wstrNewLine);
}  // end if display name not empty
}  // end for looping through displays

// Display report
DisplayReportDlg * myReport = new DisplayReportDlg( this->GetStringSelection().append( wstrSpace).append( wstrDisplayContentsTitle), wstrOut);
myReport->ShowModal();
myReport->Destroy();
}  // end if valid selection
}


		void SurfaceList::OnEnterKey(wxKeyEvent& event)
{
	if (event.GetKeyCode() == ENTER_KEY_CODE)
{
ShowSurfaceReport();
}  // end if enter key
}


// Event table
	
	BEGIN_EVENT_TABLE(SurfaceList, wxListBox)
	// Handle Enter key
	 // EVT_KEY_UP( SurfaceList::OnEnterKey)
END_EVENT_TABLE()



