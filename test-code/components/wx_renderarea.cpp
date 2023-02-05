/*
 * wxCoin3D Porting on wxWidgets of Coin3D (a.k.a Open Inventor) examples
 * Copyright (C) 2022  Fabrizio Morciano

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/SoWxRenderArea.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>

#include "wx/wx.h"

#include "common/get_scene_graph.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit()  {
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

        SoWxRenderArea * renderarea =
                new SoWxRenderArea(window, "Renderarea demonstration");
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


