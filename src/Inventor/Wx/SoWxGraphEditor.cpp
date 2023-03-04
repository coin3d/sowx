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

#if HAVE_CONFIG_H
#include <config.h>
#endif


#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/SoLists.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoWriteAction.h>

#include <sowxdefs.h>

#include <Inventor/Wx/SoWxGraphEditor.h>

#include <wx/treectrl.h>


// *************************************************************************

/*!
  \class SoWxGraphEditor Inventor/Wx/SoWxGraphEditor.h
  \brief The SoWxGraphEditor class is a component for viewing and editing
  the Inventor scene graph of a model.
  \ingroup editors

  This component is not implemented so far that editing is possible.
*/

/*!
  \enum SoWxGraphEditor::BuildFlag
  FIXME: write doc
*/

/*!
  \var SoWxGraphEditor::BuildFlag SoWxGraphEditor::MENUBAR
  FIXME: write doc
*/

/*!
  \var SoWxGraphEditor::BuildFlag SoWxGraphEditor::GRAPHEDITOR
  FIXME: write doc
*/

/*!
  \var SoWxGraphEditor::BuildFlag SoWxGraphEditor::STATUSBAR
  FIXME: write doc
*/

/*!
  \var SoWxGraphEditor::BuildFlag SoWxGraphEditor::EVERYTHING
  FIXME: write doc
*/

// *************************************************************************

SOWX_OBJECT_SOURCE(SoWxGraphEditor);

// *************************************************************************

/*!
  Public constructor.
*/

SoWxGraphEditor::SoWxGraphEditor(
        wxWindow * const parent,
        const char * const name,
        const SbBool embed,
        const int parts)
        : inherited(parent, name, embed)
{
    this->constructor(TRUE, parts);
} // SoWxGraphEditor()

/*!
  Protected constructor.
*/

SoWxGraphEditor::SoWxGraphEditor(
        wxWindow * const parent,
        const char * const name,
        const SbBool embed,
        const int parts,
        const SbBool build)
        : inherited(parent, name, embed)
{
    this->constructor(build, parts);
} // SoWxGraphEditor()

/*!
  Common constructor code.
*/

void
SoWxGraphEditor::constructor(
        const SbBool build,
        const int parts)
{
    this->scenegraph = (SoNode *) NULL;

    this->buildflags = parts & EVERYTHING;

    this->editorbase = (wxWindow *) NULL;
    this->menubar = (wxWindow *) NULL;
    this->grapheditor = (wxWindow *) NULL;
    this->graphroot = (wxWindow *) NULL;
    this->statusbar = (wxWindow *) NULL;
    this->statusmessage = (wxWindow *) NULL;

    /*
    this->horizontal = (WxAdjustment *) NULL;
    this->vertical = (WxAdjustment *) NULL;
      */
    this->setClassName("SoWxGraphEditor");

    if (! build) return;
    this->setSize(SbVec2s(350, 450));
    wxWindow * parent = this->getParentWidget();
    wxWindow * editor = this->buildWidget(parent);
    this->setBaseWidget(editor);
} // constructor()

/*!
  Destructor.
*/

SoWxGraphEditor::~SoWxGraphEditor(
        void)
{
} // ~SoWxGraphEditor()

// *************************************************************************

/*!
  Set the scene graph to edit.
*/

void
SoWxGraphEditor::setSceneGraph(// virtual
        SoNode * root)
{
    if (this->scenegraph) {
        this->clearSceneGraphTree();
        this->scenegraph->unref();
    }
    this->scenegraph = root;
    if (this->scenegraph) {
        this->scenegraph->ref();
        this->buildSceneGraphTree();
    }
} // setSceneGraph()

/*!
  Returns the scene graph used in the graph editor.
*/

SoNode *
SoWxGraphEditor::getSceneGraph(
        void) const
{
    return this->scenegraph;
} // getSceneGraph;

