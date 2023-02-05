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

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/SoWxRenderArea.h>

#define protected public
#include <Inventor/Wx/viewers/SoWxFullViewer.h>
#undef protected

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>

#include "wx/wx.h"

#include "common/get_scene_graph.h"


// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;

        wxWindow* window = SoWx::init("renderarea");

        SoSeparator * root = new SoSeparator;
        root->ref();
        SoPerspectiveCamera * camera;
        root->addChild(camera = new SoPerspectiveCamera);
        root->addChild(new SoDirectionalLight);
        SoSeparator * userroot = get_scene_graph();
        root->addChild(userroot);

        SoWxFullViewer * renderarea =
                new SoWxFullViewer(window,
                                   "Renderarea demonstration",
                                   FALSE,
                                   SoWxFullViewer::BUILD_ALL,
                                   SoWxViewer::BROWSER,
                                   TRUE);
        camera->viewAll( userroot, renderarea->getViewportRegion() );
        renderarea->setSceneGraph(root);
        renderarea->setBackgroundColor(SbColor(0.0f, 0.2f, 0.3f));
        renderarea->show();

        SoWx::show(window);
        // SoWx::mainLoop();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);


