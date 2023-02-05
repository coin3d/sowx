
#include "Inventor/Wx/widgets/SoWxGLArea.h"

extern bool ConvertWXAttrsWxGLFormat(const int *wxattrs,
                                     SoWxGLArea::GLFormat  &format);

int main() {

    {
        int gl_attr[] = {0};
        SoWxGLArea::GLFormat format;
        ConvertWXAttrsWxGLFormat(gl_attr, format);
    }
    {
        int gl_attr[] = {WX_GL_RGBA, 0};
        SoWxGLArea::GLFormat format;
        ConvertWXAttrsWxGLFormat(gl_attr, format);
    }

    {
        // set DEPTH BUFFER
        int gl_attr[] = {WX_GL_DEPTH_SIZE, 16, 0};
        SoWxGLArea::GLFormat format;
        ConvertWXAttrsWxGLFormat(gl_attr, format);
        (void)(format);
    }

}