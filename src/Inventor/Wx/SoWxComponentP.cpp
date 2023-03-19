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

#include <sstream>
#include "Inventor/Wx/SoWxComponentP.h"
#include "sowxdefs.h"
#include "Inventor/Wx/SoAny.h"

#define PRIVATE(obj) (obj)
#define PUBLIC(obj) ((obj)->pub)

SbDict * SoWxComponentP::cursordict = NULL;

SoWxComponentP::SoWxComponentP(SoWxComponent *o)
        : SoGuiComponentP(o)
        , classname("")
        , widgetname("") {

}

SoWxComponentP::~SoWxComponentP() {

}

static void
#ifdef HAVE_OIV
delete_dict_value(unsigned long /*key*/, void * value) {
#else
delete_dict_value(SbDictKeyType /*key*/, void* value) {
#endif
    delete (wxCursor*)value;
}

void
SoWxComponentP::atexit_cleanup() {
    if (SoWxComponentP::cursordict) {
        SoWxComponentP::cursordict->applyToAll(delete_dict_value);
        delete SoWxComponentP::cursordict;
        SoWxComponentP::cursordict = NULL;
    }
}

void
SoWxComponentP::fatalerrorHandler(void *userdata) {
    SoWxComponentP * that = (SoWxComponentP *)userdata;
    (void*)(that); // unused for the time being
}

void
SoWxComponentP::widgetClosed(wxEvent&) {
    if (this->closeCB) {
        this->closeCB(this->closeCBdata, PUBLIC(this));
    }
}

wxCursor *
SoWxComponentP::getNativeCursor(const SoWxCursor::CustomCursor *cc) {
    if (SoWxComponentP::cursordict == NULL) { // first call, initialize
        SoWxComponentP::cursordict = new SbDict;
        SoAny::atexit((SoAny::atexit_f*)SoWxComponentP::atexit_cleanup, 0);
    }

    void * qc;
    SbBool b = SoWxComponentP::cursordict->find((uintptr_t)cc, qc);
    if (b) {
        return (wxCursor *)qc;
    }

    wxCursor* down_cursor = nullptr;
    static int counter = 0;

#ifdef __WXMSW__
    wxBitmap down_bitmap((const char *)cc->bitmap, cc->dim[0], cc->dim[1] );
    /*
    {
        std::ostringstream oss;
        oss << "raw_1_"<<counter << "_raw.jpg";
        down_bitmap.SaveFile(oss.str(), wxBITMAP_TYPE_JPEG);
    }
     */
    wxBitmap down_mask_bitmap((const char*)cc->mask, cc->dim[0], cc->dim[1]);
/*
    {
        std::ostringstream oss;
        oss << "raw_2_"<< counter << "_raw.jpg";
        down_mask_bitmap.SaveFile(oss.str(), wxBITMAP_TYPE_JPEG);
    }
*/
    down_bitmap.SetMask(new wxMask(down_mask_bitmap));
    wxImage down_image = down_bitmap.ConvertToImage();
    down_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 6);
    down_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 14);
  /*
    {
        std::ostringstream oss;
        oss << "raw_3_"<< counter << "_raw.jpg";
        down_image.SaveFile(oss.str(), wxBITMAP_TYPE_JPEG);
    }
    */
    down_cursor = new wxCursor(down_image);
#elif defined(__WXGTK__) or defined(__WXMOTIF__) or defined(__WXQT__)
    down_cursor = new wxCursor(reinterpret_cast<const char *>(cc->bitmap), cc->dim[0], cc->dim[1], -1, -1,
                                    reinterpret_cast<const char *>(cc->mask), wxWHITE, wxBLACK);
#else
#warning "To be tested"
#endif
    SoWxComponentP::cursordict->enter((uintptr_t)cc, down_cursor);
    return (down_cursor);
}

#undef PRIVATE
#undef PUBLIC

