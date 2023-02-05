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
#ifndef SOWX_SOWXFULLVIEWERP_H
#define SOWX_SOWXFULLVIEWERP_H

#ifndef SOWX_INTERNAL
#error this is a private header file
#endif

#include "Inventor/Wx/viewers/SoGuiFullViewerP.h"
#include "Inventor/Wx/viewers/SoWxFullViewer.h"
#include <wx/tglbtn.h>
#include <string>
#include <map>

class SoWxFullViewerP : public SoGuiFullViewerP, public wxEvtHandler {
public:

    explicit SoWxFullViewerP(SoWxFullViewer *pViewer);
    ~SoWxFullViewerP();

    std::string popupmenutitle;
    wxWindow * viewerwidget;
    wxWindow * canvas;
    wxToggleButton * interactbutton;
    wxToggleButton * viewbutton;
    SbBool decorations;
    SbString menutitle;
    SbBool menuenabled;
    wxSizer* mainlayout;
    wxBoxSizer * appbuttonlayout;
    wxWindow* appbuttonform;
    SbPList * appbuttonlist;
    SbPList * viewerbuttons;

    typedef void(SoWxFullViewer:: *VoidFuncNoPar)() ;
    typedef void(SoWxFullViewer:: *VoidFuncOnePar)(float) ;
    struct WheelFunctions {
        WheelFunctions() {

        }
        WheelFunctions(VoidFuncNoPar o_p, VoidFuncNoPar o_r, VoidFuncOnePar o_m)
        :onPress(o_p)
        ,onRelease(o_r)
        ,onMove(o_m) {

        }

        VoidFuncNoPar onPress;
        VoidFuncNoPar onRelease;
        VoidFuncOnePar onMove;
    };

    typedef std::map<wxWindow*, WheelFunctions> MapEvent;
    MapEvent objectMap;
    void initThumbWheelEventMap();

    void setLeftWheelValue(const float value);
    static void setThumbWheelValue(wxWindow*, float value);
    void showDecorationWidgets(SbBool onOff);

    // Thumbwheels.
    void wheelPressed(wxCommandEvent&);
    void wheelReleased(wxCommandEvent&);
    void wheelMoved(wxCommandEvent&);


    // Button row.
    void interactbuttonClicked(wxCommandEvent & );
    void viewbuttonClicked(wxCommandEvent & );
    void homebuttonClicked(wxCommandEvent & );
    void sethomebuttonClicked(wxCommandEvent & );
    void viewallbuttonClicked(wxCommandEvent & );
    void seekbuttonClicked(wxCommandEvent & );
    void seekbuttonClicked();

    // Menu items.
    void selectedViewing();
    void selectedDecoration();
    void selectedHeadlight();

    // Generic slots.
    void increaseInteractiveCount();
    void decreaseInteractiveCount();

    void bindEvents(wxWindow*);

    // Return pointer to pushbutton in right-side decoration bar.
    wxAnyButton * getViewerbutton(const int idx) {
        return (wxAnyButton *)this->viewerbuttons->get(idx);
    }

wxDECLARE_EVENT_TABLE();
};


#endif //SOWX_SOWXFULLVIEWERP_H
