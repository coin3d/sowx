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

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE {
        if ( !wxApp::OnInit() )
            return false;
        wxFrame* w = new wxFrame(0, wxID_ANY, "Hello");

#if 1

        wxPropertyGridManager* pgMan = new wxPropertyGridManager(w, wxID_ANY,
                                                                 wxDefaultPosition, wxDefaultSize,
                // These and other similar styles are automatically
                // passed to the embedded wxPropertyGrid.
                                                                 wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|
                                                                 // Include toolbar.
                                                                 wxPG_TOOLBAR |
                                                                 // Include description box.
                                                                 wxPG_DESCRIPTION |
                                                                 // Include compactor.
                                                                 //wxPG_COMPACTOR |
                                                                 // Plus defaults.
                                                                 wxPGMAN_DEFAULT_STYLE
        );
        wxPropertyGridPage* page;
        page = pgMan->AddPage("First Page");
        page->Append( new wxPropertyCategory("Category A1") );
        page->Append( new wxIntProperty("Number",wxPG_LABEL,1) );
        page->Append( new wxColourProperty("Colour",wxPG_LABEL,*wxWHITE) );
        page = pgMan->AddPage("Second Page");
        //page->Append( "Text","wxPG_LABE","(no text)" );
        page->Append( new wxFontProperty("Font",wxPG_LABEL) );
// Display a header above the grid
        pgMan->ShowHeader();
#else
        wxBoxSizer* bSizer2;

        wxPropertyGridManager* m_propertyGridManager1 = new wxPropertyGridManager(w, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE);
        m_propertyGridManager1->SetExtraStyle( wxPG_EX_MODE_BUTTONS );

        wxPropertyGridPage* m_propertyGridPage1 = m_propertyGridManager1->AddPage( wxT("Page1"), wxNullBitmap );
        wxPGProperty *m_propertyGridItem1 = m_propertyGridPage1->Append( new wxStringProperty( wxT("Name1"), wxT("Name1") ) );
        wxPGProperty *m_propertyGridItem2 = m_propertyGridPage1->Append( new wxStringProperty( wxT("Name2"), wxT("Name2") ) );
        wxPGProperty *m_propertyGridItem3 = m_propertyGridPage1->Append( new wxStringProperty( wxT("Name3"), wxT("Name3") ) );

        wxPropertyGridPage*m_propertyGridPage2 = m_propertyGridManager1->AddPage( wxT("Page2"), wxNullBitmap );
        wxPGProperty *m_propertyGridItem12 = m_propertyGridPage2->Append( new wxStringProperty( wxT("Name1"), wxT("Name1") ) );
        wxPGProperty *m_propertyGridItem13 = m_propertyGridPage2->Append( new wxStringProperty( wxT("Name2"), wxT("Name2") ) );
        wxPGProperty *m_propertyGridItem14 = m_propertyGridPage2->Append( new wxStringProperty( wxT("Name3"), wxT("Name3") ) );
        wxPGProperty *m_propertyGridItem15 = m_propertyGridPage2->Append( new wxStringProperty( wxT("Name4"), wxT("Name4") ) );

#endif
        w->Centre( wxBOTH );

        w->Show();
        return true;
    }
};



wxIMPLEMENT_APP(MyApp);
