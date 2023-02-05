// This example shows how to put a permanent background image on
// your viewer canvas, below the 3D graphics, plus overlay
// foreground geometry.  Written by mortene.
// Copyright Kongsberg Oil & Gas Technologies 2002.

// *************************************************************************

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/viewers/SoWxExaminerViewer.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>

#include <GL/gl.h>

// *************************************************************************

class MyExaminerViewer : public SoWxExaminerViewer {

public:
    MyExaminerViewer(wxWindow* parent, const char * filename);
    ~MyExaminerViewer();

protected:
    virtual void actualRedraw(void);

private:
    SoSeparator * bckgroundroot;
    SoSeparator * foregroundroot;
    SoRotationXYZ * arrowrotation;
};

MyExaminerViewer::MyExaminerViewer(wxWindow* parent, const char * filename)
        : SoWxExaminerViewer(parent)
{
    // Coin should not clear the pixel-buffer, so the background image
    // is not removed.
    this->setClearBeforeRender(FALSE, TRUE);


    // Set up background scene graph with image in it.

    this->bckgroundroot = new SoSeparator;
    this->bckgroundroot->ref();

    SoOrthographicCamera * cam = new SoOrthographicCamera;
    cam->position = SbVec3f(0, 0, 1);
    cam->height = 1;
    // SoImage will be at z==0.0.
    cam->nearDistance = 0.5;
    cam->farDistance = 1.5;

    SoImage * img = new SoImage;
    img->vertAlignment = SoImage::HALF;
    img->horAlignment = SoImage::CENTER;
    img->filename = filename;

    this->bckgroundroot->addChild(cam);
    this->bckgroundroot->addChild(img);

    // Set up foreground, overlaid scene graph.

    this->foregroundroot = new SoSeparator;
    this->foregroundroot->ref();

    SoLightModel * lm = new SoLightModel;
    lm->model = SoLightModel::BASE_COLOR;

    SoBaseColor * bc = new SoBaseColor;
    bc->rgb = SbColor(1, 1, 0);

    cam = new SoOrthographicCamera;
    cam->position = SbVec3f(0, 0, 5);
    cam->height = 10;
    cam->nearDistance = 0;
    cam->farDistance = 10;

    const double ARROWSIZE = 2.0;

    SoTranslation * posit = new SoTranslation;
    posit->translation = SbVec3f(-2.5 * ARROWSIZE, 1.5 * ARROWSIZE, 0);

    arrowrotation = new SoRotationXYZ;
    arrowrotation->axis = SoRotationXYZ::Z;

    SoTranslation * offset = new SoTranslation;
    offset->translation = SbVec3f(ARROWSIZE/2.0, 0, 0);

    SoCube * cube = new SoCube;
    cube->width = ARROWSIZE;
    cube->height = ARROWSIZE/15.0;

    this->foregroundroot->addChild(cam);
    this->foregroundroot->addChild(lm);
    this->foregroundroot->addChild(bc);
    this->foregroundroot->addChild(posit);
    this->foregroundroot->addChild(arrowrotation);
    this->foregroundroot->addChild(offset);
    this->foregroundroot->addChild(cube);
}

MyExaminerViewer::~MyExaminerViewer()
{
    this->bckgroundroot->unref();
    this->foregroundroot->unref();
}

void
MyExaminerViewer::actualRedraw(void)
{
    // Must set up the OpenGL viewport manually, as upon resize
    // operations, Coin won't set it up until the SoGLRenderAction is
    // applied again. And since we need to do glClear() before applying
    // the action..
    const SbViewportRegion vp = this->getViewportRegion();
    SbVec2s origin = vp.getViewportOriginPixels();
    SbVec2s size = vp.getViewportSizePixels();
    glViewport(origin[0], origin[1], size[0], size[1]);

    const SbColor col = this->getBackgroundColor();
    glClearColor(col[0], col[1], col[2], 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render our scene graph with the image.
    SoGLRenderAction * glra = this->getGLRenderAction();
    glra->apply(this->bckgroundroot);


    // Render normal scene graph.
    SoWxExaminerViewer::actualRedraw();


    // Increase arrow angle with 1/1000 degrees every frame.
    arrowrotation->angle = arrowrotation->angle.getValue() + (0.001 / M_PI * 180);
    // Render overlay front scene graph.
    glClear(GL_DEPTH_BUFFER_BIT);
    glra->apply(this->foregroundroot);
}

// *************************************************************************

int
main(int argc, char ** argv)
{
    if (argc != 2) {
        (void)fprintf(stderr, "\n\n\tUsage: %s <image-filename>\n\n", argv[0]);
        exit(1);
    }

    wxWindow* window = SoWx::init(argv[0]);

    MyExaminerViewer * viewer = new MyExaminerViewer(window, argv[1]);

    viewer->setSceneGraph(new SoCone);
    viewer->show();

    SoWx::show(window);
    SoWx::mainLoop();

    delete viewer;
    return 0;
}