/*
  FIXME: write doc
*/

struct wxSoData : public  wxTreeItemData {
    enum Type {
        NODE,
        FIELD,
        UNKNOWN
    };
    Type type;
    SoNode* node;
    SoField* field;
    wxSoData(SoNode* aNode,
             SoField* aField,
             Type aType)
    :node(aNode)
    ,field(aField)
    ,type(aType){

    }
};


wxTreeItemId
SoWxGraphEditor::buildSubGraph(
        wxTreeItemId parent,
        SoNode * node)
{
    assert(node != NULL);
    const char * classname = node->getTypeId().getName().getString();
    SbName nodename = node->getName();
    wxTreeItemId subtree;

    if (! nodename) {
        nodename = classname;
    } else {
        const char * name = nodename.getString();
        char buf[128];
        sprintf(buf, "%s \"%s\"", classname, name);
        nodename = buf;
    }

    if(!parent)
        subtree = treeCtrl->AppendItem(parent, classname);
    else
        subtree = treeCtrl->AppendItem(parent, classname);

    treeCtrl->SetItemData(subtree, new wxSoData(node, 0, wxSoData::NODE));
    treeCtrl->Bind(wxEVT_TREE_ITEM_ACTIVATED,  &SoWxGraphEditor::selectionCB, this);

    SoFieldList fields;
    if (node->getFields(fields) > 0) {
        wxTreeItemId  fieldstree = treeCtrl->AppendItem(subtree, "[fields]" );
        for (int i = 0; i < fields.getLength(); i++) {
            SoField * field = fields[i];
            SbName fieldname;
            node->getFieldName(field, fieldname);
            const char * fieldnamestr = fieldname.getString();
            wxTreeItemId  fieldnode = treeCtrl->AppendItem(fieldstree, fieldnamestr );
            treeCtrl->SetItemData(fieldnode, new wxSoData(node, field, wxSoData::FIELD));
        }
    }

    SoChildList * children = node->getChildren();
    if (children != NULL && children->getLength() > 0) {
        for (int i = 0; i < children->getLength(); i++)
            SoWxGraphEditor::buildSubGraph(subtree, (SoNode *) children->get(i));
    }
    return (subtree);
}

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::buildSceneGraphTree(// virtual, protected
        void)
{
    wxTreeItemId graph = this->buildSubGraph(0, this->scenegraph);
    treeCtrl->ExpandAll();
    // TODO: ww_tree_item_expand(WX_TREE_ITEM(graph));
} // buildSceneGraphTree()

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::clearSceneGraphTree(// virtual, protected
        void)
{
    SoDebugError::postInfo("SoWxGraphEditor::clearSceneGraphTree",
                           _("[not implemented]"));
} // clearSceneGraphTree()

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::saveSceneGraph(// virtual, protected
        void)
{
    if (! this->scenegraph) {
        this->setStatusMessage(_("No scene to save."));
        return;
    }
    SoOutput * output = new SoOutput;
    if (! output->openFile("scene.iv")) {
        this->setStatusMessage(_("Error opening 'scene.iv' for writing."));
        delete output;
        return;
    }
    SoWriteAction writer(output);
    writer.apply(this->scenegraph);
    delete output;
    this->setStatusMessage(_("Scene saved in 'scene.iv'."));
} // saveSceneGraph()

// *************************************************************************

/*!
  This method returns builds and returns the basic graph editor component.
*/

