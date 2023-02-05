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

#include <Inventor/SbTime.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include "Inventor/Wx/devices/SoWxKeyboard.h"
#include "SoWxKeyboardP.h"
#include "sowxdefs.h"

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************


SoWxKeyboard::SoWxKeyboard(int eventmask ) {
    PRIVATE(this) = new SoWxKeyboardP;
    PRIVATE(this)->eventmask = eventmask;
}

SoWxKeyboard::~SoWxKeyboard(void) {
    delete PRIVATE(this);
}

void
SoWxKeyboard::enable(wxWindow* widget, SoWxEventHandler * handler, void * closure) {
    SOWX_STUB();
}

void
SoWxKeyboard::disable(wxWindow* widget, SoWxEventHandler * handler, void * closure) {
    SOWX_STUB();
}

const SoEvent *
SoWxKeyboard::translateEvent(wxEvent& event) {

    wxKeyEvent* key_event = dynamic_cast<wxKeyEvent*>(&event);

    if(!key_event) {
#ifdef SOWX_DEBUG
        SoDebugError::postWarning("SoWxKeyboard::translateEvent",
                                  "is not a key event!");
#endif
        return (0);
    }

    SbBool keypress = key_event->GetEventType() == wxEVT_KEY_DOWN;
    SbBool keyrelease = key_event->GetEventType() == wxEVT_KEY_UP;

    SbBool keyevent = keypress || keyrelease;

    if (keyevent && (PRIVATE(this)->eventmask & (KEY_PRESS | KEY_RELEASE))) {

        if (!SoWxKeyboardP::translatetable)
            SoWxKeyboardP::make_translation_table();

        int key = key_event->GetKeyCode();
        // Key code / sequence unknown.
        if (key == 0) return NULL;

        // Allocate system-neutral event object once and reuse.
        if (!PRIVATE(this)->kbdevent) PRIVATE(this)->kbdevent = new SoKeyboardEvent;

        PRIVATE(this)->kbdevent->setPrintableCharacter( key );

        // Translate keycode wx -> So
        void *table;
        if (SoWxKeyboardP::translatetable->find(key, table)) {
            struct SoWxKeyboardP::key1map *map = (struct SoWxKeyboardP::key1map *) table;
            PRIVATE(this)->kbdevent->setKey(map->to);
        } else {
            return NULL;
        }

        // Press or release?
        if (keyrelease) PRIVATE(this)->kbdevent->setState(SoButtonEvent::UP);
        else
            PRIVATE(this)->kbdevent->setState(SoButtonEvent::DOWN);

        PRIVATE(this)->kbdevent->setShiftDown( key_event->ShiftDown());
        PRIVATE(this)->kbdevent->setCtrlDown( key_event->ControlDown());
        PRIVATE(this)->kbdevent->setAltDown( key_event->AltDown());

        this->setEventPosition(PRIVATE(this)->kbdevent,
                               key_event->GetX(),
                               key_event->GetY());

        PRIVATE(this)->kbdevent->setTime(SbTime::getTimeOfDay());
        return PRIVATE(this)->kbdevent;
    }

    return (NULL);
}