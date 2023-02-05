///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AFrame
///////////////////////////////////////////////////////////////////////////////
class AFrame : public wxFrame
{
	private:

	protected:
		wxStatusBar* status_bar;
		wxMenuBar* menu_bar;
		wxMenu* file;
		wxPanel* panel;

		// Virtual event handlers, override them in your derived class
		virtual void on_about( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_quit( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_size( wxSizeEvent& event ) { event.Skip(); }


	public:

		AFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AFrame();

};

