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

#include "Inventor/Wx/viewers/SoWxPlaneViewer.h"
#include "Inventor/Wx/viewers/SoWxPlaneViewerP.h"
#include "Inventor/Wx/common/pixmaps/ortho.xpm"
#include "Inventor/Wx/common/pixmaps/perspective.xpm"
#include "Inventor/Wx/common/pixmaps/x.xpm"
#include "Inventor/Wx/common/pixmaps/y.xpm"
#include "Inventor/Wx/common/pixmaps/z.xpm"
#include "Inventor/nodes/SoOrthographicCamera.h"
#include "Inventor/nodes/SoPerspectiveCamera.h"

#include "sowxdefs.h"
#include "ButtonIndexValues.h"

#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

wxWindow*
SoWxPlaneViewer::buildWidget(wxWindow* parent) {
    return inherited::buildWidget(parent);
}

void
SoWxPlaneViewer::createViewerButtons(wxWindow* parent,
                                     SbPList * buttons) {
    inherited::createViewerButtons(parent, buttons);

    // add X, Y, Z viewpoint buttons
    PRIVATE(this)->buttons.x = new wxButton(parent, X_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;
    PRIVATE(this)->buttons.x->SetBitmap(wxImage(x_xpm));
    buttons->append(PRIVATE(this)->buttons.x);

    PRIVATE(this)->buttons.y = new wxButton(parent, Y_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;
    PRIVATE(this)->buttons.y->SetBitmap(wxImage(y_xpm));
    buttons->append(PRIVATE(this)->buttons.y);

    PRIVATE(this)->buttons.z =  new wxButton(parent, Z_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;
    PRIVATE(this)->buttons.z->SetBitmap(wxImage(z_xpm));
    buttons->append(PRIVATE(this)->buttons.z);

    // add camera toggle button
    assert(PRIVATE(this)->pixmaps.perspective != NULL);
    assert(PRIVATE(this)->pixmaps.orthogonal != NULL);
    PRIVATE(this)->buttons.camera = new wxButton(parent, CAMERA_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);;

    wxImage * pixmap = NULL;
    SoType t = this->getCameraType();
    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
        pixmap = PRIVATE(this)->pixmaps.orthogonal;
    else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        pixmap = PRIVATE(this)->pixmaps.perspective;
    else assert(0 && "unsupported cameratype");

    PRIVATE(this)->buttons.camera->SetBitmap( wxBitmap(*pixmap));
    buttons->append(PRIVATE(this)->buttons.camera);
}

void
SoWxPlaneViewer::setCamera( SoCamera* camera) {
    if (camera) {
        SoType type = camera->getTypeId();
        SbBool orthogonal =
                type.isDerivedFrom(SoOrthographicCamera::getClassTypeId());
        const char * oldLabel = this->getRightWheelString();
        if (oldLabel) {
            if (orthogonal) {
                if (strcmp("Dolly",oldLabel) == 0)
                    this->setRightWheelString("Zoom");
            }
            else if (strcmp("Zoom",oldLabel) == 0)
                this->setRightWheelString("Dolly");
        }
        if (PRIVATE(this)->buttons.camera) {
            PRIVATE(this)->buttons.camera->SetBitmap(
                    orthogonal ?
                    *(PRIVATE(this)->pixmaps.orthogonal) :
                    *(PRIVATE(this)->pixmaps.perspective));
        }
    }
    inherited::setCamera(camera);
}


SoWxPlaneViewerP::SoWxPlaneViewerP(SoWxPlaneViewer *v)
        : SoGuiPlaneViewerP(v){

}

void SoWxPlaneViewerP::constructor(SbBool buildnow) {
    this->commonConstructor(); // init generic stuff

    this->pixmaps.orthogonal = new wxImage((const char **) ortho_xpm);
    this->pixmaps.perspective = new wxImage((const char **) perspective_xpm);

    PUBLIC(this)->setClassName("SoWxPlaneViewer");
    PUBLIC(this)->setLeftWheelString("transY");
    PUBLIC(this)->setBottomWheelString("transX");

    if (!buildnow) return;

    PUBLIC(this)->setSize(SbVec2s(550, 490)); // extra buttons -> more height
    wxWindow * viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
    PUBLIC(this)->setBaseWidget(viewer);

    viewer->Bind(wxEVT_BUTTON,
                 &SoWxPlaneViewerP::cameraToggleClicked,
                 this,
                 CAMERA_BUTTON);
    viewer->Bind(wxEVT_BUTTON,
                 &SoWxPlaneViewerP::xClicked,
                 this,
                 X_BUTTON);
    viewer->Bind(wxEVT_BUTTON,
                 &SoWxPlaneViewerP::yClicked,
                 this,
                 Y_BUTTON);
    viewer->Bind(wxEVT_BUTTON,
                 &SoWxPlaneViewerP::zClicked,
                 this,
                 Z_BUTTON);
}

void SoWxPlaneViewerP::xClicked(wxEvent &) {
    this->viewPlaneX();
}

void SoWxPlaneViewerP::yClicked(wxEvent &) {
    this->viewPlaneY();
}

void SoWxPlaneViewerP::zClicked(wxEvent &) {
    this->viewPlaneZ();
}

void SoWxPlaneViewerP::cameraToggleClicked(wxEvent &) {
    PUBLIC(this)->toggleCameraType();
}

#undef PUBLIC
#undef PRIVATE
