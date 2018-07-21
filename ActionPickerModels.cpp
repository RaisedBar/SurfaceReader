// ActionPickerModels.cpp

#ifdef __WXOSX_COCOA__
#include "ActionPickerModels.h"

ActionPickerModel::ActionPickerModel( wxString wxstrRootTitle)
{
	m_root = new ActionPickerModelNode( NULL, wxstrRootTitle, SELECTED_CONTROL_LEVEL);

// Create an item for every available surface action type
				   	SurfaceAction myAction = SurfaceAction();

for (unsigned int i =first; i <= last; i++)
{
	SurfaceActionType mySurfaceActionType = static_cast<SurfaceActionType>( i); 
	ActionPickerModelNode * myTypeNode = new ActionPickerModelNode( m_root, myAction.GetSurfaceActionTypeDescription( mySurfaceActionType), mySurfaceActionType, SURFACE_ACTION_TYPE_LEVEL);
m_root->Append( myTypeNode);
}  // end for each action type
}


wxString ActionPickerModel::GetTitle( const wxDataViewItem &item ) const
{
    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();
    if (!node)      // happens if item.IsOk()==false
        return wxEmptyString;

	return node->m_title;
}


SurfaceActionType ActionPickerModel::GetSurfaceActionType( const wxDataViewItem &item ) const
{
    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();
    if (!node)      // happens if item.IsOk()==false
        return NoAction;

	return node->m_ActionType;
}


int ActionPickerModel::GetLevel( const wxDataViewItem &item ) const
{
    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();
    if (!node)      // happens if item.IsOk()==false
        return -1;

	return node->m_level;
}


int ActionPickerModel::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                               unsigned int column, bool ascending ) const
{
    wxASSERT(item1.IsOk() && item2.IsOk());
        // should never happen

    if (IsContainer(item1) && IsContainer(item2))
    {
        wxVariant value1, value2;
        GetValue( value1, item1, 0 );
        GetValue( value2, item2, 0 );

        wxString str1 = value1.GetString();
        wxString str2 = value2.GetString();
        int res = str1.Cmp( str2 );
        if (res) return res;

        // items must be different
        wxUIntPtr litem1 = (wxUIntPtr) item1.GetID();
        wxUIntPtr litem2 = (wxUIntPtr) item2.GetID();

        return litem1-litem2;
    }

    return wxDataViewModel::Compare( item1, item2, column, ascending );
}

void ActionPickerModel::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
    wxASSERT(item.IsOk());

    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();
    
	switch (col)
    {
	case SELECTED_CONTROL_LEVEL:
	case SURFACE_ACTION_TYPE_LEVEL:
{
	variant = node->m_title;
}
break;

		case SURFACE_ACTION_MODE_LEVEL:
		        {
			// variant = node->m_Action;
		}
		break;
    
    /*
	case PARAMETERS_LEVEL:
{}
break;
*/

		default:
        wxLogError( "ActionPickerModel::GetValue: wrong column %d", col );
    }
}


bool ActionPickerModel::SetValue( const wxVariant &variant,
                                 const wxDataViewItem &item, unsigned int col )
{
    wxASSERT( item.IsOk());

    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();
    switch (col)
    {
        case 0:
            {
				node->m_title = variant.GetString();
            return true;
			}
		
		case 1:
		case 2:
		case 3:
			{
				node->m_title = variant.GetString();
            return true;
			}

		default:
		{
            wxLogError( "ActionPickerModel::SetValue: wrong column" );
			return false;
		}
	}
    return false;
}


bool ActionPickerModel::IsEnabled( const wxDataViewItem &item, unsigned int col ) const
{
    wxASSERT(item.IsOk());

//     ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();

return true;
}


wxDataViewItem ActionPickerModel::GetParent( const wxDataViewItem &item ) const
{
    // the invisible root node has no parent
    if (!item.IsOk())
        return wxDataViewItem(0);

    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();

    // "MyMusic" also has no parent
    if (node == m_root)
        return wxDataViewItem(0);

    return wxDataViewItem( (void*) node->GetParent() );
}

bool ActionPickerModel::IsContainer( const wxDataViewItem &item ) const
{
    // the invisible root node can have children
    // (in our model always "MyMusic")
    if (!item.IsOk())
        return true;

    ActionPickerModelNode *node = (ActionPickerModelNode*) item.GetID();
    return node->IsContainer();
}


unsigned int ActionPickerModel::GetChildren( const wxDataViewItem &parent, wxDataViewItemArray &array ) const
{
    ActionPickerModelNode *node = (ActionPickerModelNode*) parent.GetID();
    if (!node)
    {
        array.Add( wxDataViewItem( (void*) m_root ) );
        return 1;
    }

    if (node->GetChildCount() == 0)
    {
        return 0;
    }

    unsigned int count = node->GetChildren().GetCount();
    for (unsigned int pos = 0; pos < count; pos++)
    {
        ActionPickerModelNode *child = node->GetChildren().Item( pos );
        array.Add( wxDataViewItem( (void*) child ) );
    }

    return count;
}


int ActionPickerModel::GetLevel( ActionPickerModelNode * myNode)
{
int nLevel = 0;
ActionPickerModelNode * myCurrentNode = myNode;
    
while (myCurrentNode != this->m_root)
{
	myCurrentNode = myCurrentNode->GetParent();
nLevel++;
	}
				
return nLevel;
}


#endif   // if Mac
