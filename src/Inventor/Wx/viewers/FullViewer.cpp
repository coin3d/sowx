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

#include <Inventor/Wx/viewers/SoWxFullViewer.h>
#include "Inventor/Wx/viewers/SoWxFullViewerP.h"
#include "Inventor/Wx/widgets/SoWxThumbWheel.h"
#include <Inventor/Wx/widgets/SoWxPopupMenu.h>
#include "Inventor/Wx/SoWxInternal.h"
#include "sowxdefs.h"
#include "Inventor/Wx/SoWxP.h"
#include "ButtonIndexValues.h"

// Button icons.
#include <Inventor/Wx/common/pixmaps/pick.xpm>
#include <Inventor/Wx/common/pixmaps/view.xpm>
#include <Inventor/Wx/common/pixmaps/home.xpm>
#include <Inventor/Wx/common/pixmaps/set_home.xpm>
#include <Inventor/Wx/common/pixmaps/view_all.xpm>
#include <Inventor/Wx/common/pixmaps/seek.xpm>

#include <wx/stattext.h>
#include <wx/sizer.h>

#define PUBLIC(o) (o->pub)
#define PRIVATE(o) (o->pimpl)

SOWX_OBJECT_ABSTRACT_SOURCE(SoWxFullViewer);

const int XPM_BUTTON_SIZE = 24;

