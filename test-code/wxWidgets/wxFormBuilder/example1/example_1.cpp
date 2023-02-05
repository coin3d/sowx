///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "example_1.h"

///////////////////////////////////////////////////////////////////////////

AFrame::AFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	status_bar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	menu_bar = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( file, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( about );

	wxMenuItem* quit;
	quit = new wxMenuItem( file, wxID_ANY, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( quit );

	menu_bar->Append( file, wxT("File") );

	this->SetMenuBar( menu_bar );

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );

	panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizer->Add( panel, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( sizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AFrame::on_about ), this, about->GetId());
	file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AFrame::on_quit ), this, quit->GetId());
	panel->Connect( wxEVT_SIZE, wxSizeEventHandler( AFrame::on_size ), NULL, this );
}

AFrame::~AFrame()
{
	// Disconnect Events
	panel->Disconnect( wxEVT_SIZE, wxSizeEventHandler( AFrame::on_size ), NULL, this );

}
