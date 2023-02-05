/**************************************************************************\
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Fabrizio Morciano
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include "wx/wx.h"
#include "wx/button.h"
#include "wx/gbsizer.h"

#include "common/SimpleFrame.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;

        SimpleFrame *aFrame = new SimpleFrame(NULL,
                                              "thumb wheel",
                                              wxDefaultPosition,
                                              wxSize(300,300));

        wxGridBagSizer* sizer;
        sizer = new wxGridBagSizer( 0, 0 );
        sizer->SetFlexibleDirection( wxBOTH );
        sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

        wxPanel* left_panel;
        wxPanel* center_panel;
        wxPanel* right_panel;
        wxPanel* bottom_panel;

        left_panel = new wxPanel( aFrame, wxID_ANY);
        left_panel->SetBackgroundColour( wxColour( 255, 0, 0 ) );
        left_panel->SetMinSize( wxSize( 10,100 ) );
        //left_panel->SetMaxSize( wxSize( 10,200 ) );

        sizer->Add( left_panel, wxGBPosition( 0, 0 ), wxDefaultSpan, wxEXPAND | wxALL, 5 );

        center_panel = new wxPanel( aFrame, wxID_ANY);
        center_panel->SetBackgroundColour( wxColour( 0, 255, 0 ) );
        center_panel->SetMinSize( wxSize( 10,100 ) );
        //center_panel->SetMaxSize( wxSize( 10,200 ) );

        sizer->Add( center_panel, wxGBPosition( 0, 1 ), wxDefaultSpan, wxEXPAND | wxALL, 5 );

        right_panel = new wxPanel( aFrame, wxID_ANY);
        right_panel->SetBackgroundColour( wxColour( 0, 0, 255 ) );
        right_panel->SetMinSize( wxSize( 10,100 ) );
        // right_panel->SetMaxSize( wxSize( 10,200 ) );

        sizer->Add( right_panel, wxGBPosition( 0, 2 ), wxDefaultSpan, wxEXPAND | wxALL, 5 );
#if 1
        bottom_panel = new wxPanel( aFrame, wxID_ANY);
        bottom_panel->SetBackgroundColour( wxColour( 255, 255, 255 ) );
        bottom_panel->SetMinSize( wxSize( -1,20 ) );
        bottom_panel->Show();
        //bottom_panel->SetMaxSize( wxSize( -1,20 ) );
        sizer->Add( bottom_panel, wxGBPosition( 1, 0 ), wxGBSpan( 1, 3 ), wxEXPAND | wxALL, 5 );
#endif

        sizer->AddGrowableCol( 1 );
        sizer->AddGrowableRow( 0 );

        aFrame->SetSizer( sizer );
        aFrame->Layout();
        // aFrame->Fit();
        aFrame->Center();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
