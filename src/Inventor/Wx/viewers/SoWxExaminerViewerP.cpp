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

#include "Inventor/Wx/viewers/SoWxExaminerViewerP.h"
#include "Inventor/Wx/viewers/SoWxExaminerViewer.h"
#include "Inventor/Wx/common/pixmaps/ortho.xpm"
#include "Inventor/Wx/common/pixmaps/perspective.xpm"
#include "ButtonIndexValues.h"

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoWxExaminerViewerP::SoWxExaminerViewerP(SoWxExaminerViewer *publ)
        : SoGuiExaminerViewerP(publ) {

}

SoWxExaminerViewerP::~SoWxExaminerViewerP() {
    this->genericDestructor();
}

void
SoWxExaminerViewerP::constructor(const SbBool build) {
    this->genericConstructor();

    this->cameratogglebutton = NULL;

    this->orthopixmap = wxImage((const char **) ortho_xpm);
    this->perspectivepixmap = wxImage((const char **) perspective_xpm);
    assert(this->orthopixmap.GetSize() == this->perspectivepixmap.GetSize());

    PUBLIC(this)->setClassName("SoWxExaminerViewer");
    PUBLIC(this)->setPopupMenuString("Examiner Viewer");
    PUBLIC(this)->setLeftWheelString("RotX");
    PUBLIC(this)->setBottomWheelString("RotY");

    if (build) {
        wxWindow *widget = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
        widget->Bind( wxEVT_BUTTON,
                      &SoWxExaminerViewerP::cameratoggleClicked,
                      this,
                      CAMERA_BUTTON);
        PUBLIC(this)->setBaseWidget(widget);
        widget->SetMinSize(wxSize(500,300));
        PUBLIC(this)->setSize(SbVec2s(500, 300));
    }
}

void
SoWxExaminerViewerP::cameratoggleClicked(wxCommandEvent&) {
    if (PUBLIC(this)->getCamera())
        PUBLIC(this)->toggleCameraType();
}


