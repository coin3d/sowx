/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*
  Demonstrates embedding of an SoWxExaminerViewer within a simple
  widget hierarchy.
*/

/***********************************************************************/

#include "Inventor/Wx/SoWx.h"
#include "Inventor/Wx/viewers/SoWxExaminerViewer.h"
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#include <wx/frame.h>

class SplitFrame : public wxFrame
{
private:

protected:
    wxSplitterWindow* splitter;
    wxPanel* panel1;
    wxPanel* panel2;

public:

    SplitFrame( wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxString& title = wxEmptyString,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxSize( 500,300 ),
                  long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL )
            : wxFrame( parent, id, title, pos, size, style )
    {
        this->SetSizeHints( wxDefaultSize, wxDefaultSize );

        wxBoxSizer* bSizer12;
        bSizer12 = new wxBoxSizer( wxHORIZONTAL );

        splitter = new wxSplitterWindow( this, wxID_ANY);
        splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( SplitFrame::onIdle ), NULL, this );
        splitter->Bind( wxEVT_SPLITTER_SASH_POS_CHANGING, &SplitFrame::onSplitterMovement, this);

        panel1 = new wxPanel( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        panel1->SetBackgroundColour(wxColour(125,0,125));
        panel2 = new wxPanel( splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

        // Construct a simple scenegraph.

        SoSeparator * root = new SoSeparator;

        SoCone * cone = new SoCone;
        root->addChild(cone);

        // Add the examinerviewer.

        SoWxExaminerViewer * examinerviewer = new SoWxExaminerViewer(panel2);
        examinerviewer->setSceneGraph(root);
        examinerviewer->show();

        splitter->SplitVertically( panel1, panel2, 0 );
        bSizer12->Add( splitter, 1, wxEXPAND, 5 );

        this->SetSizer( bSizer12 );
        this->Layout();

        this->Centre( wxBOTH );
    }

    ~SplitFrame() {
    }

    void onSplitterMovement(wxSplitterEvent& event) {
        std::cerr<<"move: "<<event.GetSashPosition()<<std::endl;

    }

    void onIdle( wxIdleEvent& )
    {
        splitter->SetSashPosition( 0 );
        splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( SplitFrame::onIdle ), NULL, this );
    }

};

/***********************************************************************/

int
main(int argc, char ** argv)
{
    // Initialize SoWx.
    SoWx::init((wxWindow *)NULL);

    // Set up scrollview window.
    SplitFrame * vp = new SplitFrame(0);

    // show window
    vp->Show();

    // start event loop.
    SoWx::mainLoop();

    // remove all resources
    SoWx::done();
    return 0;
}
