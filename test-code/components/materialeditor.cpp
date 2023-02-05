#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotationXYZ.h>

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/SoWxMaterialEditor.h>
#include <Inventor/Wx/viewers/SoWxExaminerViewer.h>

static SoMaterial * material;

// define to 0 or 1
#define WANT_INTERGATED 1

SoSeparator *
makescene(void)
{
  SoSeparator * root = new SoSeparator;
  root->addChild(material = new SoMaterial);
  root->addChild(new SoCube);
  return root;
}

int
main(int argc, char ** argv)
{
  wxWindow* w = SoWx::init(argc, argv, "SoWxMaterialEditor");
  SoWxExaminerViewer * viewer = new SoWxExaminerViewer(w);
  SoSeparator * root;
  viewer->setSceneGraph(root = makescene());
  viewer->show();

#if 0 // WANT_INTEGRATED
  SoSeparator * editorscene = new SoSeparator;
  SoTranslation * trans = new SoTranslation;
  trans->translation.setValue(SbVec3f(2.0f, 0.0f, 0.0f));
  SoRotationXYZ * rot = new SoRotationXYZ;
  SoMaterial * mat = new SoMaterial;
  mat->diffuseColor.setValue(0.8, 0.8, 0.8);
  rot->axis = SoRotationXYZ::Y;
  rot->angle = 0.5;
  editorscene->addChild(trans);
  editorscene->addChild(rot);
  editorscene->addChild(mat);
  SoGuiMaterialEditor * inscene = new SoGuiMaterialEditor;
  // inscene->wysiwyg.setValue(TRUE);
  inscene->material.setValue(material);
  // inscene->color.getValue(); // update field
  // material->diffuseColor.connectFrom(&(inscene->color));
  editorscene->addChild(inscene);
  root->insertChild(editorscene, 0);

#else
  SoWxMaterialEditor * editor = new SoWxMaterialEditor;
  editor->attach(material);
  editor->show();
#endif

  SoWx::show(w);
  SoWx::mainLoop();
  return 0;
}

