/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#if 0
#ifndef SOWX_GRAPHEDITOR_H
#define SOWX_GRAPHEDITOR_H

#include <Inventor/Wx/SoWxComponent.h>

class SoNode;
class SoField;

// *************************************************************************

class SOWX_DLL_API SoWxGraphEditor : public SoWxComponent {
  SOWX_OBJECT_HEADER(SoWxGraphEditor, SoWxComponent);

public:
  enum BuildFlag {
    MENUBAR =       0x01,
    GRAPHEDITOR =   0x02,
    STATUSBAR =     0x04,
    EVERYTHING =    0x07
  };

  SoWxGraphEditor(wxWindow * const parent = (wxWindow *) NULL,
                   const char * const name = (char *) NULL,
                   const SbBool embed = TRUE,
                   const int parts = EVERYTHING);
  ~SoWxGraphEditor(void);

  virtual void setSceneGraph(SoNode * root);
  SoNode * getSceneGraph(void) const;

protected:
  SoWxGraphEditor(wxWindow * const parent, const char * const name,
                   const SbBool embed, const int parts, const SbBool build);

  wxWindow * buildWidget(wxWindow * parent);
  virtual wxWindow * buildMenuBarWidget(wxWindow * parent);
  virtual wxWindow * buildGraphEditorWidget(wxWindow * parent);
  virtual wxWindow * buildStatusBarWidget(wxWindow * parent);

  virtual void sizeChanged(const SbVec2s & size);

  virtual void buildSceneGraphTree(void);
  virtual void clearSceneGraphTree(void);

  virtual void saveSceneGraph(void);

  virtual void setStatusMessage(const char * message);

  virtual void nodeSelection(wxWindow * item, SoNode * node);
  virtual void fieldSelection(wxWindow * item, SoNode * node, SoField * field);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor(const SbBool build, const int parts);

  static void saveCB(wxWindow * obj, void* closure);
  static void closeCB(wxWindow * obj, void* closure);
  static void selectionCB(wxWindow * obj, void* closure);

  wxWindow * buildSubGraph(wxWindow * parent, SoNode * node);

  SoNode * scenegraph;

  int buildflags;
  wxWindow * editorbase;
  wxWindow * menubar;
  wxWindow * grapheditor;
  wxWindow * graphroot;
  wxWindow * statusbar;
  wxWindow * statusmessage;

  WxAdjustment * vertical;
  WxAdjustment * horizontal;

}; // class SoWxGraphEditor

// *************************************************************************

#endif // ! SOWX_H
#endif