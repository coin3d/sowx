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

#include "Inventor/Wx/SoWxInternal.h"

#include <sstream>


std::string
nameToString(const wxWindow* w) {
    std::ostringstream  oss;
    oss<<"name:\""<<w->GetName()<<"\"";
    oss<<" ptr:"<<std::hex<<w;
    return (oss.str());
}

std::string
sizeToString(const wxSize& s) {
    std::ostringstream  oss;
    oss<<"size:("<<s.GetX()<<","<<s.GetY()<<")";
    return (oss.str());
}

std::string
dumpSizer(wxSizer* sizer,
          const std::string& prefix) {
    std::string out;
    out += prefix;
    out += " -> sizer:";
    out += sizeToString(sizer->GetSize());
    return (out);
}

std::string
dumpData(const wxWindow* w,
         const std::string& prefix="") {
    std::string out;
    out += prefix;
    out += nameToString(w);
    out += " ";
    out += sizeToString(w->GetSize());
    out += " minClientSize:";
    out += sizeToString(w->GetMinClientSize());
    if(w->GetSizer()) {
        out += dumpSizer(w->GetSizer(), prefix);
#if 0
        wxSizerItemList list = w->GetSizer()->GetChildren();
        wxSizerItemList::compatibility_iterator node = list.GetFirst();
        while(node) {
            if(node->GetData() && node->GetData()->GetSizer())
                dumpSizer(node->GetData()->GetSizer(), prefix);
           // if(node->GetData() && node->GetData()->GetWindow())
            //    dumpData(node->GetData()->GetWindow(), prefix);
            node = node->GetNext();
        }
#endif
    }
    return (out);
}

std::string
dumpWindowDataImpl(const wxWindow* window, int level) {
    if(window == 0) {
        return ("windows is null\n");
    }

    std::string out;
    std::string tabs;
    for(int i=0;i<level;++i)
        tabs+='-';

    out += "\n";
    out += dumpData(window, tabs);

    const wxWindowList& windows_list =  window->GetWindowChildren();
    wxWindowList::compatibility_iterator node = windows_list.GetFirst();
    while (node) {
        wxWindow *win = node->GetData();
        out += dumpWindowDataImpl(win, level+1);
        node = node->GetNext();
    }
    return (out);
}

std::string
dumpWindowData(const wxWindow* window) {
    return (dumpWindowDataImpl(window, 0));
}
