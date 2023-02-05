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

/*!
  \class SoWxThumbWheel SoWxThumbWheel.h Inventor/Wx/widgets/SoWxThumbWheel.h
  \brief The SoWxThumbWheel class is a UI component for fancy looking thumbwheel controls.

  \ingroup components
*/


#include <Inventor/Wx/widgets/SoWxThumbWheel.h>
#include <Inventor/Wx/widgets/SoAnyThumbWheel.h>
#include <Inventor/SbBasic.h>
#include <Inventor/errors/SoDebugError.h>

#include "WheelEvents.h"
#include "sowxdefs.h"
#include "Inventor/Wx/viewers/ViewersWxIds.h"

#include <wx/wx.h>
#include <wx/mstream.h>

#include <cassert>
#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdint.h>

wxBEGIN_EVENT_TABLE(SoWxThumbWheel, wxPanel)
                EVT_MOTION(SoWxThumbWheel::mouseMoveEvent)
                EVT_LEFT_DOWN(SoWxThumbWheel::mousePressEvent)
                EVT_LEFT_UP(SoWxThumbWheel::mouseReleaseEvent)
                EVT_MOUSEWHEEL(SoWxThumbWheel::mouseWheel)
                EVT_PAINT(SoWxThumbWheel::paintEvent)
wxEND_EVENT_TABLE()

static const int SHADEBORDERWIDTH = 0;

SoWxThumbWheel::SoWxThumbWheel(wxWindow * parent,
                               const char * name)
        : wxPanel(parent,
                  wxID_ANY) {
    this->constructor(SoWxThumbWheel::Vertical);
}

SoWxThumbWheel::SoWxThumbWheel(Orientation orientation,
                               wxWindow * parent,
                               const char * name)
        : wxPanel(parent,
                  wxID_ANY) {
    if(!name)
        this->SetName("SoWxThumbWheel");
    else
        this->SetName(name);
    this->constructor(orientation);
}

void
SoWxThumbWheel::constructor(Orientation orientation) {
    this->orient = orientation;
    this->state = SoWxThumbWheel::Idle;
    this->wheelValue = this->tempWheelValue = 0.0f;
    this->wheel = new SoAnyThumbWheel;
    this->wheel->setMovement(SoAnyThumbWheel::UNIFORM);
    this->wheel->setGraphicsByteOrder(SoAnyThumbWheel::ARGB);
    this->pixmaps = NULL;
    this->numPixmaps = 0;
    this->currentPixmap = -1;
    wxSize s = sizeHint();
    SetMinSize(s);
    SetMaxSize(s);
}

SoWxThumbWheel::~SoWxThumbWheel() {
    delete this->wheel;
    if (this->pixmaps) {
        for (int i = 0; i < this->numPixmaps; i++)
            delete this->pixmaps[i];
        delete [] this->pixmaps;
    }
}

void
SoWxThumbWheel::setOrientation(Orientation orientation) {
    this->orient = orientation;
    Refresh();
}

void
SoWxThumbWheel::paintEvent(wxPaintEvent& WXUNUSED(event)) {
    wxPaintDC dc(this);

    int w, dval;
    wxSize size= this->GetSize();
    if (this->orient == SoWxThumbWheel::Vertical) {
        w = size.GetX() - 12;
        dval = size.GetY() - 6;
    } else {
        w = size.GetY() - 12;
        dval = size.GetX() - 6;
    }

#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxThumbWheel::paintEvent",
                              "dval: %d and w: %d",
                              dval, w);
#endif

    // Handle resizing to too small dimensions gracefully.
    if ((dval <= 0) || (w <= 0)) return;

    this->initWheel(dval, w);

    int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                                (this->state == SoWxThumbWheel::Disabled) ?
                                                SoAnyThumbWheel::DISABLED : SoAnyThumbWheel::ENABLED);
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxThumbWheel::paintEvent",
                              "pixmap value is: %d and bitmap pointer is %p",
                              pixmap, this->pixmaps);
