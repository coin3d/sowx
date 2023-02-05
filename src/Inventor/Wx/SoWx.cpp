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

#include "Inventor/Wx/SoWx.h"
#include "Inventor/Wx/SoWxP.h"
#include <wx/event.h>
#include "Inventor/Wx/SoWxInternal.h"
#include "sowxdefs.h"

wxWindow*
SoWx::init(int & argc,
           char ** argv,
           const char * appname,
           const char * classname) {

    // retrieve the instance
    SoWxP::instance();

    if (SOWX_DEBUG && SoWxP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoWx::init",
                                  "This method should be called only once.");
        return SoWxP::instance()->getMainFrame();
    }

    // Call all the code for initializing Coin data
    SoWxP::commonInit();

    // if wxApp is not already created
    if (wxApp::GetInstance() == NULL) {
        // Set up the QApplication instance which we have derived into a
        // subclass to catch spaceball events.
        SoWxP::instance()->buildWxApp();
        wxApp::SetInstance(SoWxP::instance()->main_app);
        wxEntryStart( argc, argv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        // so nothing to do
        SoWxP::instance()->setWxApp(wxApp::GetInstance());
    }

    SoWxP::instance()->setMainFrame( new wxFrame(0,
                                                   wxID_ANY,
                                                   appname));

    assert(SoWxP::instance());
    assert(wxTheApp);
    wxTheApp->Bind(wxEVT_IDLE, &SoWxP::onIdle,  SoWxP::instance());
    SoWxP::instance()->getMainFrame()->Bind(wxEVT_CLOSE_WINDOW, &SoWxP::onClose,  SoWxP::instance());

    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                 NULL);

    SoWxP::instance()->setInitialize(true);
#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoWx::init",
                           "%s",
                           dumpWindowData(SoWxP::instance()->getMainFrame()).c_str());
#endif
    return (SoWxP::instance()->getMainFrame());
}

void
SoWx::init(wxWindow* toplevelwidget) {
#ifdef COIN_IV_EXTENSIONS
#define COIN_IV_EXTENSION(ext) ext::initClass();
    COIN_IV_EXTENSIONS
#undef COIN_IV_EXTENSION
#endif

    if (SOWX_DEBUG && SoWxP::instance()->isInitialized()) {
        SoDebugError::postWarning("SoWx::init",
                                  "This method should be called only once.");
        return;
    }

    // Call all the code for initializing Coin data
    SoWxP::commonInit();

    // if wxApp is not already created
    if (wxApp::GetInstance() == NULL) {
        SoWxP::instance()->buildWxApp();
        wxApp::SetInstance(SoWxP::instance()->main_app);
        static const char * dummyargv[1];
        dummyargv[0] = "SoWx";
        int argc = 1;
        wxEntryStart( argc, (char **)dummyargv );
        wxTheApp->CallOnInit();
    }
    else {
        // The user already set one up for us.
        // so nothing to do
        SoWxP::instance()->setWxApp(wxApp::GetInstance());
    }

    wxTheApp->Bind(wxEVT_IDLE, &SoWxP::onIdle,  SoWxP::instance());
    if(toplevelwidget) {
        SoWxP::instance()->setMainFrame(toplevelwidget);
        SoWxP::instance()->getMainFrame()->Bind(wxEVT_CLOSE_WINDOW, &SoWxP::onClose, SoWxP::instance());
    }
    SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged,
                                                 NULL);

    SoWxP::instance()->setInitialize(true);
#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoWx::init",
                           "%s",
                           dumpWindowData(SoWxP::instance()->getMainFrame()).c_str());
#endif
}

/**
 * mainLoop is required only if an external app is not available
 * (embedded in already existing window).
 */
void
SoWx::mainLoop(void) {
    wxTheApp->OnRun();
}


void
SoWx::exitMainLoop(void)  {
    wxExit();
}

void
SoWx::done() {
    SoWxP::instance()->finish();
#if 0 // FIXME: These methods exist in TGS Inventor. We should add
    // them, and then call them from here. 20060210 kyrah
  SoInteraction::finish();
  SoNodeKit::finish();
#endif
    SoDB::finish();
}

void
SoWx::show(wxWindow* const widget) {
    widget->Show();
}

void
SoWx::hide(wxWindow* const widget) {
    widget->Hide();
}

void
SoWx::createSimpleErrorDialog(wxWindow* widget,
                              const char * title,
                              const char * string1,
                              const char * string2 ) {
    SOWX_STUB();
}

wxWindow*
getTopLevelWidget(void) {
    return (wxTheApp->GetTopWindow());
}

wxWindow*
SoWx::getShellWidget(const wxWindow* w) {
#if 0
    return (wxGetTopLevelParent((wxWindowBase *) w));
#else
    wxWindow* p = const_cast<wxWindow*>(w);
    while (p !=  NULL) {
        wxFrame* top_frame = dynamic_cast<wxFrame*>(p);
        if ( top_frame ) {
            return p;
        }
        p = p->GetParent();
    }
#if SOWX_DEBUG && 0 // debug
    SoDebugError::postInfo("SoWx::getShellWidget",
                         "couldn't find shell for widget at %p", widget);
#endif // debug
    return (NULL);
#endif
}

void
SoWx::setWidgetSize(wxWindow* const widget, const SbVec2s size) {
    assert(widget != 0 && "widget can not be null");
    if ( widget ) {
        widget->SetSize(size[0], size[1]);
    }
#if SOWX_DEBUG
    else  {
        SoDebugError::postWarning("SoWx::setWidgetSize",
                                  "null widget on setting: <%d, %d>.",
                                  size[0], size[1]);
    }
#endif // SOWX_DEBUG
}


SbVec2s
SoWx::getWidgetSize(const wxWindow* widget) {
    assert(widget != 0 && "widget can not be null");
    SbVec2s size(-1,-1);
    if ( widget ) {
        wxSize wx_size = widget->GetSize();
        size[0] = wx_size.GetWidth();
        size[1] = wx_size.GetHeight();
    }
#if SOWX_DEBUG
    else  {
        SoDebugError::postWarning("SoWx::getWidgetSize",
                                  "null widget");
    }
#endif // SOWX_DEBUG
}
