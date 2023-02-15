//
// Created by fmorciano on 2/14/23.
//

#ifndef MYVIEWER_H
#define MYVIEWER_H

#include "Inventor/Wx/viewers/SoWxFullViewer.h"

class MyViewer : public SoWxFullViewer {
public:
    MyViewer(wxWindow* window) :
            SoWxFullViewer(window,
                           "Renderarea demonstration",
                           FALSE,
                           SoWxFullViewer::BUILD_ALL,
                           SoWxViewer::BROWSER,
                           FALSE) {

    }

    virtual wxWindow* buildLeftTrim(wxWindow* parent) {
        return SoWxFullViewer::buildLeftTrim(parent);
    }
    virtual wxWindow* buildBottomTrim(wxWindow* parent) {
        return SoWxFullViewer::buildBottomTrim(parent);
    }
    virtual wxWindow* buildRightTrim(wxWindow* parent) {
        return SoWxFullViewer::buildRightTrim(parent);
    }

};
#endif //MYVIEWER_H
