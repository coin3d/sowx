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
#define  SOWX_INTERNAL
#include <Inventor/Wx/SoWx.h>
#include "Inventor/Wx/SoWxP.h"
#define protected public
#include <Inventor/Wx/viewers/SoWxFullViewer.h>
#undef protected

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>

#include "wx/wx.h"

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
                                              wxSize(400,400));

        wxWindow* window = SoWx::init("renderarea");

        SoWxFullViewer * renderarea =
                new SoWxFullViewer(window,
                                   "Renderarea demonstration",
                                   FALSE,
                                   SoWxFullViewer::BUILD_ALL,
                                   SoWxViewer::BROWSER,
                                   FALSE);
        wxBoxSizer* vertical_sizer = new wxBoxSizer(wxVERTICAL);

#if 1
        wxFlexGridSizer* horizontal_sizer = new wxFlexGridSizer(1,3,0,0);

        wxWindow* left_panel;
        wxWindow* center_panel;
        wxWindow* right_panel;

        horizontal_sizer->AddGrowableCol(1);
        horizontal_sizer->AddGrowableRow(0);
        left_panel = renderarea->buildLeftTrim(aFrame);
        left_panel->SetBackgroundColour( wxColour( 255, 0, 0 ) );

        horizontal_sizer->Add( left_panel, 0, wxEXPAND|wxALL,0);

        center_panel = new wxPanel( aFrame, wxID_ANY);
        center_panel->SetBackgroundColour( wxColour( 0, 255, 0 ) );
        center_panel->SetSize(100,100);
        horizontal_sizer->Add( center_panel, 0, wxEXPAND|wxALL,0);

        right_panel = renderarea->buildRightTrim(aFrame);
        right_panel->SetBackgroundColour( wxColour( 0, 0, 255 ) );

        horizontal_sizer->Add( right_panel, 0, wxEXPAND|wxALL,0);
#endif
#if 1
        wxWindow* bottom_panel;

        bottom_panel = renderarea->buildBottomTrim(aFrame);
        bottom_panel->SetBackgroundColour( wxColour( 255, 255, 255 ) );

        vertical_sizer->Add(bottom_panel, 0, wxEXPAND|wxALL,0);
        vertical_sizer->Add(horizontal_sizer, 0, wxEXPAND|wxALL,0);
#endif

        aFrame->SetSizer( vertical_sizer );
        aFrame->Layout();
        // aFrame->Fit();
        aFrame->Center();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
