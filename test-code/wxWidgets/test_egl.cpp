//
// Created by fmorciano on 1/22/23.
//

#include <wx/wx.h>
#include <wx/glcanvas.h>

class MyCanvas : public wxGLCanvas {
public:
    MyCanvas (wxWindow* parent,
              int * attributes): wxGLCanvas(parent,
        wxID_ANY,
        NULL) {
        wxGLContext ctx(this);
        bool res = wxGLCanvas::IsDisplaySupported(attributes);
        if(res) {
            std::cout<<"EGL is supported!"<<std::endl;
        } else {
            std::cout<<"EGL is not supported!"<<std::endl;
        }
    };
};

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;
        wxFrame* w = new wxFrame(0, wxID_ANY, "Hello");
        int egl [] = {WX_GL_ES2, 0};
        MyCanvas c(w, egl);
        w->Show();
        return true;
    }
};


wxIMPLEMENT_APP(MyApp);