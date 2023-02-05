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

#include <Inventor/Wx/common/pixmaps/pick.xpm>
#include <Inventor/Wx/common/pixmaps/view.xpm>
#include <Inventor/Wx/common/pixmaps/home.xpm>
#include <Inventor/Wx/common/pixmaps/set_home.xpm>
#include <Inventor/Wx/common/pixmaps/view_all.xpm>
#include <Inventor/Wx/common/pixmaps/seek.xpm>

wxButton* addButton(wxWindow* parent,
                    const char** xpm) {
    wxButton* p = new wxButton(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxImage img(xpm);
    p->SetBitmap(wxImage(img));
    //p->SetMaxSize(bmp.GetSize());
    return (p);
}

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;

        SimpleFrame *aFrame = new SimpleFrame(NULL,
                                              "thumb wheel",
                                              wxDefaultPosition);

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add( addButton(aFrame, pick_xpm), 0, wxALL, 1);
        sizer->Add( addButton(aFrame, view_xpm), 0, wxALL, 1);
        sizer->Add( addButton(aFrame, home_xpm), 0, wxALL, 1);
        sizer->Add( addButton(aFrame, set_home_xpm), 0, wxALL, 1);
        sizer->Add( addButton(aFrame, view_all_xpm), 0, wxALL, 1);
        sizer->Add( addButton(aFrame, seek_xpm), 0, wxALL, 1);
        aFrame->SetSizer(sizer);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
