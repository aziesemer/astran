#ifndef LAYERITEM_H
#define LAYERITEM_H

#include "../sight/vertex.h"

#include <vector>
	using std::vector;

#include <wx/wx.h>


// -----------------------------------------------------------------------------

class LayerList;

class LayerItem : public wxPanel {
	protected:
		wxStaticBitmap* clsBitmap;
		wxStaticText* clsName;
		wxCheckBox* clsVisibility;
		wxBoxSizer* clsBoxSizer;		
	
		void createBitmapFromMask(const unsigned char * mask, wxColor line, wxColor fill);
	
	public:
		LayerItem( LayerList* parent, int id, const unsigned char * mask,wxString name, wxColor line, wxColor fill );
		
		void setColor( const wxColour &c ) { SetBackgroundColour( c ); }
		void setFontColor( const wxColour &c ) { clsName->SetForegroundColour( c ); } 
}; // end class

// -----------------------------------------------------------------------------

class LayerList : public wxScrolledWindow {
	private:
		wxBoxSizer* clsBoxSizer;
		vector< LayerItem * > clsItems;
		
		LayerItem * clsSelectedItem;
	
		wxColour clsSelectedBackgroundColor;
		wxColour clsSelectedForegroundColor;
		wxColour clsNonSelectedBackgroundColor;
		wxColour clsNonSelectedForegroundColor;

		void selectItem( LayerItem * );
		void deselectItem( LayerItem * );

	public:
	
		LayerList( wxWindow *parent, const int ID );
	
		int getNumItems() const { return clsItems.size(); }
		LayerItem *getLayerItem( const int index ) { return clsItems[ index ]; }
		void addItem( LayerItem * newItem );
	
		void itemClicked( LayerItem *layerItem , const int ID );
		void itemChecked(  wxCommandEvent& event );
		
		void OnPaint(wxNcPaintEvent &event);		
		int	getLayerID() const{ return clsSelectedItem->GetId(); }
		
		DECLARE_EVENT_TABLE()	

}; // end class

// -----------------------------------------------------------------------------

class LayerItemEventHandler : public wxEvtHandler {
	private:
		LayerList *clsLayerList;
		LayerItem *clsLayerItem;
		
		void OnClick(wxMouseEvent& event);		
	
	
	public:
		LayerItemEventHandler( LayerList *layerList, LayerItem *layerItem )
			: clsLayerList( layerList ), clsLayerItem( layerItem ) {}
	
		
	DECLARE_EVENT_TABLE()
}; // end class

// -----------------------------------------------------------------------------

class wxLayerListEvent : public wxNotifyEvent { 
	public: 
		wxLayerListEvent(wxEventType commandType = wxEVT_NULL, int id = 0): wxNotifyEvent(commandType, id) {}
		wxLayerListEvent(const wxLayerListEvent& event) : wxNotifyEvent(event) 	{} 

		virtual wxEvent *Clone() const { return new wxLayerListEvent(*this); } 

	DECLARE_DYNAMIC_CLASS(wxLayerListEvent); 
}; 

typedef void (wxEvtHandler::*wxLayerListEventFunction) (wxLayerListEvent&);

BEGIN_DECLARE_EVENT_TYPES() 
	DECLARE_EVENT_TYPE(wxEVT_LAYER_LIST_SELECTION_CHANGED, 801) 
	DECLARE_EVENT_TYPE(wxEVT_LAYER_LIST_CHECK, 802) 
END_DECLARE_EVENT_TYPES() 

#define EVT_LAYER_LIST_SELECTION_CHANGED(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_LAYER_LIST_SELECTION_CHANGED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxLayerListEventFunction) & fn, (wxObject *) NULL ),

#define EVT_LAYER_LIST_CHECK(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_LAYER_LIST_CHECK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxLayerListEventFunction) & fn, (wxObject *) NULL ),

#endif







