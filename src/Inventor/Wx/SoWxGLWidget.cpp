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

#include "Inventor/Wx/SoWxGLWidget.h"
#include "Inventor/Wx/SoWxGLWidgetP.h"
#include "Inventor/Wx/widgets/SoWxGLArea.h"
#include "sowxdefs.h"
#include "Inventor/Wx/SoAny.h"

SOWX_OBJECT_ABSTRACT_SOURCE(SoWxGLWidget);

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SoWxGLWidget::SoWxGLWidget(wxWindow* const parent ,
                           const char * const name ,
                           const SbBool embed ,
                           const int glmodes ,
                           const SbBool build):
        inherited(parent, name, embed),
        waitForExpose(true),
        drawToFrontBuffer(false) {

    PRIVATE(this) = new SoWxGLWidgetP(this);
    PRIVATE(this)->initGLModes(glmodes);

    PRIVATE(this)->glSize = SbVec2s(0, 0);
    PRIVATE(this)->glSizeUnscaled = SbVec2s(0, 0);
    PRIVATE(this)->wasresized = false;

    PRIVATE(this)->glparent = NULL;
    PRIVATE(this)->currentglwidget = NULL;
    PRIVATE(this)->previousglwidget = NULL;
    PRIVATE(this)->currentglarea = NULL;
    PRIVATE(this)->previousglarea = NULL;

    if (! build) {
        return;
    }

    this->setClassName("SoWxGLWidget");
    wxWindow* parent_widget = this->getParentWidget();
    wxWindow* widget = this->buildWidget(parent_widget);
    this->setBaseWidget(widget);
}

SoWxGLWidget::~SoWxGLWidget() {

    if (PRIVATE(this)->currentglwidget) {
        SoAny::si()->unregisterGLContext((void *)this);
    }

    delete PRIVATE(this);
}

void
SoWxGLWidget::setBorder(const SbBool enable) {
    SOWX_STUB();
}

void
SoWxGLWidget::setQuadBufferStereo(const SbBool enable) {
    SOWX_STUB();
}

SbBool
SoWxGLWidget::isQuadBufferStereo(void) const {
    return (FALSE);
}

void
SoWxGLWidget::setGLSize(const SbVec2s size){
    if (size == PRIVATE(this)->glSize) return;
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxGLWidget::setGLSize",
                           "[invoked (%d, %d)]", size[0], size[1]);
#endif // debug
    PRIVATE(this)->glSize = size;
    PRIVATE(this)->glSizeUnscaled = size;
    if (PRIVATE(this)->currentglwidget) {
        int frame = this->isBorder() ? PRIVATE(this)->borderthickness : 0;
        // PRIVATE(this)->currentglwidget->SetSize( size[0], size[1] );
    }
}

template <typename T>
T
toSbVec2(const wxSize& wx_size) {
    return (T(wx_size.GetX(), wx_size.GetY()));
}

SbVec2s
SoWxGLWidget::getGLSize(void) const{
    return (PRIVATE(this)->glSize);
}

float
SoWxGLWidget::getGLAspectRatio(void) const{
    SbVec2f v2(toSbVec2<SbVec2f>(PRIVATE(this)->currentglwidget->GetSize()));
    return ( v2[0] /v2[1] );
}

SbBool
SoWxGLWidget::isRGBMode(void){
    const bool rgb_mode = SoWxGLArea::isGLFeatureAvailable(PRIVATE(this)->gl_attributes,
                                                                WX_GL_RGBA);
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxGLWidget::isRGBMode",
                           ": %d",
                           rgb_mode);
#endif
    return (rgb_mode);
}

void
SoWxGLWidget::glLockNormal(void){
    assert(PRIVATE(this)->currentglarea != NULL);
    PRIVATE(this)->currentglarea->makeCurrent();
}

void
SoWxGLWidget::glUnlockNormal(void){
    // do nothing
}

void
SoWxGLWidget::glLockOverlay(void){
    SOWX_STUB();
}

void
SoWxGLWidget::glUnlockOverlay(void){
    SOWX_STUB();
}

void
SoWxGLWidget::glSwapBuffers(void){
    PRIVATE(this)->currentglarea->SwapBuffers();
}

