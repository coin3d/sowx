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
delete_dict_value(SbDict::Key key, void * value) {
    delete (wxCursor*)value;
}

void
SoWxComponentP::atexit_cleanup() {
    SOWX_STUB();
    if (SoWxComponentP::cursordict) {
        SoWxComponentP::cursordict->applyToAll(delete_dict_value);
        delete SoWxComponentP::cursordict;
        SoWxComponentP::cursordict = NULL;
    }
}

void
SoWxComponentP::fatalerrorHandler(void *userdata) {
    SOWX_STUB();
    SoWxComponentP * that = (SoWxComponentP *)userdata;
    (void*)(that); // unused for the time being
}

void
SoWxComponentP::widgetClosed(void) {
    SOWX_STUB();
    if (this->closeCB) { this->closeCB(this->closeCBdata, PUBLIC(this)); }
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

#define MAXBITMAPWIDTH 32
#define MAXBITMAPHEIGHT 32
#define MAXBITMAPBYTES (((MAXBITMAPWIDTH + 7) / 8) * MAXBITMAPHEIGHT)

    uint8_t cursorbitmap[MAXBITMAPBYTES];
    uint8_t cursormask[MAXBITMAPBYTES];
    (void)memset(cursorbitmap, 0x00, MAXBITMAPBYTES);
    (void)memset(cursormask, 0x00, MAXBITMAPBYTES);

    if ( !(cc->dim[0] <= MAXBITMAPWIDTH) )
        printf("cursor bitmap width too large: %d\n", cc->dim[0]);
    if ( !(cc->dim[1] <= MAXBITMAPHEIGHT) )
        printf("cursor bitmap height too large: %d\n", cc->dim[1]);
    assert(cc->dim[0] <= MAXBITMAPWIDTH && "internal bitmap too large");
    assert(cc->dim[1] <= MAXBITMAPHEIGHT && "internal bitmap too large");

    const int BYTEWIDTH = (cc->dim[0] + 7) / 8;
    for (int h=0; h < cc->dim[1]; h++) {
        for (int w=0; w < BYTEWIDTH; w++) {
            const int cursorpos = h * ((MAXBITMAPWIDTH + 7) / 8) + w;
            const int nativepos = h * BYTEWIDTH + w;
            cursorbitmap[cursorpos] = cc->bitmap[nativepos];
            cursormask[cursorpos] = cc->mask[nativepos];
        }
    }

#ifdef __WXMSW__
    wxBitmap down_bitmap(down_bits, 32, 32);
    wxBitmap down_mask_bitmap(down_mask, 32, 32);
    down_bitmap.SetMask(new wxMask(down_mask_bitmap));
    wxImage down_image = down_bitmap.ConvertToImage();
    down_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 6);
    down_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 14);
    wxCursor down_cursor = wxCursor(down_image);
#elif defined(__WXGTK__) or defined(__WXMOTIF__) or defined(__WXQT__)
    wxCursor * c = new wxCursor(reinterpret_cast<const char *>(cursorbitmap), 32, 32, 6, 14,
                                    reinterpret_cast<const char *>(cursormask), wxWHITE, wxBLACK);
#else
#error "To be tested"
#endif
    SoWxComponentP::cursordict->enter((uintptr_t)cc, c);
    return c;
}

#undef PRIVATE
#undef PUBLIC

