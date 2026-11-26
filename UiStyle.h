// UiStyle.h
// SurfaceReader's layout conventions, applied in one place.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SR_UI_STYLE_H
#define SR_UI_STYLE_H

#pragma once

#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/window.h>

/* SurfaceReader's windows were laid out with almost no spacing: controls touched each other and the
window edges, button rows sat wherever the layout put them, and dialogs had fixed sizes or were maximised.
Rather than adjust every Add() call in every dialog, these functions apply the same conventions to any
window's layout:
- every control without a border gets a standard one (DPI-scaled);
- a row made only of buttons is right-aligned, as Windows and macOS dialogs normally are;
- dialogs are sized to fit their contents and centred.
They change only sizes and positions. Tab order, labels, accessible names and keyboard behaviour are left
exactly as they were, and no colours are set, so high-contrast themes still apply. */

class wxWizard;

namespace UiStyle
{
	// The border around each control, in device-independent pixels
	const int nControlBorder = 5;

	// Apply the spacing conventions to a window's sizers and to those of all its child windows
	void ApplySpacing( wxWindow * pWindow);

	// ApplySpacing for a wizard's pages only, before it runs (it sizes itself around them, and its own
	// buttons keep wxWidgets' layout)
	void ApplyWizardSpacing( wxWizard * pWizard);

	// ApplySpacing, then size the dialog to fit its contents and centre it on its parent or the screen.
	// SurfaceReaderApp::FilterEvent calls this for every dialog as it is shown.
	void PrepareDialog( wxDialog * pDialog);

	// Monospaced (so a surface display's columns line up), at the size of the user's normal interface text
	wxFont DisplayFont( wxWindow * pWindow);
}

#endif  // SR_UI_STYLE_H
