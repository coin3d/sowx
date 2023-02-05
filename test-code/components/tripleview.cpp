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

/*
  This is just a simple test application to check that we can have
  SoWxComponent derived objects within other widgets.

  It also demonstrates having multiple views on a scene from multiple
  SoWxRenderArea instances.

  Note that this example doesn't work correctly with SoWx + TGS'
  Inventor for some reason. Looks like a TGS' Inventor bug to me. See
  also Bugzilla #20.

  mortene@sim.no
*/

/***********************************************************************/

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/SoWxRenderArea.h>
#include <Inventor/Wx/viewers/SoWxExaminerViewer.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <wx/gbsizer.h>

#include "wx/wx.h"

#include "Inventor/Wx/SoWxInternal.h"

/***********************************************************************/

// Timer callback function will rotate the scene according to the
// current time.
static void
timer_callback(void * data, SoSensor * sensor)
{
    static SbTime t = SbTime::getTimeOfDay().getValue();
    SbTime timediff = SbTime::getTimeOfDay() - t;

    SbRotation rotx(SbVec3f(1, 0, 0), 0.5 * timediff.getValue());
    SbRotation roty(SbVec3f(0, 1, 0), timediff.getValue());
    SbRotation rotz(SbVec3f(0, 0, 1), 1.5 * timediff.getValue());
    SoRotation * scenerotate = (SoRotation *)data;
    scenerotate->rotation.setValue(rotx * roty * rotz);
}

// Make a Wx renderarea as a child widget of viewparent, adding the
// scene under common and a camera with the given orientation.
void
add_view(wxWindow * viewparent, SoGroup * common, SbRotation cameraorientation)
{
    SoSeparator * root = new SoSeparator;

    SoPerspectiveCamera * camera = new SoPerspectiveCamera;
    camera->orientation = cameraorientation;
    root->addChild(camera);

    root->addChild(common);

    std::cerr<<"-----------------------------------\n";
    std::cerr<<dumpWindowData(viewparent)<<std::endl;
    std::cerr<<"-----------------------------------\n";

    SoWxRenderArea * area = new SoWxRenderArea(viewparent);
    // SoWxExaminerViewer * area = new SoWxExaminerViewer(viewparent);
    area->setSceneGraph(root);
    std::cerr<<"-------AFTER-----------------\n";
    std::cerr<<dumpWindowData(viewparent)<<std::endl;
    std::cerr<<"-----------------------------------\n";

#ifndef __COIN__
    // IMPORTANT: make sure each GL context has a unique cache context
  // id.  this is needed for TGS/SGI Inventor. Coin handles this
  // automatically.
  static uint32_t contextcnt = 0;
  area->getGLRenderAction()->setCacheContext(contextcnt++);
#endif // !__COIN__

    camera->viewAll(root, area->getViewportRegion());
}

/***********************************************************************/

// Define a new application type
class MyApp : public wxApp
{
public:

    virtual bool OnInit()  {
        if ( !wxApp::OnInit() )
            return false;

        // Initialize system.

        wxWindow * parent = new wxFrame(0,wxID_ANY, "");
        SoWx::init(parent);

        wxPanel* view0;
        wxPanel* view1;
        wxPanel* view2;

        wxGridBagSizer* gbSizer2;
        gbSizer2 = new wxGridBagSizer( 0, 0 );
        gbSizer2->SetFlexibleDirection( wxBOTH );
        gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

        const int border_size = 5;
        view0 = new wxPanel(parent);
        gbSizer2->Add( view0, wxGBPosition( 0, 0 ), wxGBSpan( 2, 1 ), wxEXPAND | wxALL, border_size );

        view1 = new wxPanel( parent);
        wxBoxSizer* b_s = new wxBoxSizer(wxHORIZONTAL);
        b_s->Add(view1, 1,wxALL | wxEXPAND,0);
        gbSizer2->Add( b_s, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, border_size );

        view2 = new wxPanel( parent);
        wxBoxSizer* b_s1 = new wxBoxSizer(wxHORIZONTAL);
        b_s1->Add(view2, 1,wxALL | wxEXPAND,0);
        gbSizer2->Add( b_s1, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, border_size );

        gbSizer2->AddGrowableRow(0);
        gbSizer2->AddGrowableCol(0);
        gbSizer2->AddGrowableCol(1);
        gbSizer2->AddGrowableRow(1);

        // Construct the common part of the scenegraph.

        SoGroup * commonroot = new SoGroup;
        SoDirectionalLight * light = new SoDirectionalLight;
        light->direction.setValue(-0.5f, -0.5f, -0.8f);
        commonroot->addChild(light);
        SoRotation * scenerotate = new SoRotation;
        commonroot->addChild(scenerotate);

        if (argc == 2) {
            SoInput in;
            in.openFile(argv[1]);
            SoSeparator * fileroot = SoDB::readAll(&in);
            if (!fileroot) exit(1);
            commonroot->addChild(fileroot);
        }
        else {
            SoMaterial * mat = new SoMaterial;
            mat->diffuseColor.setValue(1, 1, 0);
            commonroot->addChild(mat);

            SoCube * cube = new SoCube;
            commonroot->addChild(cube);

            mat = new SoMaterial;
            mat->diffuseColor.setValue(0, 0, 1);
            commonroot->addChild(mat);

            SoTranslation * trans = new SoTranslation;
            trans->translation.setValue(0, 0, 1);
            commonroot->addChild(trans);

            SoSphere * sphere = new SoSphere;
            sphere->radius = 0.5;
            commonroot->addChild(sphere);
        }

        // Add the 3 renderareas.
        add_view(view0, commonroot, SbRotation(SbVec3f(0, 0, 1), 0));
        add_view(view1, commonroot, SbRotation(SbVec3f(0, 1, 0), float(M_PI / 2.0)));
        add_view(view2, commonroot, SbRotation(SbVec3f(1, 0, 0), float(-M_PI / 2.0)));

        // Set up a timer callback to do a simple animation.

        SoTimerSensor ts(timer_callback, scenerotate);
        ts.setInterval(0.02f); // max 50 fps
        ts.schedule();

        // Map window and start event loop.
        parent->SetSizer( gbSizer2 );
        parent->Layout();

        SoWx::show(parent);
        SoWx::mainLoop();
        exit(0);  // TODO: check error on exit!
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
