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

#include "Inventor/Wx/SoWxComponent.h"
#include "Inventor/Wx/SoWxComponentP.h"
#include "Inventor/Wx/SoAny.h"
#include "Inventor/Wx/SoWxGLWidget.h"
#include "Inventor/Wx/SoWxRenderArea.h"
#include "Inventor/Wx/SoWxCursor.h"

#include "Inventor/Wx/viewers/SoWxViewer.h"
#include "Inventor/Wx/viewers/SoWxFullViewer.h"
#include "Inventor/Wx/viewers/SoWxExaminerViewer.h"
#include "Inventor/Wx/viewers/SoWxPlaneViewer.h"
#include "Inventor/Wx/viewers/SoWxConstrainedViewer.h"
#include "Inventor/Wx/viewers/SoWxFlyViewer.h"

#include "Inventor/Wx/SoWxInternal.h"

#include "sowxdefs.h"
#include "SoWxP.h"

#define SOWXCOMP_RESIZE_DEBUG 1

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SOWX_OBJECT_ABSTRACT_SOURCE(SoWxComponent);

SoWxComponent::~SoWxComponent(){

}

SoWxComponent::SoWxComponent(wxWindow* const parent,
                             const char * const name,
                             const SbBool embed) {
    PRIVATE(this) = new SoWxComponentP(this);

    PRIVATE(this)->realized = false;
    PRIVATE(this)->shelled = false;
    PRIVATE(this)->widget = NULL;
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->closeCB = NULL;
    PRIVATE(this)->closeCBdata = NULL;
    PRIVATE(this)->visibilitychangeCBs = NULL;
    PRIVATE(this)->fullscreen = false;

    this->setClassName("SoWxComponent");

    PRIVATE(this)->storesize.setValue(-1, -1);

    SoAny::si()->addInternalFatalErrorHandler(SoWxComponentP::fatalerrorHandler,
                                              PRIVATE(this));

    PRIVATE(this)->widgetname = (name ? name :
                                 this->getDefaultWidgetName());

    if (!parent || !embed) {
        PRIVATE(this)->parent = new wxFrame(NULL, wxID_ANY, name);
        PRIVATE(this)->embedded = false;
        PRIVATE(this)->shelled = true;
    }
    else {
        PRIVATE(this)->parent = parent;
        PRIVATE(this)->embedded = true;
    }

    // TODO: PRIVATE(this)->parent->installEventFilter(PRIVATE(this));
}

void  SoWxComponent::initClasses(void) {
    SoWxComponent::initClass();
    SoWxGLWidget::initClass();
    SoWxRenderArea::initClass();
    SoWxViewer::initClass();
    SoWxExaminerViewer::initClass();
    SoWxPlaneViewer::initClass();
    SoWxConstrainedViewer::initClass();
    SoWxFullViewer::initClass();
    SoWxFlyViewer::initClass();
}

void
SoWxComponent::afterRealizeHook(void) {
    SOWX_STUB();
}

void
SoWxComponent::setClassName(const char * const name) {
    PRIVATE(this)->classname = name;
}

void
SoWxComponent::setBaseWidget(wxWindow* w) {

#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoWx::setBaseWidget",
                           "%s",
                           dumpWindowData(w).c_str());
#endif

    std::string iconText = this->getDefaultIconTitle();
    std::string widgetName = PRIVATE(this)->widgetname;

    assert(w);

    if (PRIVATE(this)->widget) {
        /* TODO: iconText = (PRIVATE(this)->widget-windowIconText().isEmpty() ?
		iconText :
		PRIVATE(this)->widget->windowIconText());
         */
        widgetName = (PRIVATE(this)->widget->GetName().IsEmpty() ?
                      widgetName :
                      PRIVATE(this)->widget->GetName());

        this->unregisterWidget(PRIVATE(this)->widget);
    }

    PRIVATE(this)->widget = w;

    this->registerWidget(PRIVATE(this)->widget);

