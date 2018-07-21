// ActionsPageModels.cpp

#ifdef __WXOSX_COCOA__
#include "ActionsPageModels.h"


// ----------------------------------------------------------------------------
// ActionsTreeModel
// ----------------------------------------------------------------------------

ActionsTreeModel::ActionsTreeModel( wxString wxstrRootTitle, std::map <std::string , MessageDefinition> * pHardwareControls, boost::shared_ptr <AppConfig> pAppConfig)
{
pMyHardwareControls = pHardwareControls;
pMyAppConfig = pAppConfig;
	m_root = new ActionsTreeModelNode( NULL, wxstrRootTitle, PROTOCOL_LEVEL);

// Create an item for every available hardware control
				std::map <std::string, MessageDefinition>::iterator it;

for (it = pMyHardwareControls->begin(); it != pMyHardwareControls->end(); it++)
{
	ActionsTreeModelNode * myControlNode = new ActionsTreeModelNode( m_root, it->second.GetLabel(), CONTROL_LEVEL);
AddScreenReaderNodes( myControlNode, it->first, it->second);
m_root->Append( myControlNode);
}  // end for each control
}


void ActionsTreeModel::AddScreenReaderNodes( ActionsTreeModelNode * myControlNode, std::string strHash, MessageDefinition myMessage)
{
for  (unsigned int i =ID_JAWS; i <= ID_MAC; i++)
{
ActiveProduct myActiveProduct = static_cast<ActiveProduct>( i); 	
ActionsTreeModelNode * myScreenReaderNode = new ActionsTreeModelNode( myControlNode, GetActiveProductName( myActiveProduct), SPEECH_LEVEL);
AddSurfaceModeNodes( myScreenReaderNode, strHash, myMessage);
myControlNode->Append( myScreenReaderNode);
}  // end for
}


void ActionsTreeModel::AddSurfaceModeNodes( ActionsTreeModelNode * myScreenReaderNode , std::string strHash, MessageDefinition myMessage)
{
ActionsTreeModelNode * myLiveModeNode = new ActionsTreeModelNode( myScreenReaderNode, wstrLiveMode, MODE_LEVEL);
AddActionNodes( myLiveModeNode, strHash, myMessage);
myScreenReaderNode->Append( myLiveModeNode);

ActionsTreeModelNode * myScreenReaderModeNode = new ActionsTreeModelNode( myScreenReaderNode, wstrScreenReaderMode, SPEECH_LEVEL);
AddActionNodes( myScreenReaderModeNode, strHash, myMessage);
myScreenReaderNode->Append( myScreenReaderModeNode);
}

	
void ActionsTreeModel::AddActionNodes( ActionsTreeModelNode * myModeNode, std::string strHash, MessageDefinition myMessage)
{
// Create a child node for every defined SurfaceAction contained within this hardware control
ActiveProduct myProduct = GetProductFromNode( myModeNode);
SurfaceActions myActions; 

try
	{
		myActions = myMessage.GetSurfaceActions( myProduct);
}
catch( RBException &e)
	{
return;		
}

if (IsLiveModeNode( myModeNode))
{
	for  (int i = 0; i < myActions.GetLiveModeActionCount(); i++)
	{
		try
			{
				std::wstring  wstrNodeLabel = myActions.GetLiveModeAction( i).GetSurfaceActionTypeDescription( myActions.GetLiveModeAction( i).GetSurfaceActionType());
wstrNodeLabel.append( GetActionParameterDescription( myActions.GetLiveModeAction( i)));
ActionsTreeModelNode * myActionNode = new ActionsTreeModelNode( myModeNode, wstrNodeLabel, ACTION_LEVEL);
myModeNode->Append( myActionNode);
}  // end try
catch( ...)
{
break;
}
	}  // end for
		return;
	}  // end if live mode
	else
{
	if (IsScreenReaderModeNode( myModeNode))
{
	for  (int i = 0; i < myActions.GetLiveModeActionCount(); i++)
	{
		try
			{
				std::wstring  wstrDescription = myActions.GetScreenReaderModeAction( i).GetSurfaceActionTypeDescription( myActions.GetScreenReaderModeAction( i).GetSurfaceActionType());
		ActionsTreeModelNode * myActionNode = new ActionsTreeModelNode( myModeNode, wstrDescription, ACTION_LEVEL);
myModeNode->Append( myActionNode);
		}  // end try
	catch( ...)
	{
break;
	} // end catch
	}  // end for
	
return;
}  // end if ScreenReader mode
	}  // end else for valid modes
	
	return;
}