#endif

    if(pixmap >= numPixmaps)
        return;

    wxRect widgetrect(0, 0,
                      this->GetSize().GetWidth(),
                      this->GetSize().GetHeight());
    wxRect wheelrect(widgetrect);

    if (this->orient == Vertical) {
        wheelrect.SetTop(   wheelrect.GetTop() + 2);
        wheelrect.SetBottom(wheelrect.GetBottom() - 4);
        wheelrect.SetLeft(  wheelrect.GetLeft() + 5);
        wheelrect.SetRight( wheelrect.GetRight() - 10);
    } else {
        wheelrect.SetTop(   wheelrect.GetTop() + 5);
        wheelrect.SetBottom(wheelrect.GetBottom() - 10);
        wheelrect.SetLeft(  wheelrect.GetLeft() + 2);
        wheelrect.SetRight( wheelrect.GetRight() - 4);
    }
    dc.DrawRectangle(wheelrect);

    wheelrect.SetTop(   wheelrect.GetTop() + 1);
    wheelrect.SetBottom(wheelrect.GetBottom() - 1);
    wheelrect.SetLeft(  wheelrect.GetLeft() + 1);
    wheelrect.SetRight( wheelrect.GetRight() - 1);
    // wheelrect is now wheel-only

    wxRect dRect;
    if (this->orient == Vertical) {
        dRect = wxRect(wheelrect.GetLeft(),wheelrect.GetTop(),w,dval);
    }
    else {
        dRect = wxRect(wheelrect.GetLeft(),wheelrect.GetTop(),dval,w);
    }

    assert(pixmap < numPixmaps);
    wxBitmap bitmap(*this->pixmaps[pixmap]);
    dc.DrawBitmap(bitmap, dRect.GetX(), dRect.GetY(), false);

    this->currentPixmap = pixmap;
}

/*!
  \internal
*/

void
SoWxThumbWheel::mousePressEvent(wxMouseEvent&  event) {
    if (this->state != SoWxThumbWheel::Idle)
        return;
    this->state = SoWxThumbWheel::Dragging;

    if (this->orient == SoWxThumbWheel::Vertical)
        this->mouseDownPos = event.GetY() - SHADEBORDERWIDTH - 6;
    else
        this->mouseDownPos = event.GetX() - SHADEBORDERWIDTH - 6;

    this->mouseLastPos = this->mouseDownPos;

#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxThumbWheel::mouseMoveEvent","");
#endif

    sendEvent(SO_WX_MOUSE_WHEEL_PRESSED, "mousePressEvent");
}

/*!
  \internal
*/
void
SoWxThumbWheel::mouseMoveEvent(wxMouseEvent& event) {

    if (this->state != SoWxThumbWheel::Dragging)
        return;

    if (this->orient == SoWxThumbWheel::Vertical)
        this->mouseLastPos = event.GetY() - SHADEBORDERWIDTH - 6;
    else
        this->mouseLastPos = event.GetX() - SHADEBORDERWIDTH - 6;

    int delta = this->mouseLastPos - this->mouseDownPos;
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxThumbWheel::mouseMoveEvent",
                           "delta: %d wheelValue: %d mouseDownPos: %d",
                           delta,
                           this->wheelValue,
                           this->mouseDownPos);
#endif

    this->tempWheelValue = this->wheel->calculateValue(this->wheelValue,
                                                       this->mouseDownPos,
                                                       delta);

    sendEvent(SO_WX_MOUSE_WHEEL_MOVED, "mouseWheel");
    Refresh();
}

/*!
  \internal
*/
void
SoWxThumbWheel::mouseReleaseEvent(wxMouseEvent& WXUNUSED(event)) {
    if (this->state != SoWxThumbWheel::Dragging)
        return;

    this->wheelValue = this->tempWheelValue;
    this->mouseLastPos = this->mouseDownPos;
    this->state = SoWxThumbWheel::Idle;
    sendEvent(SO_WX_MOUSE_WHEEL_RELEASED, "mouseReleaseEvent");
}

void
SoWxThumbWheel::mouseWheel(wxMouseEvent &WXUNUSED(event)) {
    SOWX_STUB();
    return;
#if 0
    int delta = /*event.GetWheelDelta() * */(float)(event.GetWheelRotation()) / 120.0;
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxThumbWheel::mouseWheel",
                           "delta: %d wheelValue: %d mouseDownPos: %d",
                           delta,
                           this->wheelValue,
                           this->mouseDownPos);
#endif

    this->tempWheelValue = this->wheel->calculateValue(this->wheelValue,
                                                       this->mouseDownPos,
                                                       delta);
    Refresh();
    sendEvent(SO_WX_MOUSE_WHEEL_MOVED,
              "mouseWheel");
#endif
}

/*
float
SoWxThumbWheel::getNormalizedValue(int pos) const
{
  int relativepos = pos - this->mouseDownPos;
  return (float) relativepos / (float)this->getWheelLength() * 2.0f;
}
*/

/*
int
SoWxThumbWheel::getWheelLength() const
{
  return this->orient == SoWxThumbWheel::Vertical ?
    this->GetSize().GetY() : this->GetSize().GetX();
}
*/

/*
int
SoWxThumbWheel::orientedCoord(const QPoint &p) const
{
  return (this->orient == SoWxThumbWheel::Horizontal) ?  p.x() : p.y();
}
*/

