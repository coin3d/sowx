/**************************************************************************\
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Fabrizio Morciano
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

/*
  Demonstrates embedding of an SoWxRenderArea within a QScrollView.
*/

/***********************************************************************/

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/SoWxRenderArea.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#include <wx/scrolwin.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyScrollView
///////////////////////////////////////////////////////////////////////////////
class MyScrollView : public wxFrame
{
private:
    SoWxRenderArea * renderarea;
    wxScrolledWindow* scrolledWindow;

public:

    MyScrollView( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL ) : wxFrame( parent, id, title, pos, size, style )
    {
        this->SetSizeHints( wxSize(500,500),
                            wxSize(500,500) );

        wxBoxSizer* sizer;
        sizer = new wxBoxSizer( wxVERTICAL );

        scrolledWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
        scrolledWindow->SetScrollRate( 5, 5 );
        scrolledWindow->SetSize(wxSize(1000,1000));
        sizer->Add( scrolledWindow, 1, wxEXPAND | wxALL, 5 );

        // Construct a simple scenegraph.
        SoSeparator * root = new SoSeparator;

        SoDirectionalLight * light = new SoDirectionalLight;
        light->direction.setValue(-0.5f, -0.5f, -0.8f);
        root->addChild(light);

        SoPerspectiveCamera * camera = new SoPerspectiveCamera;
        camera->orientation = SbRotation(SbVec3f(0, 0, 1), 0);
        root->addChild(camera);

        SoCone * cone = new SoCone;
        root->addChild(cone);

        // Add the renderarea.
        renderarea = new SoWxRenderArea(scrolledWindow);
        renderarea->setSceneGraph(root);
        camera->viewAll(root, renderarea->getViewportRegion());

        this->SetSizer( sizer );
        this->Layout();

        this->Centre( wxBOTH );
    }

    ~MyScrollView()
    {
    }
};

#if 0
class MyScrollView : public QScrollArea {
public:
  MyScrollView(QWidget * parent) : QScrollArea(parent)
    {
#if QT_VERSION < 0x040000 // Wx < 4.0.0
      this->resizeContents(10000, 10000);
      this->enableClipper(TRUE);
#endif

      // Container widget for the SoWxRenderArea.

      QWidget * container = new QWidget(this);
      container->resize(1000, 1000);

      // Construct a simple scenegraph.

      SoSeparator * root = new SoSeparator;

      SoDirectionalLight * light = new SoDirectionalLight;
      light->direction.setValue(-0.5f, -0.5f, -0.8f);
      root->addChild(light);

      SoPerspectiveCamera * camera = new SoPerspectiveCamera;
      camera->orientation = SbRotation(SbVec3f(0, 0, 1), 0);
      root->addChild(camera);

      SoCone * cone = new SoCone;
      root->addChild(cone);

      // Add the renderarea.

      renderarea = new SoWxRenderArea(container);
      renderarea->setSceneGraph(root);
      camera->viewAll(root, renderarea->getViewportRegion());

#if QT_VERSION < 0x040000 // Wx < 4.0.0
      this->addChild(container, 100, 100);
      this->showChild(container);
#else
      this->setWidget(container);
#endif
    }

protected:
  void drawContents(QPainter * p, int cx, int cy, int cw, int ch)
    {
      p->fillRect(cx, cy, cw, ch, QColor(40, 80, 0));
      // FIXME: should redraw only when necessary. 20001127 mortene.
      renderarea->scheduleRedraw();
    }

private:
  SoWxRenderArea * renderarea;
};

#endif

/***********************************************************************/

int
main(int , char ** )
{
  // Initialize Wx and SoWx.
  SoWx::init((wxWindow*)NULL);

  // Set up scrollview window.
  MyScrollView * vp = new MyScrollView(NULL);

  // Map window.
  vp->Show();

  // Start event loop.
  SoWx::mainLoop();

  return 0;
}