bool ActionsTreeModel::AddAction( const wxDataViewItem &item, const wxString &wxstrTitle)
{
ActionsTreeModelNode * node = (ActionsTreeModelNode*) item.GetID();
    
if (!node)      // happens if item.IsOk()==false
{
	return false;
}
else
{
ActionsTreeModelNode * myActionNode = new ActionsTreeModelNode( node, wxstrTitle, ACTION_LEVEL);

node->Append( myActionNode);
return true;
}  // end if OK node
}


std::wstring ActionsTreeModel::GetTitle( const wxDataViewItem &item ) const
{
    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
    if (!node)      // happens if item.IsOk()==false
        return L"";

	return node->m_title;
}


bool ActionsTreeModel::SetTitle( wxDataViewItem item, std::wstring  wstrTitle)
{
    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
    
	if (!node)      // happens if item.IsOk()==false
        {
			return false;
	}

node->m_title = wstrTitle;
	return true;
}


int ActionsTreeModel::GetLevel( const wxDataViewItem &item ) const
{
    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
    if (!node)      // happens if item.IsOk()==false
        return -1;

	return node->m_level;
}


void ActionsTreeModel::Delete( const wxDataViewItem &item )
{
    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
    if (!node)      // happens if item.IsOk()==false
        {
			return;
	}
    
wxDataViewItem parent( node->GetParent() );

	if (!parent.IsOk())
    {
        wxASSERT(node == m_root);

        // don't make the control completely empty:
        wxLogError( "Cannot remove the root item!" );
        return;
    }

    // first remove the node from the parent's array of children;
    // NOTE: ActionsTreeModelNodePtrArray is only an array of _pointers_
    //       thus removing the node from it doesn't result in freeing it
    node->GetParent()->GetChildren().Remove( node );

    // free the node
    delete node;

    // notify control
    ItemDeleted( parent, item );
}

int ActionsTreeModel::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
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

void ActionsTreeModel::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
    wxASSERT(item.IsOk());

    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
    
	switch (col)
    {
    case PROTOCOL_LEVEL:
	case CONTROL_LEVEL:
	case SPEECH_LEVEL:
	case MODE_LEVEL:
{
	variant = node->m_title;
}
break;

		case ACTION_LEVEL:
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
        wxLogError( "ActionsTreeModel::GetValue: wrong column %d", col );
    }
}

bool ActionsTreeModel::SetValue( const wxVariant &variant,
                                 const wxDataViewItem &item, unsigned int col )
{
    wxASSERT(item.IsOk());

    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
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
            wxLogError( "ActionsTreeModel::SetValue: wrong column" );
    }
    return false;
}


bool ActionsTreeModel::IsEnabled( const wxDataViewItem &item, unsigned int col ) const
{
    wxASSERT(item.IsOk());

    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();

return true;
}


wxDataViewItem ActionsTreeModel::GetParent( const wxDataViewItem &item ) const
{
    // the invisible root node has no parent
    if (!item.IsOk())
        return wxDataViewItem(0);

    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();

    // "MyMusic" also has no parent
    if (node == m_root)
        return wxDataViewItem(0);

    return wxDataViewItem( (void*) node->GetParent() );
}

bool ActionsTreeModel::IsContainer( const wxDataViewItem &item ) const
{
    // the invisible root node can have children
    // (in our model always "MyMusic")
    if (!item.IsOk())
        return true;

    ActionsTreeModelNode *node = (ActionsTreeModelNode*) item.GetID();
    return node->IsContainer();
}


unsigned int ActionsTreeModel::GetChildren( const wxDataViewItem &parent, wxDataViewItemArray &array ) const
{
    ActionsTreeModelNode *node = (ActionsTreeModelNode*) parent.GetID();
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
        ActionsTreeModelNode *child = node->GetChildren().Item( pos );
        array.Add( wxDataViewItem( (void*) child ) );
    }

    return count;
}


std::wstring ActionsTreeModel::GetActiveProductName( ActiveProduct apID)
	{
switch (apID)
{
case ID_NONE:
	{
		return wstrNone;
	}
	break;

case ID_JAWS:
	{
		return wstrJaws;
	}
	break;

case ID_SYSTEMACCESS:
	{
		return wstrSA;
	}
	break;

case ID_WindowEyes:
	{
		return wstrWE;
	}
	break;

case ID_NVDA:
	{
		return wstrNVDA;
	}
	break;

case ID_DOLPHIN:
{
		return wstrDolphin;
	}
	break;

case ID_MAC:
{
return wstrMac;
	}
break;

default:
{
		return wstrNone;
	}
	break;
}  // end switch
	}

	
