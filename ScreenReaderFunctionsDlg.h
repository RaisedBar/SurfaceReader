// ScreenReaderFunctionsDlg.h
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SCREEN_READER_FUNCTIONS_DLG_H
#define SCREEN_READER_FUNCTIONS_DLG_H

#pragma once

#include "RBSpeech.h"
#include "AppConfig.h"
#include "SurfaceAction.h"
#include "ScreenReaderParametersDlg.h"
#include "RBException.h"
#include "RBEnums.h"
#include "RBStringFuncs.h"

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>
using boost::any_cast;

#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/msgdlg.h> 
#include <wx/msw/winundef.h> 


// Screen positions
const int ACTIONS_LIST_LEFT = 210;
const int ACTIONS_TOP_MARGIN = 60;
const int ACTIONS_WIDTH = 400;


class ScreenReaderFunctionsDlg: public wxDialog
{
public:
#ifdef __WINDOWS__
	ScreenReaderFunctionsDlg(const wxString & title, SurfaceActionType mySAType, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache);
#else
ScreenReaderFunctionsDlg(const wxString & title, SurfaceActionType mySAType, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<RBSpeech> pSpeech);
#endif
	
~ScreenReaderFunctionsDlg();

bool IsValidParameterSet();
std::wstring GetTargetFileName();
std::wstring GetFunctionName();
ScreenReaderActionType GetActionTypeFromAction(ActionInfoType Action);
ActionInfoType GetAction( std::wstring wstrFunctionName);

private:
void ToggleButtons();
ActionCollectionType GetCachedJawsActionsListItem( std::wstring wstrAppName, std::string strProtocolID);
std::wstring GetJawsJSDFileName();
std::wstring GetHSCFileName();

#ifdef __WINDOWS__
// std::vector<JAWSParameter> GetJawsParameters(std::wstring wstrAction);
boost::shared_ptr <JawsParametersType> GetJawsParameters(std::wstring wstrAction);
#endif

	void ListAvailableActions( SurfaceActionType mySAType);
void ListHotSpots();
void ListScreenReaderActions();
#ifdef __WINDOWS__
void ListJawsActions();
		void ListSAActions();
void ListNVDAActions();
		void ListDolphinActions();
#endif
	
	//active product.
		ActiveProduct myProduct;

		// Event handlers

void OnActionSelect( wxCommandEvent& event);
void OnProperties( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);

void OnCancel( wxCommandEvent& event);

// Internal storage
// List box of available screen reader actions
wxListBox * lbxActionNames; 

// Buttons
wxButton * btnOK, * btnCancel, * btnProperties;

// The action type we're dealing with:
SurfaceActionType mySurfaceActionType;

// The retrieved actions
ActionCollectionType myActionCollection;
std::vector<JAWSParameter> Parameters;

// Pointer to the protocol being referenced
boost::shared_ptr<AppConfig> pMyAppConfig;
// Pointer to the speech output
boost::shared_ptr<RBSpeech> pMySpeech;

	// Jaws caching
#ifdef __WINDOWS__
boost::shared_ptr<JawsCacheType> pMyJawsCache;
#endif

std::wstring wstrJSDFileName;
std::wstring wstrHSCFileName();
std::wstring wstrTargetFileName;
std::wstring wstrArguments;

DECLARE_EVENT_TABLE()
};

#endif       // SCREEN_READER_FUNCTIONS_DLG_H