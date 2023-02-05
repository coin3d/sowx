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

#ifndef SOWX_SOWXDIRECTIONALLIGHTEDITOR_H
#define SOWX_SOWXDIRECTIONALLIGHTEDITOR_H

#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/Wx/SoWxComponent.h>

typedef void 	SoWxDirectionalLightEditorCB (void *userData, const SoDirectionalLight *light);

class SoPath;
class SoSeparator;
class SoCamera;
class SoPerspectiveCamera;
class SoNodeSensor;
class SoDirectionalLightManip;
class SoCallbackList;
class SoWxClipboard;
class SoWxRenderArea;
class _SoWxColorEditor;
class _SoWxColorSlider;

// *************************************************************************

typedef void SoWxDirectionalLightEditorCB(
        void * user, const SoDirectionalLight * light);

class SOWX_DLL_API SoWxDirectionalLightEditor : public SoWxComponent {

SOWX_OBJECT_HEADER(SoWxDirectionalLightEditor, SoWxComponent);

public:
    SoWxDirectionalLightEditor(
            wxWindow* parent = NULL,
            const char * const name = NULL,
            SbBool embed = TRUE);
    ~SoWxDirectionalLightEditor();

    void attach(SoPath * pathToLight);
    void detach(void);
    SbBool isAttached(void);

    void setLight(const SoDirectionalLight & light);
    const SoDirectionalLight & getLight(void) const;

    void addLightChangedCallback(
            SoWxDirectionalLightEditorCB * callback, void * user = NULL);
    void removeLightChangedCallback(
            SoWxDirectionalLightEditorCB * callback, void * user = NULL);

    virtual void show(void);
    virtual void hide(void);

protected:
    SoWxDirectionalLightEditor(
            wxWindow* parent,
            const char * const name,
            SbBool embed,
            SbBool build);

    virtual const char * getDefaultWidgetName(void) const;
    virtual const char * getDefaultTitle(void) const;
    virtual const char * getDefaultIconTitle(void) const;

    SoDirectionalLight * dirLight;
    SoSeparator * root;
    SoSeparator * litStuff;

    SoPerspectiveCamera * myCamera;
    SoCamera * cameraToWatch;

    _SoWxColorEditor * colorEditor;
    _SoWxColorSlider * intensitySlider;
    SoWxRenderArea * renderArea;
    SoNodeSensor * lightSensor;
    SoNodeSensor * cameraSensor;
    SoDirectionalLightManip * dirLightManip;
    static char * geomBuffer;
    SbBool ignoreCallback;
    SoCallbackList * callbackList;

    SoWxClipboard * clipboard;

    void copyLight(SoDirectionalLight * dst, const SoDirectionalLight * src);
    void updateLocalComponents(void);

private:
    wxWindow* buildWidget(wxWindow* parent);
    void constructor(SbBool build);

}; // class SoWxDirectionalLightEditor

#endif //SOWX_SOWXDIRECTIONALLIGHTEDITOR_H
