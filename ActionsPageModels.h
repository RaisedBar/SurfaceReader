// ActionsPageModels.hpp

#ifndef ACTIONS_PAGE_MODELS_H
#define ACTIONS_PAGE_MODELS_H

#pragma once

#include <wx/platform.h>
#ifdef __WXOSX_COCOA__

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "AppConfig.h"
#include "MessageDefinition.h"
#include "RBStringFuncs.h"
#include "SRConstants.h"
#include "RBException.h"

#include <string>

#include <wx/wx.h>

 // #include "null.xpm"
// #include "wx_small.xpm"
#include "wx/dataview.h"


const int PROTOCOL_LEVEL = 0;
const int CONTROL_LEVEL = 1;
const int SPEECH_LEVEL = 2;
const int MODE_LEVEL = 3;
const int ACTION_LEVEL = 4;
const int PROPERTIES_LEVEL = 5;


// ActionsTreeModelNode: a node inside ActionsTreeModel

class ActionsTreeModelNode;
WX_DEFINE_ARRAY_PTR( ActionsTreeModelNode*, ActionsTreeModelNodePtrArray );

class ActionsTreeModelNode
{
public:
	ActionsTreeModelNode( ActionsTreeModelNode* parent, const wxString &title, int nLevel)
    {
        m_parent = parent;
m_title = title.ToStdWstring();
m_level = nLevel;
m_container = true;
    }

	ActionsTreeModelNode( ActionsTreeModelNode* parent, const wxString &title, MessageDefinition myMessage, int nLevel)
    {
        m_parent = parent;
m_title = title.ToStdWstring();
m_level = nLevel;
m_container = false;
    }

~ActionsTreeModelNode()
    {
        // free all our children nodes
        size_t count = m_children.GetCount();

for (size_t i = 0; i < count; i++)
        {
            ActionsTreeModelNode *child = m_children[i];
            delete child;
        }
    }

    bool IsContainer() const
        { 
			return m_container; 
	}

    ActionsTreeModelNode* GetParent()
        { 
			return m_parent; 
	}

	ActionsTreeModelNodePtrArray& GetChildren()
        { 
			return m_children; 
	}

	void Insert( ActionsTreeModelNode* child, unsigned int n)
        { 
			m_children.Insert( child, n); 
	}

	void Append( ActionsTreeModelNode* child )
        { 
			m_children.Add( child ); 
	}

	unsigned int GetChildCount() const
        { 
			return m_children.GetCount(); 
	}

public:     // public to avoid getters/setters
	std::wstring                m_title;
int m_level;
bool m_container;

private:
ActionsTreeModelNode          *m_parent;
    ActionsTreeModelNodePtrArray   m_children;
};


// ActionsTreeModel

class ActionsTreeModel: public wxDataViewModel
{
public:
    ActionsTreeModel( wxString wxstrRootTitle, std::map <std::string, MessageDefinition> * pHardwareControls, boost::shared_ptr <AppConfig> pAppConfig);

	~ActionsTreeModel()
    {
        delete m_root;
    }

bool AddAction( const wxDataViewItem &item, const wxString &wxstrTitle);
	std::wstring GetTitle( const wxDataViewItem &item) const;
	bool SetTitle( wxDataViewItem item, std::wstring  wstrTitle);

int GetLevel( const wxDataViewItem &item ) const;
std::pair <std::string, MessageDefinition> GetControl( const wxDataViewItem &item ) const;
    void Delete( const wxDataViewItem &item );
    
	// override sorting to always sort branches ascendingly
int Compare( const wxDataViewItem &item1, const wxDataViewItem &item2, unsigned int column, bool ascending ) const;

    // implementation of base class virtuals to define model

    virtual unsigned int GetColumnCount() const
    {
        return PROPERTIES_LEVEL +1;
    }

        virtual wxString GetColumnType( unsigned int col ) const
    {
        return wxT("string");
    }

    virtual void GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const;
    virtual bool SetValue( const wxVariant &variant, const wxDataViewItem &item, unsigned int col );

    virtual bool IsEnabled( const wxDataViewItem &item, unsigned int col ) const;

    virtual wxDataViewItem GetParent( const wxDataViewItem &item ) const;
    virtual bool IsContainer( const wxDataViewItem &item ) const;
    virtual unsigned int GetChildren( const wxDataViewItem &parent, wxDataViewItemArray &array ) const;

private:
    	int GetLevel( ActionsTreeModelNode * myNode);
ActiveProduct GetProductFromNode( ActionsTreeModelNode * myNode);
bool IsLiveModeNode( ActionsTreeModelNode * myNode);
bool IsScreenReaderModeNode( ActionsTreeModelNode * myNode);
	ActiveProduct GetProductFromName( std::wstring wstrName);
	std::wstring GetActionParameterDescription( SurfaceAction mySA);

void AddScreenReaderNodes( ActionsTreeModelNode * myControlNode, std::string strHash, MessageDefinition myMessage);
void AddSurfaceModeNodes( ActionsTreeModelNode * myScreenReaderNode, std::string strHash, MessageDefinition myMessage);
void AddActionNodes( ActionsTreeModelNode * myScreenReaderNode, std::string strHash, MessageDefinition myMessage);

	std::wstring GetActiveProductName( ActiveProduct apID);
	std::map <std::string, MessageDefinition> * pMyHardwareControls;
	boost::shared_ptr <AppConfig> pMyAppConfig;
ActionsTreeModelNode*   m_root;
};
#endif  // if mac
#endif  // guard
