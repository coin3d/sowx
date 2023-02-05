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
#include <wx/glcanvas.h>

class SoWxGLArea : public wxGLCanvas
{
public:
    SoWxGLArea(wxWindow *parent,
               wxGLAttributes& attributes)
#if 0
            : wxGLCanvas(parent,
                         attributes,
                         wxID_ANY,
                         wxDefaultPosition,
                         parent->GetClientSize())
#else
            : wxGLCanvas(parent,
                         wxID_ANY,
                         NULL,
                         wxDefaultPosition,
                         parent->GetClientSize())
#endif
                         {
        gl_real_context = new wxGLContext(this);
        is_gl_initialized = false;
    }


    virtual ~SoWxGLArea() {
        delete gl_real_context;
    }

protected:
    void OnPaint(wxPaintEvent& event) {
        // must always be here
        wxPaintDC dc(this);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3,0.3,0.3,1.0);

        InitGL();
        event.Skip();
        std::cout<<"OnPaint"<<std::endl;
        SwapBuffers();
    }

    void OnEraseBackground(wxEraseEvent& event) {
        event.Skip();
        std::cout<<"OnEraseBackground"<<std::endl;
    }

    void OnSize(wxSizeEvent& event) {
        event.Skip();
        std::cout<<"OnSize"<<std::endl;
    }

private:
    void InitGL() {
        if(!is_gl_initialized) {
            SetCurrent(*gl_real_context);
            is_gl_initialized = true;
        } else {
            SetCurrent(*gl_real_context);
        }
    }

    bool is_gl_initialized;
    wxGLContext* gl_real_context;

wxDECLARE_NO_COPY_CLASS(SoWxGLArea);
wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(SoWxGLArea, wxGLCanvas)
                EVT_PAINT(SoWxGLArea::OnPaint)
                EVT_ERASE_BACKGROUND(SoWxGLArea::OnEraseBackground)
                EVT_SIZE(SoWxGLArea::OnSize)
wxEND_EVENT_TABLE()

#define USE_PANEL 1

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;
        wxFrame* w = new wxFrame(0, wxID_ANY, "Hello");
        w->SetBackgroundColour(wxColour(244,0,0));
#if USE_PANEL
        wxPanel *panel = new wxPanel(w);
        panel->SetBackgroundColour(wxColour(0, 0, 255));

        wxFlexGridSizer* sizer = new wxFlexGridSizer(0);
        sizer->AddGrowableCol(0);
        sizer->AddGrowableRow(0);
        panel->SetSizer(sizer);
#endif
        wxGLAttributes attributes;
        attributes.PlatformDefaults();
        attributes.RGBA();
        attributes.EndList();
#if USE_PANEL
        SoWxGLArea* area = new SoWxGLArea(panel, attributes);
        sizer->Add(area, 0, wxEXPAND|wxALL, 0);
        sizer->Layout();
#else
        SoWxGLArea* area = new SoWxGLArea(w, attributes);
#endif
        area->SetBackgroundColour(wxColour(0,255,0));

        w->Show();
        return true;
    }
};



wxIMPLEMENT_APP(MyApp);
