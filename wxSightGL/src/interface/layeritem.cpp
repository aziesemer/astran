#include "layeritem.h"

#include <iostream>
	using namespace std;
#define printme( e ) { cout << #e << " = " << ( e ) << "\n"; } 

BEGIN_EVENT_TABLE( LayerItemEventHandler, wxEvtHandler )
	EVT_LEFT_UP( LayerItemEventHandler::OnClick)		
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( LayerList, wxEvtHandler )
//	EVT_NC_PAINT(LayerList::OnPaint)	
	EVT_CHECKBOX(wxID_ANY, LayerList::itemChecked)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_LAYER_LIST_CHECK) 
DEFINE_EVENT_TYPE(wxEVT_LAYER_LIST_SELECTION_CHANGED) 
IMPLEMENT_DYNAMIC_CLASS(wxLayerListEvent, wxNotifyEvent)

// -----------------------------------------------------------------------------

LayerItem::LayerItem( LayerList* parent, int id, const unsigned char * mask, wxString name, wxColor line, 
											wxColor fill ) : wxPanel( parent, id ) {
											
	clsBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	clsVisibility = new wxCheckBox( this, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	clsVisibility->SetValue(true);
	clsBoxSizer->Add( clsVisibility, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 3 );
		
	clsBitmap = new wxStaticBitmap( this, id, wxNullBitmap, wxDefaultPosition, wxSize( 16, 16 ), 0 );
	clsBoxSizer->Add( clsBitmap, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	clsName = new wxStaticText( this, id, name, wxDefaultPosition, wxDefaultSize, 0 );
	clsName->Wrap( -1 );
	clsBoxSizer->Add( clsName, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 3 );
		
	createBitmapFromMask(mask, line, fill);
	
	this->SetSizer( clsBoxSizer );
	this->Layout();
	
	// [TODO] Can create just one EvtHandler and push it on each objetc without crashing the objects?
	clsName->PushEventHandler(  new LayerItemEventHandler( parent, this ) );
	clsBitmap->PushEventHandler(  new LayerItemEventHandler( parent, this ) );
	PushEventHandler(  new LayerItemEventHandler( parent, this ) );	
}

// -----------------------------------------------------------------------------

void LayerItem::createBitmapFromMask(const unsigned char * mask, wxColor line, wxColor fill) {
	struct Pixel {
		unsigned char x, y, z;
	};
	
	Color *data = new Color[ 32*32 ];

	Color backgroundColor = Color();
	Color borderColor = Color(line.Red(), line.Green(), line.Blue());
	
	Color fillColor = Color(fill.Red(), fill.Green(), fill.Blue()) ;

	// Interior
	for ( int i = 0; i < 128; i++ ) {
		unsigned char byte = mask[ i ];
		for ( int k = 0; k < 8; k++ ) {
			const int index = i * 8;
			if ( byte & ( ( (unsigned char) 128 ) >> k ) )
				data[ index + k ] = fillColor;
			else
				data[ index + k ] = backgroundColor;
		} // end for k
	} // end for i
	
	wxImage image;
	image.SetData( (unsigned char*) data, 32, 32 );

	const int w = clsBitmap->GetSize().x;
	const int h = clsBitmap->GetSize().y;

	image.Resize( wxSize( w, h ), wxPoint( 0, 0 ) ); // Remark: this delete the contents of data
	
	// Border
	data = (Color * ) image.GetData();
	for ( int i1 =       0; i1 <       w; i1 += 1 ) data[ i1 ] = borderColor; // top
	for ( int i2 =       w; i2 < w*(h-1); i2 += w ) data[ i2 ] = borderColor; // left
	for ( int i3 =   w*2-1; i3 <     w*h; i3 += w ) data[ i3 ] = borderColor; // right
	for ( int i4 = w*(h-1); i4 <     w*h; i4 += 1 ) data[ i4 ] = borderColor; // bottom

	// Set the new layer bitmap.
	clsBitmap->SetBitmap( wxBitmap( image ) );
} // end method

// -----------------------------------------------------------------------------

LayerList::LayerList( wxWindow *parent, const int ID ) : wxScrolledWindow( parent, ID, wxPoint(0,0), wxSize(130,100) ) {
	clsBoxSizer = new wxBoxSizer( wxVERTICAL );	

	SetScrollbars( 20, 20, 50, 630 );

	clsSelectedBackgroundColor = wxColour(  51, 111, 203 );
	clsSelectedForegroundColor = wxColour( 255, 255, 255 );

	clsNonSelectedBackgroundColor = wxColour( 255, 255, 255 );
	clsNonSelectedForegroundColor = wxColour(   0,   0,   0 );

	SetBackgroundColour( clsNonSelectedBackgroundColor );
	
	clsSelectedItem = NULL;


	this->SetSizer( clsBoxSizer );
	this->Layout();
} // end constructor

// -----------------------------------------------------------------------------

void LayerList::addItem( LayerItem * newItem ) { 
	clsItems.push_back( newItem ); 
	if ( clsItems.size() == 1 )
		selectItem( newItem );
	clsBoxSizer->Add( newItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0);
} // end method

// -----------------------------------------------------------------------------

void LayerList::itemClicked( LayerItem *layerItem, const int ID ){
	if( clsSelectedItem != layerItem ) {
		if ( clsSelectedItem )
			deselectItem( clsSelectedItem );
		selectItem( layerItem );
	}

	wxLayerListEvent event( wxEVT_LAYER_LIST_SELECTION_CHANGED, GetId()); 
	event.SetEventObject(this); 
	GetEventHandler()->ProcessEvent(event);
}

// -----------------------------------------------------------------------------

void LayerList::itemChecked( wxCommandEvent& eventt ){
	int ID = eventt.GetId();
	wxLayerListEvent event( wxEVT_LAYER_LIST_CHECK, GetId() ); 
	event.SetEventObject(eventt.GetEventObject() ); 
	event.SetInt(ID);
	GetEventHandler()->ProcessEvent(event);
}
		
// -----------------------------------------------------------------------------

void LayerList::selectItem( LayerItem *item ) {
	clsSelectedItem = item;	
	item->setColor( clsSelectedBackgroundColor );
	item->setFontColor( clsSelectedForegroundColor );
	item->Refresh( true );
} // end method

// -----------------------------------------------------------------------------

void LayerList::deselectItem( LayerItem *item ) {
	item->setColor( clsNonSelectedBackgroundColor );
	item->setFontColor( clsNonSelectedForegroundColor );
	item->Refresh( true );
} // end method

// -----------------------------------------------------------------------------
		
void LayerItemEventHandler::OnClick(wxMouseEvent& event) {
	clsLayerList->itemClicked(  clsLayerItem, event.GetId() );
	event.Skip(); // propagate event
} // end method






