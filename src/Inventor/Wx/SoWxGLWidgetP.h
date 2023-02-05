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
#ifndef SOWX_SOWXGLWIDGETP_H
#define SOWX_SOWXGLWIDGETP_H

#include "Inventor/Wx/SoGuiGLWidgetP.h"
#include "Inventor/Wx/SoWxGLWidget.h"

#include <Inventor/SbVec2s.h>

#include <wx/glcanvas.h>
#include <wx/wx.h>
#include <wx/timer.h>

#include <set>
#include <vector>

class SoWxGLArea;

class SoWxGLWidgetP :  public SoGuiGLWidgetP
{
public:

    explicit SoWxGLWidgetP(SoWxGLWidget * publ);

    virtual ~SoWxGLWidgetP();

    void initGLModes(int);

    std::vector<int> gl_attributes;
    SoWxGLArea* buildGLWidget();

    SbVec2s glSize;
    SbVec2s glSizeUnscaled;
    SbBool wasresized;

    wxWindow * currentglwidget;
    wxWindow * previousglwidget;
    SoWxGLArea * currentglarea;
    SoWxGLArea * previousglarea;
    wxWindow * glparent;

    int borderthickness;

    const wxGLContext * oldcontext;

    void gl_init(wxCommandEvent&);
    void gl_reshape(wxSizeEvent&);
    void gl_exposed(wxCommandEvent&);
    void onMouse(wxMouseEvent&);
    void onKey(wxKeyEvent&);

    static bool isAPanel(wxWindow*);
    void addSizer();

    bool hasZBuffer() const;
    bool hasOverlay() const;

    // Required by the common code
    static void eventHandler(wxWindow*, void*, wxEvent&, bool*);

protected:
    virtual SbBool isDirectRendering(void);

    const wxGLContext *getOverlayContext(void);
    const wxGLContext *getNormalContext(void);
};


#endif //SOWX_SOWXGLWIDGETP_H
