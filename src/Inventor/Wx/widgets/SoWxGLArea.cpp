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

#include "Inventor/Wx/widgets/SoWxGLArea.h"
#include "Inventor/Wx/SoWxGLWidgetP.h"
#include "sowxdefs.h"

#include "wx/wx.h"
#include "wx/file.h"
#include "wx/dcclient.h"

#include <GL/gl.h>
#include <map>

wxBEGIN_EVENT_TABLE(SoWxGLArea, wxGLCanvas)
                EVT_PAINT(SoWxGLArea::OnPaint)
                EVT_ERASE_BACKGROUND(SoWxGLArea::OnEraseBackground)
wxEND_EVENT_TABLE()

wxDEFINE_EVENT(SO_WX_GL_INIT, wxCommandEvent);
wxDEFINE_EVENT(SO_WX_GL_DRAW, wxCommandEvent);


SoWxGLArea::SoWxGLArea(wxWindow *parent,
                       const std::vector<int>& attributes)
        : wxGLCanvas(parent,
                     wxID_ANY,
                     &attributes[0],
                     wxDefaultPosition,
                     parent->GetClientSize()) {
    this->SetName("SoWxGLArea");

    gl_real_context = new wxGLContext(this);
    is_gl_initialized = false;
    gl_format = attributes;
}


SoWxGLArea::~SoWxGLArea() {
    delete gl_real_context;
}

void SoWxGLArea::OnPaint(wxPaintEvent& event ) {
#if SOWX_DEBUG
    SoDebugError::postInfo("SoWxGLArea::OnPaint",
                           "size:%d %d",
                           GetSize().x,
                           GetSize().y);
#endif

    // must always be here
    wxPaintDC dc(this);

    InitGL();

    wxCommandEvent a_wx_event(SO_WX_GL_DRAW, GetId());
    a_wx_event.SetEventObject(this);
    a_wx_event.SetString("SO_WX_GL_INIT");
    ProcessWindowEvent(a_wx_event);

    event.Skip();
}

void SoWxGLArea::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
    SOWX_STUB();
    // Do nothing, to avoid flashing on MSW
}

void SoWxGLArea::InitGL() {
    if(!is_gl_initialized) {
        SetCurrent(*gl_real_context);
        is_gl_initialized = true;

        wxCommandEvent a_wx_event(SO_WX_GL_INIT, GetId());
        a_wx_event.SetEventObject(this);
        a_wx_event.SetString("SO_WX_GL_INIT");
        ProcessWindowEvent(a_wx_event);
    } else {
        SetCurrent(*gl_real_context);
    }
}

void SoWxGLArea::makeCurrent() {
    if(gl_real_context)
        SetCurrent(*gl_real_context);
}

const wxGLContext *
SoWxGLArea::context() {
    return gl_real_context;
}

bool
isBoolean(int value) {
    bool res = false;
    switch(value ) {
        case WX_GL_RGBA:
        case WX_GL_DOUBLEBUFFER:
        case WX_GL_STEREO:
            res = true;
            break;
        default:
            break;
    }
    return (res);
}

bool
SoWxGLArea::isGLFeatureAvailable(const SoWxGLArea::GLFormat& format,
                                 int feature) {
    bool res = false;

    if(format.empty())
        return (res);
    if(format[0] == 0)
        return (res);

    assert(format.size()>1);

    for ( int arg = 0; format[arg] != 0; arg++ ) {

        // get the value for parameters that have a value or 0 for the last one
        int v = format[arg + 1];

        if (format[arg] == feature) {
            res = true;
            break;
        } else if (!isBoolean(format[arg])) {
            ++arg;
        } else {
            if (!v) {
                res = false;
                break;
            }
        }
    }
    return (res);
}

bool
SoWxGLArea::areEqual(const SoWxGLArea::GLFormat &format1,
                     const SoWxGLArea::GLFormat &format2) {
    SOWX_STUB();
    return (false);
}