#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxComponent::setBaseWidget",
                           "widget: %p, parent: %p", w, PRIVATE(this)->parent);
#endif

    if (!PRIVATE(this)->parent || PRIVATE(this)->parent->IsTopLevel()) {

        if (PRIVATE(this)->widget->GetName() == "") {
            this->setTitle(this->getDefaultTitle());
        }

        SoWx::getShellWidget(this->getWidget())->SetName(iconText);
    }
    PRIVATE(this)->widget->SetName(widgetName);
}

void
SoWxComponent::show(void) {
    if(SOWX_DEBUG && !PRIVATE(this)->widget) { // debug
        SoDebugError::postWarning("SoWxComponent::show",
                                  "Called while no wxWindow has been set.");
        return;
    }

    if (SOWXCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWxComponent::show-1",
                               "resizing %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->storesize[0],
                               PRIVATE(this)->storesize[1]);
    }

    // only parent has power for setting the correct size
    PRIVATE(this)->parent->SetClientSize(PRIVATE(this)->storesize[0],
                                         PRIVATE(this)->storesize[1]);

    if (SOWXCOMP_RESIZE_DEBUG) {  // debug
        SoDebugError::postInfo("SoWxComponent::show-2",
                               "resized %p: (%d, %d)",
                               PRIVATE(this)->widget,
                               PRIVATE(this)->widget->GetSize().GetX(),
                               PRIVATE(this)->widget->GetSize().GetY());
    }

    if(! PRIVATE(this)->embedded && PRIVATE(this)->shelled) {
#if SOWX_DEBUG
        SoDebugError::postInfo("SoWxComponent::show",
                               "perform show if is not embedded and is shelled");
        PRIVATE(this)->parent->Show();
#endif
    }
    PRIVATE(this)->widget->Show();

    this->sizeChanged(PRIVATE(this)->storesize);
}

void
SoWxComponent::hide(void) {
    SOWX_STUB();
    PRIVATE(this)->widget->Hide();
}

void
SoWxComponent::setComponentCursor(const SoWxCursor & cursor) {
    SOWX_STUB();
    SoWxComponent::setWidgetCursor(this->getWidget(), cursor);
}

void
SoWxComponent::setWidgetCursor(wxWindow* w, const SoWxCursor & cursor) {

    if(!w) {
        return;
    }
    // FIXME: as this function is called all the time when the cursor is
    // grabbed by the window under X11, we should really compare with
    // the previous cursor before doing anything, to avoid spending
    // unnecessary clockcycles during animation. 20011203 mortene.

    if (cursor.getShape() == SoWxCursor::CUSTOM_BITMAP) {
        const SoWxCursor::CustomCursor * cc = &cursor.getCustomCursor();
        w->SetCursor(*SoWxComponentP::getNativeCursor(cc));
    }
    else {
        switch (cursor.getShape()) {
            case SoWxCursor::DEFAULT:
                w->SetCursor(*wxSTANDARD_CURSOR);
                break;

            case SoWxCursor::BUSY:
                w->SetCursor(*wxHOURGLASS_CURSOR);
                break;

            case SoWxCursor::CROSSHAIR:
                w->SetCursor(*wxCROSS_CURSOR);
                break;

            case SoWxCursor::UPARROW:
                w->SetCursor(*wxCROSS_CURSOR);
                break;

            default:
                assert(false && "unsupported cursor shape type");
                break;
        }
    }
}