void
SoWxGLWidget::glFlushBuffer(void){
    glFlush();
}

SbBool
SoWxGLWidget::glScheduleRedraw(void){
    SOWX_STUB();
    return (TRUE);
}

SbBool
SoWxGLWidget::isBorder(void) const{
    return (PRIVATE(this)->borderthickness ? true : false);
}

void
SoWxGLWidget::setDoubleBuffer(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoWxGLWidget::isDoubleBuffer(void) const{
    const bool double_buffer = SoWxGLArea::isGLFeatureAvailable(PRIVATE(this)->gl_attributes,
                                                                WX_GL_DOUBLEBUFFER);
#if SOWX_DEBUG && 0
    SoDebugError::postInfo("SoWxGLWidget::isDoubleBuffer",
                           ": %d",
                           double_buffer);
#endif
    return (double_buffer);
}

void
SoWxGLWidget::setDrawToFrontBufferEnable(const SbBool enable){
    drawToFrontBuffer = enable;
}

SbBool
SoWxGLWidget::isDrawToFrontBufferEnable(void) const{
    return (drawToFrontBuffer);
}

void
SoWxGLWidget::setAccumulationBuffer(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoWxGLWidget::getAccumulationBuffer(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoWxGLWidget::setStencilBuffer(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoWxGLWidget::getStencilBuffer(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoWxGLWidget::setAlphaChannel(const SbBool enable){
    SOWX_STUB();
}

SbBool
SoWxGLWidget::getAlphaChannel(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoWxGLWidget::setOverlayRender(const SbBool onoff){
    SOWX_STUB();
}

SbBool
SoWxGLWidget::isOverlayRender(void) const{
    SOWX_STUB();
    return (FALSE);
}

void
SoWxGLWidget::setSampleBuffers(const int numsamples){
    SOWX_STUB();
}

int
SoWxGLWidget::getSampleBuffers(void) const{
    SOWX_STUB();
    return (0);
}

wxWindow*
SoWxGLWidget::getGLWidget(void) const{
    return (PRIVATE(this)->currentglwidget);
}

wxWindow*
SoWxGLWidget::getNormalWidget(void) const{
    SOWX_STUB();
    return (0);
}

wxWindow*
SoWxGLWidget::getOverlayWidget(void) const{
    SOWX_STUB();
    return (0);
}

SbBool
SoWxGLWidget::hasOverlayGLArea(void) const{
    SOWX_STUB();
    return (false);
}

SbBool
SoWxGLWidget::hasNormalGLArea(void) const{
    bool res = (PRIVATE(this)->currentglarea->context() != 0);
    return (res);
}

unsigned long
SoWxGLWidget::getOverlayTransparentPixel(void){
    SOWX_STUB();
    return (0);
}

void
SoWxGLWidget::processEvent(wxEvent& WXUNUSED(event)){
    // Nothing is done here for the SoWxGLWidget, as realize, resize and
    // expose events are caught by explicitly attaching signal callbacks
    // to the widget.
}

wxWindow *
SoWxGLWidget::buildWidget(wxWindow* parent) {
    assert(parent !=0 && "parent can not be null");
    PRIVATE(this)->glparent = parent;
    return (PRIVATE(this)->buildGLWidget());
}

void
SoWxGLWidget::redrawOverlay(void){
    SOWX_STUB();
}

void
SoWxGLWidget::initGraphic(void){
    this->glLockNormal();
    // Need to set this explicitly when running on top of Open Inventor,
    // as it seems to have been forgotten there.
    // This code should be invoked from SoWxRenderArea::initGraphics()
    glEnable(GL_DEPTH_TEST);
    this->glUnlockNormal();
}
void
SoWxGLWidget::initOverlayGraphic(void){
    SOWX_STUB();
}

void
SoWxGLWidget::sizeChanged(const SbVec2s & size){
    SOWX_STUB();
#if SOWX_DEBUG
    SoDebugError::postInfo("SoWxGLWidget::sizeChanged",
                           "<%d, %d>",
                           size[0], size[1]
    );
#endif
    // Do nothing
}

void
SoWxGLWidget::widgetChanged(wxWindow* w){
    SOWX_STUB();
}

#undef PRIVATE
#undef PUBLIC
