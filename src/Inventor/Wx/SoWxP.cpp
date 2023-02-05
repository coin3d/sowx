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

#include "Inventor/Wx/SoWxP.h"

#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <iostream>
#include <wx/sizer.h>
#include <sstream>

class SoWxApp : public  wxApp {
public:

    virtual bool
    OnInit()  {
        if ( !wxApp::OnInit() )
            return false;
        return true;
    }

    virtual void
    CleanUp()  {
#if SOWX_DEBUG
        SoDebugError::postInfo("SoWxApp::CleanUp",
                               "done!");
#endif
    }
};

wxTimer * SoWxP::timerqueuetimer = 0;
wxTimer * SoWxP::delaytimeouttimer = 0;

SoWxP::SoWxP() {
    init = false;
    main_frame = 0;
    main_app = 0;
    is_a_sowwp_app = false;
}

void
SoWxP::buildWxApp() {
    if(!main_app) {
        is_a_sowwp_app = true;
        setWxApp( new SoWxApp);
    } else if (SOWX_DEBUG){
        SoDebugError::postWarning("SoWxP::buildWxApp",
                               "wxApp already built");
    }
}

void
SoWxP::setWxApp(wxAppConsole* app) {
    main_app = app;
}

void
SoGuiP::sensorQueueChanged(void *) {
    SoWxP::instance()->sensorQueueChanged();
}

class TimerQueueTimer : public wxTimer {
public:
    virtual void
    Notify() {
#if SOWX_DEBUG && 0
            SoDebugError::postInfo("TimerQueueTimer::Notify",
                                   "processing timer queue");
            SoDebugError::postInfo("TimerQueueTimer::Notify",
                                   "is %s",
                                   this->IsRunning() ?
                                   "active" : "inactive");
#endif

        SoDB::getSensorManager()->processTimerQueue();

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(NULL);
    }
};

// The delay sensor timeout point has been reached, so process the
// delay queue even though the system is not idle (to avoid
// starvation).
class DelayTimeoutTimer : public wxTimer {
public:
    virtual void
    Notify() {
#if SOWX_DEBUG && 0
            SoDebugError::postInfo("DelayTimeoutTimer::Notify",
                                   "processing delay queue");
            SoDebugError::postInfo("DelayTimeoutTimer::Notify", "is %s",
                                   this->IsRunning() ?
                                   "active" : "inactive");
#endif

        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(false);

        // The change callback is _not_ called automatically from
        // SoSensorManager after the process methods, so we need to
        // explicitly trigger it ourselves here.
        SoGuiP::sensorQueueChanged(NULL);
    }
};

void
SoWxP::sensorQueueChanged(void) {
    // We need three different mechanisms to interface Coin sensor
    // handling with wxWidgets event handling, which are:
    //
    // 1. Detect when the application is idle and then empty the
    // delay-queue completely for delay-sensors -- handled by
    // SoWxP::onIdle.
    //
    // 2. Detect when one or more timer-sensors are ripe and trigger
    // those -- handled by SoWxP::timerqueuetimer.
    //
    // 3. On the "delay-sensor timeout interval", empty all highest
    // priority delay-queue sensors to avoid complete starvation in
    // continually busy applications -- handled by
    // SoWxP::delaytimeouttimer.


    // Allocate wx timers on first call.
    SoWxP::initTimers();

    SoSensorManager * sm = SoDB::getSensorManager();
    if (SoWxP::timerqueuetimer) {
        // Set up timer queue timeout if necessary.
        SbTime t;
        if (sm->isTimerSensorPending(t)) {
            SbTime interval = t - SbTime::getTimeOfDay();

            // We also want to avoid setting it to 0.0, as that has a special
            // semantic meaning: trigger only when the application is idle and
            // event queue is empty -- which is not what we want to do here.
            //
            // So we clamp it, to a small positive value:
            if (interval.getValue() <= 0.0) { interval.setValue(1.0 / 5000.0); }

#if SOWX_DEBUG && 0
                SoDebugError::postInfo("SoWxP::sensorQueueChanged",
                    "timersensor pending, interval %f",
                    interval.getValue());
#endif

            // Change interval of timerqueuetimer when head node of the
            // timer-sensor queue of SoSensorManager changes.
            SoWxP::timerqueuetimer->Start((int)interval.getMsecValue(), true);
        }
        // Stop timerqueuetimer if queue is completely empty.
        else if (SoWxP::timerqueuetimer->IsRunning()) {
            SoWxP::timerqueuetimer->Stop();
        }
    }


    // Set up idle notification for delay queue processing if necessary.
    if (SoWxP::delaytimeouttimer) {
        if (sm->isDelaySensorPending()) {
            if (SOWX_DEBUG && 0) { // debug
                SoDebugError::postInfo("SoWxP::sensorQueueChanged",
                    "delaysensor pending");
            }

            if (!SoWxP::delaytimeouttimer->IsRunning()) {
                const SbTime& delaySensorTimeout = SoDB::getDelaySensorTimeout();
                if (delaySensorTimeout != SbTime::zero()) {
                    unsigned long timeout = delaySensorTimeout.getMsecValue();
                    SoWxP::delaytimeouttimer->Start((int)timeout, true);
                }
            }
        }
        else {
            if (SoWxP::delaytimeouttimer->IsRunning())
                SoWxP::delaytimeouttimer->Stop();
        }
    }
}

