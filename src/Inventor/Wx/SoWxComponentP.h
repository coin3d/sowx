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
#ifndef SOWX_SOWXCOMPONENTP_H
#define SOWX_SOWXCOMPONENTP_H

#include "Inventor/Wx/SoGuiComponentP.h"
#include "Inventor/Wx/SoWxComponent.h"
#include <Inventor/Wx/SoWxCursor.h>
#include <wx/window.h>
#include <wx/frame.h>

class SoWxComponentP : public SoGuiComponentP {
public:
    SoWxComponentP(SoWxComponent * publ);
    ~SoWxComponentP();

    static void fatalerrorHandler(void * userdata);
    static void atexit_cleanup();

    static class wxCursor * getNativeCursor(const SoWxCursor::CustomCursor * cc);

    // Variables.

    wxWindow * parent;
    wxWindow * widget;
    SbBool embedded, shelled;
    std::string classname, widgetname;
    SoWxComponentCB * closeCB;
    void * closeCBdata;
    SbPList * visibilitychangeCBs;
    SbBool realized;
    SbVec2s storesize;
    SbVec2s size;

    SbBool fullscreen;

    // virtual bool eventFilter(QObject * obj, QEvent * e);

public:
    void widgetClosed(void);

private:
    static SbDict * cursordict;
};


#endif //SOWX_SOWXCOMPONENTP_H