SoWxFullViewer::SoWxFullViewer(wxWindow* parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag buildFlag,
                               Type type,
                               SbBool build)
        : inherited(parent,
                    name,
                    embed,
                    type,
                    FALSE)
{
    PRIVATE(this) = new SoWxFullViewerP(this);

    PRIVATE(this)->viewerwidget = NULL;
    PRIVATE(this)->canvas = NULL;

    PRIVATE(this)->viewbutton = NULL;
    PRIVATE(this)->interactbutton = NULL;

    this->leftDecoration = NULL;
    this->bottomDecoration = NULL;
    this->rightDecoration = NULL;

    this->leftWheel = NULL;
    this->leftWheelLabel = NULL;
    this->leftWheelStr = NULL;
    this->leftWheelVal = 0.0f;

    this->bottomWheel = NULL;
    this->bottomWheelLabel = NULL;
    this->bottomWheelStr = NULL;
    this->bottomWheelVal = 0.0f;

    this->rightWheel = NULL;
    this->rightWheelLabel = NULL;
    this->rightWheelStr = NULL;
    this->rightWheelVal = 0.0f;

    this->setLeftWheelString("Motion X");
    this->setBottomWheelString("Motion Y");
    this->setRightWheelString("Dolly");

    PRIVATE(this)->mainlayout = NULL;
    PRIVATE(this)->appbuttonlayout = NULL;

    PRIVATE(this)->menuenabled = buildFlag & SoWxFullViewer::BUILD_POPUP;
    PRIVATE(this)->decorations = (buildFlag & SoWxFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

    this->prefmenu = NULL;
    PRIVATE(this)->menutitle = "Viewer Menu";

    PRIVATE(this)->viewerbuttons = new SbPList;
    PRIVATE(this)->appbuttonlist = new SbPList;
    PRIVATE(this)->appbuttonform = NULL;

    if (! build) return;

    this->setClassName("SoWxFullViewer");
    wxWindow * viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
}

SoWxFullViewer::~SoWxFullViewer() {
    delete PRIVATE(this)->viewerbuttons;
    delete PRIVATE(this)->appbuttonlist;
    delete [] this->rightWheelStr;
    delete [] this->leftWheelStr;
    delete [] this->bottomWheelStr;
    delete PRIVATE(this);
}

wxWindow*
SoWxFullViewer::buildWidget(wxWindow* parent) {
    // This will build the main view widgets, along with the decorations
    // widgets and popup menu if they are enabled.
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxFullViewer::buildWidget", "[invoked]");
    parent->SetName("MainWindow");
    SoDebugError::postInfo("SoWxFullViewer::buildWidget", "Step-1");
    dumpWindowData(parent);
#endif

    PRIVATE(this)->viewerwidget = parent;
    PRIVATE(this)->viewerwidget->SetName("viewer_widget");

    this->registerWidget(PRIVATE(this)->viewerwidget);

#if SOWX_DEBUG && 0
    PRIVATE(this)->viewerwidget->SetBackgroundColour(wxColour(125, 150, 190));
#endif

    PRIVATE(this)->canvas = inherited::buildWidget(PRIVATE(this)->viewerwidget);
    PRIVATE(this)->canvas->SetMinSize(wxSize(100,100));

#if SOWX_DEBUG && 0
    PRIVATE(this)->canvas->SetBackgroundColour(wxColour(250, 0, 255));
#endif

    this->buildDecoration( PRIVATE(this)->viewerwidget );
    PRIVATE(this)->showDecorationWidgets( PRIVATE(this)->decorations );

    if (PRIVATE(this)->menuenabled)
        this->buildPopupMenu();

#if SOWX_DEBUG && 0
        SoDebugError::postInfo("SoWxFullViewer::buildWidget", "Step-2");
    dumpWindowData(parent);
#endif

    PRIVATE(this)->bindEvents(PRIVATE(this)->viewerwidget);

    return PRIVATE(this)->viewerwidget;
}


void
SoWxFullViewer::setDecoration(const SbBool enable){
#if SOWX_DEBUG && 0
    if ((enable  && this->isDecoration()) ||
        (!enable && !this->isDecoration())) {
        SoDebugError::postWarning("SoWxFullViewer::setDecoration",
                                  "decorations already turned %s",
                                  enable ? "on" : "off");
        return;
    }
#endif

    PRIVATE(this)->decorations = enable;
    if (PRIVATE(this)->viewerwidget)
        PRIVATE(this)->showDecorationWidgets(enable);
}

SbBool
SoWxFullViewer::isDecoration(void) const{
    return (PRIVATE(this)->decorations);
}

void SoWxFullViewer::setPopupMenuEnabled(const SbBool enable){
#if SOWX_DEBUG
    if ((enable && this->isPopupMenuEnabled()) ||
        (!enable && !this->isPopupMenuEnabled())) {
        SoDebugError::postWarning("SoWxFullViewer::setPopupMenuEnabled",
                                  "popup menu already turned %s",
                                  enable ? "on" : "off");
        return;
    }
#endif
    PRIVATE(this)->menuenabled = enable;
}

SbBool
SoWxFullViewer::isPopupMenuEnabled(void) const{
    return (PRIVATE(this)->menuenabled);
}

wxWindow*
SoWxFullViewer::getAppPushButtonParent(void) const {
    SOWX_STUB();
    return (0);
}

void
SoWxFullViewer::addAppPushButton(wxWindow* newButton)  {
    SOWX_STUB();
}

void
SoWxFullViewer::insertAppPushButton(wxWindow* newButton, int index) {
    SOWX_STUB();
}

void
SoWxFullViewer::removeAppPushButton(wxWindow* oldButton) {
    SOWX_STUB();
}

int
SoWxFullViewer::findAppPushButton(wxWindow* oldButton) const {
    SOWX_STUB();
    return (0);
}

int
SoWxFullViewer::lengthAppPushButton(void) const {
    SOWX_STUB();
    return (0);
}

wxWindow*
SoWxFullViewer::getRenderAreaWidget(void) const {
    return (PRIVATE(this)->canvas);
}

void
SoWxFullViewer::setViewing(SbBool enable) {
    if ((enable && this->isViewing()) ||
        (!enable && !this->isViewing())) {
#if SOWX_DEBUG && 0
        SoDebugError::postWarning("SoWxFullViewer::setViewing",
                              "view mode already %s", enable ? "on" : "off");
#endif
        return;
    }

    inherited::setViewing(enable);

    // Must check that buttons have been built, in case this viewer
    // component was made without decorations.
    if (PRIVATE(this)->viewerbuttons->getLength() > 0) {
        ((wxToggleButton*)(PRIVATE(this))->getViewerbutton(EXAMINE_BUTTON))->SetValue(enable);
        ((wxToggleButton*)(PRIVATE(this))->getViewerbutton(INTERACT_BUTTON))->SetValue(enable ? FALSE : TRUE);
        ((wxButton*)PRIVATE(this)->getViewerbutton(SEEK_BUTTON))->Enable(enable);
    }
}


void
SoWxFullViewer::buildDecoration(wxWindow* parent) {
    this->leftDecoration = this->buildLeftTrim(parent);
#if SOWX_DEBUG && 0
    this->leftDecoration->SetBackgroundColour(wxColour(255, 0, 0));
#endif
    this->bottomDecoration = this->buildBottomTrim(parent);
#if SOWX_DEBUG && 0
    this->bottomDecoration->SetBackgroundColour(wxColour(0, 255, 0));
#endif
    this->rightDecoration = this->buildRightTrim(parent);
#if SOWX_DEBUG && 0
    this->rightDecoration->SetBackgroundColour(wxColour(0, 0, 255));
#endif

#if SOWX_DEBUG && 0
    dumpWindowData(this->leftDecoration);
    dumpWindowData(this->rightDecoration);
    dumpWindowData(this->bottomDecoration);
#endif

    PRIVATE(this)->initThumbWheelEventMap();
}

wxWindow*
SoWxFullViewer::buildLeftTrim(wxWindow* parent){
    wxPanel* p = new wxPanel(parent);
    p->SetName("leftTrim");
    p->SetMinSize(wxSize(24,100));

#if SOWX_DEBUG && 0
    p->SetBackgroundColour(wxColour(255, 0, 255));
#endif

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SoWxThumbWheel * t = new SoWxThumbWheel(SoWxThumbWheel::Vertical, p);
    t->SetName("left thumb wheel");
    t->setRangeBoundaryHandling(SoWxThumbWheel::ACCUMULATE);
    this->leftWheelVal = t->value();
    this->leftWheel = t;
    sizer->Add(0,0,1,0);
    sizer->Add(t, 0, wxALL |wxALIGN_CENTER_HORIZONTAL, 0);
    p->SetSizer(sizer);
    p->Fit();
#if SOWX_DEBUG && 0
    dumpWindowData(p);
#endif
    return p;
}

wxWindow*
SoWxFullViewer::buildBottomTrim(wxWindow* parent) {
    wxWindow * w = new wxPanel(parent);
    w->SetName("bottomTrim");
    w->SetMinSize(wxSize(100,24));
    wxStaticText* label = new wxStaticText( w, wxID_ANY, this->leftWheelStr);
#if SOWX_DEBUG && 0
    label->SetBackgroundColour(wxColour(200,200,0));
#endif
    label->SetName("left wheel label");
    this->leftWheelLabel = label;

    label = new wxStaticText( w, wxID_ANY, this->bottomWheelStr ,wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
#if SOWX_DEBUG && 0
    label->SetBackgroundColour(wxColour(100,100,0));
#endif
    label->SetName("bottom wheel label");
    this->bottomWheelLabel = label;

    label = new wxStaticText( w, wxID_ANY, this->rightWheelStr, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
#if SOWX_DEBUG && 0
    label->SetBackgroundColour(wxColour(100,50,0));
#endif
    label->SetName("right wheel label");
    this->rightWheelLabel = label;

    SoWxThumbWheel * t = new SoWxThumbWheel(SoWxThumbWheel::Horizontal, w);
    t->SetName("bottom thumb wheel");
#if SOWX_DEBUG && 0
    t->SetBackgroundColour(wxColour(0,0,0));
#endif

    this->bottomWheel = t;
    t->setRangeBoundaryHandling(SoWxThumbWheel::ACCUMULATE);

    this->bottomWheelVal = t->value();

    wxFlexGridSizer* layout = new wxFlexGridSizer( 0, 6, 0, 5 );

    layout->AddGrowableCol( 1 );
    layout->AddGrowableCol( 4 );
    layout->Add( this->leftWheelLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    layout->AddStretchSpacer();
    layout->Add( this->bottomWheelLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    layout->Add( this->bottomWheel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    layout->AddStretchSpacer();
    layout->Add( this->rightWheelLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    w->SetSizer( layout );
    w->Fit();
    return w;
}

wxWindow*
SoWxFullViewer::buildRightTrim(wxWindow* parent) {
    wxPanel* p = new wxPanel(parent);
    p->SetName("rightTrim");
#if SOWX_DEBUG && 0
    p->SetBackgroundColour(wxColour(255,0,0));
#endif
    p->SetMinSize(wxSize(XPM_BUTTON_SIZE+12, 100));
    p->SetMaxSize(wxSize(XPM_BUTTON_SIZE+12, -1));
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SoWxThumbWheel * t = new SoWxThumbWheel(SoWxThumbWheel::Vertical, p);
    t->SetName("right thumb wheel");
#if SOWX_DEBUG && 0
    t->SetBackgroundColour(wxColour(100,250,110));
#endif
    t->setRangeBoundaryHandling(SoWxThumbWheel::ACCUMULATE);
    this->rightWheelVal = t->value();
    this->rightWheel = t;
    const int border_size = 0;
    sizer->Add(this->buildViewerButtons(p),  1, wxALL | wxCENTER, border_size);
    sizer->Add( 0, 0, 1, wxEXPAND, border_size);
    sizer->Add(t, 1, wxALL| wxCENTER, border_size);
    p->SetSizer(sizer);
    p->Layout();
    return p;
}

wxWindow*
SoWxFullViewer::buildAppButtons(wxWindow* parent) {
    SOWX_STUB();
    return (0);
}

wxWindow*
SoWxFullViewer::buildViewerButtons(wxWindow* parent) {
    wxPanel * w = new wxPanel(parent);
    w->SetName("viewerButtons");
#if SOWX_DEBUG && 0
    w->SetBackgroundColour(wxColour(250,100,250));
#endif
    this->createViewerButtons(w, PRIVATE(this)->viewerbuttons);

    const int numViewerButtons = PRIVATE(this)->viewerbuttons->getLength();
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    for (int i = 0; i < numViewerButtons; i++) {
        wxAnyButton * b = PRIVATE(this)->getViewerbutton(i);
        sizer->Add( b, 0, wxALL | wxCENTER, 0);
    }
    w->SetSizer(sizer);
    return (w);
}

void
SoWxFullViewer::createViewerButtons(wxWindow* parent,
                                    SbPList * button_list) {
    for (int i=0; i <= SEEK_BUTTON; i++) {
        wxAnyButton *p = new wxButton(parent, i);

        switch (i) {
            case INTERACT_BUTTON: {
                parent->RemoveChild(p);
                delete p;
                PRIVATE(this)->interactbutton = new wxToggleButton(parent, i, wxEmptyString);
                p = PRIVATE(this)->interactbutton;
                p->SetBitmap(wxImage(pick_xpm));
                p->SetName("INTERACT");
                PRIVATE(this)->interactbutton->SetValue(this->isViewing() ? FALSE : TRUE);
            }

                break;
            case EXAMINE_BUTTON: {
                parent->RemoveChild(p);
                delete p;
                PRIVATE(this)->viewbutton = new wxToggleButton(parent, i, wxEmptyString);
                p = PRIVATE(this)->viewbutton;
                PRIVATE(this)->viewbutton->SetValue(this->isViewing());
                p->SetName("EXAMINE");
                p->SetBitmap(wxImage(view_xpm));
            }
                break;
            case HOME_BUTTON: {
                p->SetBitmap(wxImage(home_xpm));
                p->SetName("HOME");
            }
                break;
            case SET_HOME_BUTTON: {
                p->SetBitmap(wxImage(set_home_xpm));
                p->SetName("SET_HOME");
            }
                break;
            case VIEW_ALL_BUTTON: {
                p->SetBitmap(wxImage (view_all_xpm));
                p->SetName("VIEW_ALL");
            }
                break;
            case SEEK_BUTTON: {
                p->SetBitmap(wxImage(seek_xpm));
                p->SetName("SEEK");
            }
                break;
            default:
                assert(0);
                break;
        }

        p->SetWindowStyle( wxBU_EXACTFIT | wxBU_NOTEXT);
        p->SetSize(XPM_BUTTON_SIZE, XPM_BUTTON_SIZE);
        button_list->append(p);
    }
}

void
SoWxFullViewer::buildPopupMenu(void) {
    this->prefmenu = PRIVATE(this)->setupStandardPopupMenu();
}

void
SoWxFullViewer::openPopupMenu(const SbVec2s position) {
    if (! this->isPopupMenuEnabled()) return;
    if (this->prefmenu == NULL)
        this->buildPopupMenu();
    int x = 2 + position[0];
    int y = 2 + this->getGLSize()[1] - position[1] - 1;

    PRIVATE(this)->prepareMenu(this->prefmenu);
    this->prefmenu->popUp(this->getGLWidget(), x, y);
}

void
initString(char* &destination,
           const char* data) {
    delete [] destination;
    destination = 0;
    if (data) {
        int length = strlen(data) + 1;
        destination = strncpy(new char[length], data, length);
    }
}

void
SoWxFullViewer::setLeftWheelString(const char * const name) {
    initString(this->leftWheelStr, name);

    if (this->leftWheelLabel)
        this->leftWheelLabel->SetLabel(name ? name : "");
}

void
SoWxFullViewer::setBottomWheelString(const char * const name) {
    initString(this->bottomWheelStr, name);

    if (this->bottomWheelLabel)
        this->leftWheelLabel->SetLabel(name ? name : "");

}

void
SoWxFullViewer::setRightWheelString(const char * const name) {
    initString(this->rightWheelStr, name);

    if (this->rightWheelLabel) {
        this->rightWheelLabel->SetLabel(name ? name : "");
    }
}

void
SoWxFullViewer::sizeChanged(const SbVec2s & size) {
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxFullViewer::sizeChanged", "(%d, %d)",
                         size[0], size[1]);
#endif

    SbVec2s new_size(size);
    // wxWidgets already has only size of gl area
    // decorations size do not need to be removed
    /*
     if (PRIVATE(this)->decorations) {
        new_size[0] -= width(this->leftDecoration);
        new_size[0] -= width(this->rightDecoration);
        new_size[1] -= height(this->bottomDecoration);
    }
    new_size = SbVec2s(SoWxMax(new_size[0], (short)1),
                      SoWxMax(new_size[1], (short)1));
*/

    inherited::sizeChanged(new_size);
}

const char *
SoWxFullViewer::getRightWheelString() const {
    return (this->rightWheelStr);
}

#undef PUBLIC
#undef PRIVATE