wxSize
SoWxThumbWheel::sizeHint() const {
    const int length = 122;
    int thick = 24;

    if (this->orient == SoWxThumbWheel::Horizontal)
        return wxSize(length, thick);
    else
        return wxSize(thick, length);
}

SoWxThumbWheel::Orientation
SoWxThumbWheel::orientation() const {
    return this->orient;
}

float
SoWxThumbWheel::value() const {
    return this->wheelValue;
}

void
fill(std::vector<uint8_t>& buffer,
          unsigned long n,
          int channel = 3) {
    if(channel>3) {
        buffer.push_back((n >> 24) & 0xFF);
    }
    buffer.push_back((n >> 16) & 0xFF);
    buffer.push_back((n >> 8) & 0xFF);
    buffer.push_back(n & 0xFF);
}

uint8_t*
toRGBChannel(const std::vector<unsigned int>& img) {
    std::vector<uint8_t> vout;
    for(size_t i=0;i<img.size();++i) {
        fill(vout, img[i]);
    }
    assert(vout.size() == img.size()*3);
    uint8_t* out = static_cast<uint8_t*>(malloc( vout.size() ));
    memcpy(out, &vout[0], vout.size());
    return (out);
}

void
SoWxThumbWheel::initWheel(int diameter, int width) {
    int dval, w;
    this->wheel->getSize(dval, w);
    if (dval == diameter && w == width) return;

    this->wheel->setSize(diameter, width);

    int pwidth = width;
    int pheight = diameter;
    if (this->orient == Horizontal) {
        pwidth = diameter;
        pheight = width;
    }

    if (this->pixmaps != NULL) {
        for (int i = 0; i < this->numPixmaps; i++)
            delete this->pixmaps[i];
        delete [] this->pixmaps;
    }

    this->numPixmaps = this->wheel->getNumBitmaps();
    this->pixmaps = new wxBitmap * [this->numPixmaps];

    for (int i = 0; i < this->numPixmaps; i++) {
        std::vector<unsigned int> buffer(pwidth*pheight);
        this->wheel->drawBitmap(i, &buffer[0],
                                (this->orient == Vertical) ? SoAnyThumbWheel::VERTICAL :
                                SoAnyThumbWheel::HORIZONTAL);
        uint8_t* rgb = toRGBChannel(buffer);
        wxImage img(pwidth, pheight,rgb);
        assert(img.IsOk());
        this->pixmaps[i] = new wxBitmap(img);
    }
}

void
SoWxThumbWheel::setEnabled(bool enable) {
    if (enable)
        this->state = SoWxThumbWheel::Idle;
    else
        this->state = SoWxThumbWheel::Disabled;
    Refresh();
}

bool
SoWxThumbWheel::isEnabled() const {
    return (this->state != SoWxThumbWheel::Disabled);
}

void
SoWxThumbWheel::setValue(float value) {
    this->wheelValue = this->tempWheelValue = value;
    this->mouseDownPos = this->mouseLastPos;
    Refresh();
}

void
SoWxThumbWheel::setRangeBoundaryHandling(boundaryHandling handling) {
    switch (handling) {
        case CLAMP:
            this->wheel->setBoundaryHandling(SoAnyThumbWheel::CLAMP);
            break;
        case MODULATE:
            this->wheel->setBoundaryHandling(SoAnyThumbWheel::MODULATE);
            break;
        case ACCUMULATE:
            this->wheel->setBoundaryHandling(SoAnyThumbWheel::ACCUMULATE);
            break;
        default:
            assert(0 && "impossible");
    }
}

SoWxThumbWheel::boundaryHandling
SoWxThumbWheel::getRangeBoundaryHandling() const {
    switch (this->wheel->getBoundaryHandling()) {
        case SoAnyThumbWheel::CLAMP:
            return CLAMP;
        case SoAnyThumbWheel::MODULATE:
            return MODULATE;
        case SoAnyThumbWheel::ACCUMULATE:
            return ACCUMULATE;
        default:
            assert(0 && "impossible");
    }
    return CLAMP; // never reached
}

void
SoWxThumbWheel::sendEvent(long id,
                          const std::string& event_id) {
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxThumbWheel::sendEvent",
                           "id: %d event: %s tempWheelValue: %d",
                           id,
                           event_id.c_str(),
                           this->tempWheelValue);
#endif

    wxCommandEvent a_wx_event(id, GetId());
    a_wx_event.SetEventObject(this);
    a_wx_event.SetString(event_id);
    a_wx_event.SetClientData(&this->tempWheelValue);
    ProcessWindowEvent(a_wx_event);
}
