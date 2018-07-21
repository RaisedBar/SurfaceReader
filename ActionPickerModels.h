// ActionPickerModels.hpp


#ifndef ACTION_PICKER_MODELS_H
#define ACTION_PICKER_MODELS_H

#pragma once

#include <wx/platform.h>
#ifdef __WXOSX_COCOA__

#include "AppConfig.h"
#include "MessageDefinition.h"
#include "RBStringFuncs.h"
#include "SRConstants.h"
#include "RBException.h"

#include <wx/wx.h>
#include "wx/dataview.h"
#include "null.xpm"
#include "wx_small.xpm"

const int SELECTED_CONTROL_LEVEL = 0;
const int SURFACE_ACTION_TYPE_LEVEL = 1;
const int SURFACE_ACTION_MODE_LEVEL = 2;


class ActionPickerModelNode;
WX_DEFINE_ARRAY_PTR( ActionPickerModelNode*, ActionPickerModelNodePtrArray );

class ActionPickerModelNode
{
public:
	ActionPickerModelNode( ActionPickerModelNode* parent, const wxString &title, int nLevel)
    {
        m_parent = parent;
m_title = title;
m_level = nLevel;
m_container = true;
    }

    
	    ActionPickerModelNode( ActionPickerModelNode* parent, const wxString &title, SurfaceActionType myActionType, int nLevel)
    {
        m_parent = parent;
m_title = title;
m_ActionType = myActionType;
m_level = nLevel;
m_container = false;
    }

    
~ActionPickerModelNode()
    {
        // free all our children nodes
        size_t count = m_children.GetCount();
        
		for (size_t i = 0; i < count; i++)
        {
            ActionPickerModelNode *child = m_children[i];
            delete child;
        }
    }

    bool IsContainer() const
        { 
			return m_container; 
	}

    ActionPickerModelNode* GetParent()
        { 
			return m_parent; 
	}

	ActionPickerModelNodePtrArray& GetChildren()
        { 
			return m_children; 
	}
    
	void Insert( ActionPickerModelNode* child, unsigned int n)
        { 
			m_children.Insert( child, n); 
	}

	void Append( ActionPickerModelNode* child )
        { 
			m_children.Add( child ); 
	}

	unsigned int GetChildCount() const
        { 
			return m_children.GetCount(); 
	}

wxString                m_title;
SurfaceActionType m_ActionType;
	int m_level;
bool m_container;

private:
ActionPickerModelNode          *m_parent;
    ActionPickerModelNodePtrArray   m_children;
};


class ActionPickerModel: public wxDataViewModel
{
public:
    ActionPickerModel( wxString wxstrRootTitle);
    
	~ActionPickerModel()
    {
        delete m_root;
    }

    wxString GetTitle( const wxDataViewItem &item) const;
SurfaceActionType GetSurfaceActionType( const wxDataViewItem &item) const;
int GetLevel( const wxDataViewItem &item ) const;
    
	// override sorting to always sort branches ascendingly
int Compare( const wxDataViewItem &item1, const wxDataViewItem &item2, unsigned int column, bool ascending ) const;

    // implementation of base class virtuals to define model

    virtual unsigned int GetColumnCount() const
    {
        return SURFACE_ACTION_MODE_LEVEL +1;
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
    	int GetLevel( ActionPickerModelNode * myNode);
	
ActionPickerModelNode*   m_root;
};
#endif  // if OSX
#endif  // guard