wxWindow *
SoWxGraphEditor::buildWidget(// virtual, protected
        wxWindow * parent)
{
    treeCtrl = new wxTreeCtrl(parent);

    /* TODO:
  this->editorbase = WX_WIDGET(ww_vbox_new(FALSE, 0));
  if (this->buildflags & MENUBAR) {
    this->menubar = this->buildMenuBarWidget(this->editorbase);
    ww_widget_set_usize(WX_WIDGET(this->menubar), 0, 30);
    ww_widget_show(this->menubar);
    ww_box_pack_start(WX_BOX(this->editorbase), WX_WIDGET(this->menubar), FALSE, TRUE, 0);
  }
  if (this->buildflags & GRAPHEDITOR) {
    this->grapheditor = this->buildGraphEditorWidget(this->editorbase);
    ww_widget_show(this->grapheditor);
    ww_box_pack_start(WX_BOX(this->editorbase), WX_WIDGET(this->grapheditor), TRUE, TRUE, 0);
  }
  if (this->buildflags & STATUSBAR) {
    this->statusbar = this->buildStatusBarWidget(this->editorbase);
    ww_widget_set_usize(WX_WIDGET(this->statusbar), 0, 30);
    ww_widget_show(this->statusbar);
    ww_box_pack_end(WX_BOX(this->editorbase), WX_WIDGET(this->statusbar), FALSE, TRUE, 0);
  }

  return this->editorbase;
     */
    return (treeCtrl);
} // buildWidget()

/*!
  This function builds and returns the graph editor menu bar.
*/

wxWindow *
SoWxGraphEditor::buildMenuBarWidget(// virtual, protected
        wxWindow * parent)
{
    /* TODO
  wxWindow * menubar = WX_WIDGET(ww_menu_bar_new());
  wxWindow * filemenuitem = ww_menu_item_new_with_label(_("File"));
  ww_widget_show(filemenuitem);
  ww_menu_bar_append(WX_MENU_BAR(menubar), WX_WIDGET(filemenuitem));
  wxWindow * filemenu = WX_WIDGET(ww_menu_new());
  ww_widget_show(filemenu);
  wxWindow * saveitem = ww_menu_item_new_with_label(_("Save"));
  ww_widget_show(saveitem);
  ww_menu_append(WX_MENU(filemenu), WX_WIDGET(saveitem));
  ww_signal_connect(WX_OBJECT(saveitem), "activate",
    WX_SIGNAL_FUNC(SoWxGraphEditor::saveCB), (void*) this);
  wxWindow * closeitem = ww_menu_item_new_with_label(_("Close"));
  ww_widget_show(closeitem);
  ww_menu_append(WX_MENU(filemenu), WX_WIDGET(closeitem));
  ww_signal_connect(WX_OBJECT(closeitem), "activate",
    WX_SIGNAL_FUNC(SoWxGraphEditor::closeCB), (void*) this);
  ww_menu_item_set_submenu(WX_MENU_ITEM(filemenuitem), WX_WIDGET(filemenu));
  return menubar;
     */
    return 0;
} // buildMenuBarWidget()

/*!
  This function builds and returns the actual graph editor widget.
*/

wxWindow *
SoWxGraphEditor::buildGraphEditorWidget(// virtual, protected
        wxWindow * parent)
{
#if 0
    // TODO
  /* value, lower, upper, step_increment, page_increment, page_size */
  this->vertical = WX_ADJUSTMENT(ww_adjustment_new(0, 0, 1000, 10, 100, 100));
  this->horizontal = WX_ADJUSTMENT(ww_adjustment_new(0, 0, 1000, 10, 100, 1000));

  wxWindow * editor = WX_WIDGET(ww_scrolled_window_new(horizontal, vertical));
  ww_scrolled_window_set_policy(
    WX_SCROLLED_WINDOW(editor), WX_POLICY_NEVER, WX_POLICY_ALWAYS);

  this->graphroot = WX_WIDGET(ww_tree_new());
  ww_widget_show(this->graphroot);
  ww_scrolled_window_add_with_viewport(WX_SCROLLED_WINDOW(editor), this->graphroot);

  return editor;
#endif
    return 0;
} // buildGraphEditorWidget()

/*!
  This function builds and returns the graph editor status bar.
*/