int ActionsTreeModel::GetLevel( ActionsTreeModelNode * myNode)
{
int nLevel = 0;
ActionsTreeModelNode * myCurrentNode = myNode;
    
while (myCurrentNode != this->m_root)
{
	myCurrentNode = myCurrentNode->GetParent();
nLevel++;
	}
				
return nLevel;
}


ActiveProduct ActionsTreeModel::GetProductFromNode( ActionsTreeModelNode * myNode)
{
	ActiveProduct myProduct = ID_NONE;
ActionsTreeModelNode * myCurrentNode = myNode;
    
while ((myCurrentNode != this->m_root) && (GetLevel( myCurrentNode) > SPEECH_LEVEL))
{
	myCurrentNode = myCurrentNode->GetParent();
	}  // end while
				
myProduct = GetProductFromName( myCurrentNode->m_title);
return myProduct;
}


bool ActionsTreeModel::IsLiveModeNode( ActionsTreeModelNode * myNode)
{
	bool blnResult = false;

	if (GetLevel(myNode) < MODE_LEVEL)
{
return blnResult;
}

	ActionsTreeModelNode  * myCurrentNode = myNode;
    
	while ((myCurrentNode != this->m_root) && (GetLevel( myCurrentNode) > MODE_LEVEL))
{
	myCurrentNode = myCurrentNode->GetParent();
	}  // end while
				
	if (myCurrentNode->m_title.compare( wstrLiveMode) == 0)
{
blnResult = true;
}

return blnResult;
}


bool ActionsTreeModel::IsScreenReaderModeNode( ActionsTreeModelNode * myNode)
{
	bool blnResult = false;

if (GetLevel(myNode) < MODE_LEVEL)
{
return blnResult;
}

	ActionsTreeModelNode * myCurrentNode = myNode;
    
	while ((myNode != this->m_root) && (GetLevel( myCurrentNode) > MODE_LEVEL))
{
	myCurrentNode = myCurrentNode->GetParent();
	}  // end while
				
	if (myCurrentNode->m_title.compare( wstrScreenReaderMode) == 0)
{
blnResult = true;
}

return blnResult;
}


ActiveProduct ActionsTreeModel::GetProductFromName( std::wstring wstrName)
{
ActiveProduct myProduct = ID_NONE;

if (wstrName.compare( wstrNone) == 0)
{
		return ID_NONE;
	}
	
if (wstrName.compare( wstrJaws) == 0)
{
		return ID_JAWS;
	}
	
if (wstrName.compare( wstrSA) == 0)
{
		return ID_SYSTEMACCESS;
	}
	
if (wstrName.compare( wstrWE) == 0)
{
		return ID_WindowEyes;
	}
	
if (wstrName.compare( wstrNVDA) == 0)
{
		return ID_NVDA;
	}
	
if (wstrName.compare( wstrDolphin) == 0)
{
		return ID_DOLPHIN;
	}
	
if (wstrName.compare( wstrMac) == 0)
{
		return ID_MAC;
	}
	
return myProduct;
}


std::wstring ActionsTreeModel::GetActionParameterDescription( SurfaceAction mySA)
{
	std::wstring  wstrDescription;
unsigned int nParameters = mySA.GetParameterCount();

for (unsigned int i = 0; i < nParameters; i++)
{
	switch (mySA.GetSurfaceActionType())
		{
	case ReadDisplay:
		case ReadDisplayLine:
		case ReadDisplayStrip:
		case ReadDisplaySection:
			{
if (i == 0)
	{
		// Get the name of the targeted display
		wxString wxstrDisplayHash = mySA.GetParameter( i).second;
		wstrDescription.append( pMyAppConfig->GetDisplayDefinition( wxstrDisplayHash.ToStdString()).GetLabel());
}
else
	{
		wstrDescription.append( mySA.GetParameter( i).second);
wstrDescription.append( wstrSpacedComma);
}
			}
			break;

default:
	{
		wstrDescription.append( mySA.GetParameter( i).second);

		if (i < (nParameters -1))
	{
wstrDescription.append( L", ");
}
			}
break;
}  // end switch
}  // end for
	
return wstrDescription;
}
#endif   // If Mac


