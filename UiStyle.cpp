// UiStyle.cpp
// SurfaceReader's layout conventions, applied in one place.
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "UiStyle.h"

#include <wx/button.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/toplevel.h>
#include <wx/wizard.h>

#include <algorithm>

namespace UiStyle
{
	namespace
	{
		// True for a sizer whose items are all buttons (spacers allowed)
		bool IsButtonRow( wxSizer * pSizer)
		{
			bool blnHasButton = false;
			for (wxSizerItem * pItem : pSizer->GetChildren())
			{
				if (pItem->IsSpacer())
				{
					continue;
				}
				if ((pItem->IsWindow() == false) || (wxDynamicCast( pItem->GetWindow(), wxButton) == nullptr))
				{
					return false;
				}
				blnHasButton = true;
			}
			return blnHasButton;
		}

		void StyleSizer( wxWindow * pOwner, wxSizer * pSizer)
		{
			const int nBorder = pOwner->FromDIP( nControlBorder);
			wxBoxSizer * pBox = wxDynamicCast( pSizer, wxBoxSizer);
			bool blnVertical = (pBox != nullptr) && (pBox->GetOrientation() == wxVERTICAL);

			for (wxSizerItem * pItem : pSizer->GetChildren())
			{
				if (pItem->IsWindow())
				{
					// Leave any border the layout already chose
					if (pItem->GetBorder() == 0)
					{
						pItem->SetBorder( nBorder);
						pItem->SetFlag( pItem->GetFlag() | wxALL);
					}
				}
				else if (pItem->IsSizer())
				{
					wxSizer * pChild = pItem->GetSizer();
					if (blnVertical && IsButtonRow( pChild))
					{
						// Buttons go at the right of a dialog, at their natural size
						int nFlags = pItem->GetFlag();
						nFlags &= ~(wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
						nFlags |= wxALIGN_RIGHT;
						pItem->SetFlag( nFlags);
					}
					StyleSizer( pOwner, pChild);
				}
			}
		}
	}


	void ApplySpacing( wxWindow * pWindow)
	{
		if (pWindow == nullptr)
		{
			return;
		}

		// wxWidgets gives every panel the window text "panel", which screen readers can expose as the name
		// of a pane; a panel is only a container, so it shouldn't have a name
		if ((wxDynamicCast( pWindow, wxPanel) != nullptr) && (pWindow->GetLabel() == wxPanelNameStr))
		{
			pWindow->SetLabel( wxEmptyString);
		}

		// Children first, so their new minimum sizes count towards this window's
		for (wxWindow * pChild : pWindow->GetChildren())
		{
			// Other dialogs and frames are styled when they are shown
			if (pChild->IsTopLevel() == false)
			{
				ApplySpacing( pChild);
			}
		}

		if (pWindow->GetSizer() != nullptr)
		{
			StyleSizer( pWindow, pWindow->GetSizer());

			// A panel or wizard page whose minimum size was fixed by SetSizerAndFit() before the spacing was
			// added would otherwise be laid out too small, cutting off its last controls
			if (pWindow->IsTopLevel() == false)
			{
				wxSize myNeeded = pWindow->GetSizer()->GetMinSize();
				wxSize myMin = pWindow->GetMinSize();
				pWindow->SetMinSize( wxSize( std::max( myMin.x, myNeeded.x), std::max( myMin.y, myNeeded.y)));
			}
		}

		pWindow->Layout();
	}


	void ApplyWizardSpacing( wxWizard * pWizard)
	{
		// Only the pages: the wizard's own buttons and bitmap area keep wxWidgets' layout
		for (wxWindow * pChild : pWizard->GetChildren())
		{
			if (wxDynamicCast( pChild, wxWizardPage) != nullptr)
			{
				ApplySpacing( pChild);
			}
		}
	}


	void PrepareDialog( wxDialog * pDialog)
	{
		ApplySpacing( pDialog);

		if (pDialog->IsMaximized())
		{
			pDialog->Maximize( false);
		}

		// Most dialogs put their controls on a single panel; some lay them out on the dialog itself
		wxWindow * pContent = pDialog;
		if ((pDialog->GetSizer() == nullptr) && (pDialog->GetChildren().GetCount() == 1))
		{
			pContent = pDialog->GetChildren().GetFirst()->GetData();
		}

		if (pContent->GetSizer() != nullptr)
		{
			wxSize myBest = pContent->GetSizer()->GetMinSize();
			pDialog->SetMinClientSize( wxSize());  // allow shrinking from the old fixed size
			pDialog->SetClientSize( myBest);
			pDialog->SetMinClientSize( myBest);
			pDialog->Layout();
		}

		pDialog->CentreOnParent();
	}


	wxFont DisplayFont( wxWindow * pWindow)
	{
		wxFont myNormal = pWindow->GetFont().IsOk() ? pWindow->GetFont() : wxSystemSettings::GetFont( wxSYS_DEFAULT_GUI_FONT);
		return wxFont( wxFontInfo( myNormal.GetFractionalPointSize()).Family( wxFONTFAMILY_TELETYPE));
	}
}