wxWindow *
SoWxGraphEditor::buildStatusBarWidget(// virtual, protected
        wxWindow * parent)
{
#if 0
    // TODO
  wxWindow * statusframe = WX_WIDGET(ww_frame_new((gchar *) NULL));
  ww_widget_set_usize(statusframe, 0, 30);
  ww_frame_set_shadow_type(WX_FRAME(statusframe), WX_SHADOW_ETCHED_IN);
  ww_container_set_border_width(WX_CONTAINER(statusframe), 3);
  
  wxWindow * statuswidgets = ww_hbox_new(FALSE, 0);
  ww_widget_set_usize(statusframe, 0, 30);
  ww_container_add(WX_CONTAINER(statusframe), statuswidgets);
  ww_widget_show(statuswidgets);

  this->statusmessage = WX_WIDGET(ww_label_new((gchar *) NULL));
  ww_label_set_justify(WX_LABEL(this->statusmessage), WX_JUSTIFY_LEFT);
  ww_widget_show(this->statusmessage);
  ww_box_pack_start(WX_BOX(statuswidgets), this->statusmessage, FALSE, TRUE, 3);

  return statusframe;
#endif
    return (0);
} // buildStatusBarWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::nodeSelection(SoNode * node)
{
    this->setStatusMessage(node->getTypeId().getName().getString());
} // nodeSelection()

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::fieldSelection(
        SoNode * node,
        SoField * field)
{
    this->setStatusMessage(field->getTypeId().getName().getString());
} // fieldSelection()

// *************************************************************************

// doc in super
void
SoWxGraphEditor::sizeChanged(const SbVec2s & size)
{
    // inherited::sizeChanged(size);
} // sizeChanged()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::saveCB(// static, private
        wxWindow * obj,
        void* closure)
{
    assert(closure != NULL);
    SoWxGraphEditor * editor = (SoWxGraphEditor *) closure;
    editor->saveSceneGraph();
} // saveCB()

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::closeCB(// static, private
        wxWindow * obj,
        void* closure)
{
    assert(closure != NULL);
    SoWxGraphEditor * editor = (SoWxGraphEditor *) closure;
    editor->hide();
} // closeCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::setStatusMessage(// virtual, protected
        const char * message)
{
#if SOWX_DEBUG
    SoDebugError::postInfo("SoWxGraphEditor::selectionCB", message);
#endif
    // TODO: ww_label_set_text(WX_LABEL(this->statusmessage), message);
} // setStatusMessage()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWxGraphEditor::selectionCB(wxTreeEvent& event)
{
    assert(treeCtrl != NULL);
    wxSoData * soData = dynamic_cast<wxSoData*>(treeCtrl->GetItemData(event.GetItem()));
    if(soData) {
        switch (soData->type) {
            case wxSoData::FIELD: {
                SoDebugError::postInfo("SoWxGraphEditor::selectionCB", "node and field");
                assert(soData->field);
                fieldSelection(soData->node, soData->field);
            }
                break;
            case wxSoData::NODE: {
                SoDebugError::postInfo("SoWxGraphEditor::selectionCB", "node");
                nodeSelection(soData->node);

            }
                break;
            default:
                SoDebugError::postInfo("SoWxGraphEditor::selectionCB", "[unknown data]");
                break;
        }
    } else {
        SoDebugError::postInfo("SoWxGraphEditor::selectionCB", "[invalid selection data]");
    }
} // selectionCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoWxGraphEditor::getDefaultWidgetName(// virtual, protected
        void) const
{
    static const char defaultWidgetName[] = "SoWxGraphEditor";
    return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoWxGraphEditor::getDefaultTitle(// virtual, protected
        void) const
{
    static const char defaultTitle[] = N_("Graph Editor");
    return _(defaultTitle);
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoWxGraphEditor::getDefaultIconTitle(// virtual, protected
        void) const
{
    static const char defaultIconTitle[] = N_("Graph Editor");
    return _(defaultIconTitle);
} // getDefaultIconTitle()

// *************************************************************************