SoWxP *
SoWxP::instance() {
    static SoWxP singleton;
    return (&singleton);
}

bool
SoWxP::isInitialized() const {
    return (init);
}

void
SoWxP::setInitialize(bool i) {
    init = i;
}

wxWindow *
SoWxP::getMainFrame() const {
    return (main_frame);
}

void
SoWxP::setMainFrame(wxWindow * frame) {
    main_frame = frame;
}

#define INIT_TIMER(timer_name, timer_type)  \
    if (!timer_name) {                      \
        timer_name = new timer_type;        \
    }                                       \
    assert(timer_name != 0)

void
SoWxP::initTimers() {
    static bool are_initialized = false;

    if(!are_initialized) {
        INIT_TIMER(SoWxP::timerqueuetimer, TimerQueueTimer);
        INIT_TIMER(SoWxP::delaytimeouttimer, DelayTimeoutTimer);
        are_initialized = true;
    }
}

#undef INIT_TIMER

#define STOP_TIMER(timer_name) if(timer_name) timer_name->Stop()

void
SoWxP::stopTimers() {
    STOP_TIMER(SoWxP::timerqueuetimer);
    wxDELETE(SoWxP::timerqueuetimer);
    STOP_TIMER(SoWxP::delaytimeouttimer);
    wxDELETE(SoWxP::delaytimeouttimer);
}

#undef STOP_TIMER

void
SoWxP::finish() {
#ifdef SOWX_DEBUG
    SoDebugError::postInfo("SoWxP::finish",
                           "remove all internal resources");
#endif
    wxTheApp->Unbind(wxEVT_IDLE, &SoWxP::onIdle, SoWxP::instance());

    stopTimers();

    // only if app is built by SoWx perform exit and cleanup
    if(SoWxP::instance()->is_a_sowwp_app) {
        wxTheApp->OnExit();
        wxEntryCleanup();
    }
}

void
SoWxP::onIdle(wxIdleEvent& WXUNUSED(event)) {
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxP::onIdle",
                               "idlesensor pending");
#endif

    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(true);

    // The change callback is _not_ called automatically from
    // SoSensorManager after the process methods, so we need to
    // explicitly trigger it ourselves here.
    SoGuiP::sensorQueueChanged(NULL);
}

void
SoWxP::onClose(wxCloseEvent& event) {
#if SOWX_DEBUG
    SoDebugError::postInfo("SoWxP::onClose",
                           "clean up in progress!");
#endif

    // turn off timers
    SoWxP::instance()->stopTimers();

    // To avoid getting any further invocations of
    // SoGuiP::sensorQueueChanged() (which would re-allocate the timers
    // we destruct below). This could for instance happen when
    // de-coupling the scenegraph camera, triggering a notification
    // chain through the scenegraph.
    SoDB::getSensorManager()->setChangedCallback(NULL, NULL);

    event.Skip(); // perform destroy
}
