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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <Inventor/Wx/devices/SoWxMouse.h>
#include "Inventor/Wx/devices/SoGuiMouseP.h"
#include "sowxdefs.h"

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

class SoWxMouseP : public SoGuiMouseP {
public:
    SoWxMouseP(SoWxMouse * p) : SoGuiMouseP(p) { }

    //SoLocation2Event * makeLocationEvent(XMotionEvent * event);
    //SoMouseButtonEvent * makeButtonEvent(XButtonEvent * event, SoButtonEvent::State state);
};

SoWxMouse::SoWxMouse(int mask ) {
    PRIVATE(this) = new SoWxMouseP(this);
    PRIVATE(this)->eventmask = mask;
}

SoWxMouse::~SoWxMouse(void) {
    delete PRIVATE(this);
}

void SoWxMouse::enable(wxWindow* widget, SoWxEventHandler * handler, void * closure)    {
    // Nothing to do, already managed by callback in GLArea
}

void SoWxMouse::disable(wxWindow* widget, SoWxEventHandler * handler, void * closure) {
    // Nothing to do, already managed by callback in GLArea
}

const SoEvent * SoWxMouse::translateEvent(wxEvent& event) {

    SoEvent * conv = NULL;

    wxMouseEvent* mouse_event = dynamic_cast<wxMouseEvent*>(&event);
    if(!mouse_event) {
#if SOWX_DEBUG && 0
        SoDebugError::postWarning("SoWxMouse::translateEvent",
                                  "is not a mouse event!");
#endif
        return (conv);
    }
     bool isWheelInverted = false;
     //TODO: mouse_event->IsWheelInverted()
    // Convert wheel mouse events to Coin SoMouseButtonEvents.
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
    if (mouse_event->GetWheelRotation() > 0)
        PRIVATE(this)->buttonevent->setButton(isWheelInverted ?
                                              SoMouseButtonEvent::BUTTON5 :
                                              SoMouseButtonEvent::BUTTON4);
    else if (mouse_event->GetWheelRotation() < 0)
        PRIVATE(this)->buttonevent->setButton(isWheelInverted ?
                                              SoMouseButtonEvent::BUTTON4 :
                                              SoMouseButtonEvent::BUTTON5);

    PRIVATE(this)->buttonevent->setState(SoButtonEvent::DOWN);
    conv = PRIVATE(this)->buttonevent;

#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5

// Check for mousebutton press/release. Note that mousebutton
// doubleclick events are handled by converting them to two
// press/release events. In other words: it's the user's
// responsibility to translate pairs of singleclicks to
// doubleclicks, if doubleclicks have a special meaning in the
// application.

    if (((mouse_event->ButtonDClick()) ||
         (mouse_event->ButtonDown()) ||
         (mouse_event->ButtonUp())) &&
        (PRIVATE(this)->eventmask & (BUTTON_PRESS | BUTTON_RELEASE))) {

        // Which button?
        switch (mouse_event->GetButton()) {
            case wxMOUSE_BTN_LEFT:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON1);
                break;
            case wxMOUSE_BTN_RIGHT:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON2);
                break;
            case wxMOUSE_BTN_MIDDLE:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON3);
                break;
            // Not sure if this can actually happen.
            case wxMOUSE_BTN_ANY:
            default:
                PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::ANY);
                break;
        }

        // Press or release?
        if (mouse_event->ButtonUp())
            PRIVATE(this)->buttonevent->setState(SoButtonEvent::UP);
        else
            PRIVATE(this)->buttonevent->setState(SoButtonEvent::DOWN);

        conv = PRIVATE(this)->buttonevent;
    }

    // Check for mouse movement.
    if (mouse_event->Dragging() || mouse_event->Moving()) {
        conv = PRIVATE(this)->locationevent;
    }

    // Common settings for SoEvent superclass.
    if (conv) {
        conv->setShiftDown(mouse_event->ShiftDown());
        conv->setCtrlDown(mouse_event->ControlDown());
        conv->setAltDown(mouse_event->AltDown());
        this->setEventPosition(conv,
                               mouse_event->GetX(),
                               mouse_event->GetY());
        conv->setTime(SbTime::getTimeOfDay());
    }

    return (conv);
}
