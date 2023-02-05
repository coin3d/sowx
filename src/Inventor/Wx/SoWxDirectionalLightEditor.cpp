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

#include "Inventor/Wx/SoWxDirectionalLightEditor.h"

#include "sowxdefs.h"

SOWX_OBJECT_SOURCE(SoWxDirectionalLightEditor);

SoWxDirectionalLightEditor::SoWxDirectionalLightEditor(wxWindow *parent, const char *const name, SbBool embed)
        : SoWxComponent(parent, name, embed) {

}

SoWxDirectionalLightEditor::SoWxDirectionalLightEditor(wxWindow *parent, const char *const name, SbBool embed,
                                                       SbBool build) {
    SOWX_STUB();
}

SoWxDirectionalLightEditor::~SoWxDirectionalLightEditor() {

}


void SoWxDirectionalLightEditor::attach(SoPath *pathToLight) {
    SOWX_STUB();
}

void SoWxDirectionalLightEditor::detach() {
    SOWX_STUB();
}

SbBool SoWxDirectionalLightEditor::isAttached() {
    SOWX_STUB();
    return 0;
}

void SoWxDirectionalLightEditor::setLight(const SoDirectionalLight &newLight) {
    SOWX_STUB();
}

const SoDirectionalLight &SoWxDirectionalLightEditor::getLight() const {
    SOWX_STUB();
    static SoDirectionalLight* light = 0;
    return (*light);
}

void SoWxDirectionalLightEditor::addLightChangedCallback(SoWxDirectionalLightEditorCB *f, void *userData) {
    SOWX_STUB();
}

void SoWxDirectionalLightEditor::removeLightChangedCallback(SoWxDirectionalLightEditorCB *f, void *userData) {
    SOWX_STUB();
}

void SoWxDirectionalLightEditor::show() {
    SOWX_STUB();
}

void SoWxDirectionalLightEditor::hide() {
    SOWX_STUB();
}


const char *SoWxDirectionalLightEditor::getDefaultWidgetName(void) const {
    SOWX_STUB();
    return ("");
}

const char *SoWxDirectionalLightEditor::getDefaultTitle(void) const {
    SOWX_STUB();
    return ("");
}

const char *SoWxDirectionalLightEditor::getDefaultIconTitle(void) const {
    SOWX_STUB();
    return ("");
}