SbBool
SoWxComponent::isFullScreen(void) const {
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoWxComponent::setFullScreen(const SbBool onoff) {
    wxWindow * w = this->getShellWidget();
    if (w == NULL) w = this->getParentWidget();
    if (w == NULL) w = this->getWidget();
    if (!w) { return false; }
    wxFrame* frame =  dynamic_cast<wxFrame*>(w);
    if(frame) {
        frame->ShowFullScreen(onoff);
    }
    else {
        return (false);
    }

    PRIVATE(this)->fullscreen = onoff;
    return (PRIVATE(this)->fullscreen);
}

SbBool
SoWxComponent::isVisible(void) {
    bool ret = false;
    if( PRIVATE(this)->widget ) {
        ret = PRIVATE(this)->widget->IsShownOnScreen();
    }
    return (ret);
}

SbBool
SoWxComponent::isTopLevelShell(void) const {
#if SOWX_DEBUG
    if (! PRIVATE(this)->widget) {
        SoDebugError::postWarning("SoWxComponent::isTopLevelShell",
                                  "Called while no wxWindow has been set.");
        return false;
    }
#endif // SOWX_DEBUG
    return PRIVATE(this)->embedded ? false : true;
}

wxWindow*
SoWxComponent::getWidget(void) const {
    return this->getBaseWidget();
}

wxWindow*
SoWxComponent::getBaseWidget(void) const {
    return PRIVATE(this)->widget;
}

wxWindow*
SoWxComponent::getParentWidget(void) const {
    return PRIVATE(this)->parent;
}

void
SoWxComponent::setSize(const SbVec2s size) {

#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoWxComponent::setSize",
                           " baseWidget %s",
                           dumpWindowData(this->getBaseWidget()).c_str());
    SoDebugError::postInfo("SoWxComponent::setSize",
                           " shellWidget %s",
                           dumpWindowData(this->getShellWidget()).c_str());
#endif

#if 0
    if ( PRIVATE(this)->embedded ) {
        SoWx::setWidgetSize(this->getBaseWidget(), size);
    }
    else {
        SoWx::setWidgetSize(this->getShellWidget(), size);
    }
#endif
#if 1
#if SOWX_DEBUG
    if((size[0] <= 0) || (size[1] <= 0)) {
        SoDebugError::postWarning("SoWxComponent::setSize",
                                  "Invalid size setting: <%d, %d>.",
                                  size[0], size[1]);
        return;
    }
#endif // SOWX_DEBUG

#if SOWXCOMP_RESIZE_DEBUG  // debug
    SoDebugError::postInfo("SoWxComponent::setSize",
                           "resize %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           size[0], size[1]);
#endif // debug
    const SbBool yetbuilt = (this->getWidget() != NULL);
    if (yetbuilt) {
        wxWindow * shell = this->getShellWidget();
        if (shell) {
            // shell->SetSize(size[0], size[1]);
            shell->SetClientSize(wxSize(size[0], size[1]));
        }
    }
#endif
    PRIVATE(this)->storesize = size;
    this->sizeChanged(size);
}

SbVec2s
SoWxComponent::getSize(void) const {
    return (PRIVATE(this)->storesize);
}

void
SoWxComponent::setTitle(const char * const title) {
    SoWx::getShellWidget(this->getWidget())->SetName(title);
}

const char *
SoWxComponent::getTitle(void) const {
    return (SoWx::getShellWidget(this->getWidget())->GetName());
}

void
SoWxComponent::setIconTitle(const char * const title) {
    SOWX_STUB();
}

const char *
SoWxComponent::getIconTitle(void) const {
    SOWX_STUB();
    return ("");
}

const char *
SoWxComponent::getWidgetName(void) const{
    SOWX_STUB();
    return ("");
}

const char *
SoWxComponent::getClassName(void) const {
    SOWX_STUB();
    return ("");
}

void
SoWxComponent::setWindowCloseCallback(SoWxComponentCB * const func,
                                      void * const user ) {
    SOWX_STUB();
}

void
SoWxComponent::sizeChanged(const SbVec2s & size) {
    SOWX_STUB();
}

void
SoWxComponent::addVisibilityChangeCallback(SoWxComponentVisibilityCB * const func,
                                           void * const user) {
    SOWX_STUB();
}

void
SoWxComponent::removeVisibilityChangeCallback(SoWxComponentVisibilityCB * const func,
                                              void * const user){
    SOWX_STUB();
}

#undef PUBLIC
#undef PRIVATE