bool ConvertWXAttrsWxGLFormat(const int *wxattrs,
                              SoWxGLArea::GLFormat  &format)
{
    if (!wxattrs)
        return true;

    for ( int arg = 0; wxattrs[arg] != 0; arg++ )
    {
        // indicates whether we have a boolean attribute
        bool isBoolAttr = false;

        int v = wxattrs[arg+1];
        switch ( wxattrs[arg] )
        {
            case WX_GL_BUFFER_SIZE:
                //format.setRgba(false);
                format.push_back(WX_GL_BUFFER_SIZE);
                // I do not know how to set the buffer size, so fail
                return false;

            case WX_GL_LEVEL:
                // format.setPlane(v);
                format.push_back(WX_GL_LEVEL);
                format.push_back(v);
                break;

            case WX_GL_RGBA:
                format[WX_GL_RGBA] = 1;
                // format.setRgba(true);
                isBoolAttr = true;
                break;

            case WX_GL_DOUBLEBUFFER:
                format[WX_GL_DOUBLEBUFFER] = 1;
                // format.setDoubleBuffer(true);
                isBoolAttr = true;
                break;

            case WX_GL_STEREO:
                format[WX_GL_STEREO] = 1;
                // format.setStereo(true);
                isBoolAttr = true;
                break;

            case WX_GL_AUX_BUFFERS:
                format[WX_GL_AUX_BUFFERS] = 1;
                // don't know how to implement
                return false;

            case WX_GL_MIN_RED:
                format[WX_GL_MIN_RED] = v*8;
                // format.setRedBufferSize(v*8);
                break;

            case WX_GL_MIN_GREEN:
                format[WX_GL_MIN_GREEN] = v;
                // format.setGreenBufferSize(v);
                break;

            case WX_GL_MIN_BLUE:
                format[WX_GL_MIN_BLUE] = v;
                // format.setBlueBufferSize(v);
                break;

            case WX_GL_MIN_ALPHA:
                format[WX_GL_MIN_ALPHA] = v;
                // format.setAlpha(true);
                // format.setAlphaBufferSize(v);
                break;

            case WX_GL_DEPTH_SIZE:
                format[WX_GL_DEPTH_SIZE] = v;
                // format.setDepth(true);
                // format.setDepthBufferSize(v);
                break;

            case WX_GL_STENCIL_SIZE:
                format[WX_GL_STENCIL_SIZE] = v;
                // format.setStencil(true);
                // format.setStencilBufferSize(v);
                break;

            case WX_GL_MIN_ACCUM_RED:
            case WX_GL_MIN_ACCUM_GREEN:
            case WX_GL_MIN_ACCUM_BLUE:
            case WX_GL_MIN_ACCUM_ALPHA:
                format[WX_GL_MIN_ACCUM_RED] = v;
                format[WX_GL_MIN_ACCUM_GREEN] = v;
                format[WX_GL_MIN_ACCUM_BLUE] = v;
                format[WX_GL_MIN_ACCUM_ALPHA] = v;
                // format.setAccumBufferSize(v);
                break;

            case WX_GL_SAMPLE_BUFFERS:
                format[WX_GL_SAMPLE_BUFFERS] = v;
                // format.setSampleBuffers(v);
                // can we somehow indicate if it's not supported?
                break;

            case WX_GL_SAMPLES:
                format[WX_GL_SAMPLES] = v;
                // format.setSamples(v);
                // can we somehow indicate if it's not supported?
                break;

            case WX_GL_MAJOR_VERSION:
                format[WX_GL_MAJOR_VERSION] = v;
                // format.setVersion ( v,0 );
                break;

            default:
                wxLogDebug(wxT("Unsupported OpenGL attribute %d"),
                           wxattrs[arg]);
                continue;
        }

        if ( !isBoolAttr ) {
            if ( !v )
                return false; // zero parameter
            arg++;
        }
    }

    return true;
}
