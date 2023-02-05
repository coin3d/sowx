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

#include "SoWxKeyboardP.h"

SbDict * SoWxKeyboardP::translatetable = NULL;

struct SoWxKeyboardP::key1map SoWxKeyboardP::wxToSoMapping[] = {
        {WXK_ESCAPE, SoKeyboardEvent::ESCAPE, '.'},
        {WXK_TAB, SoKeyboardEvent::TAB, '.'},
        {WXK_BACK, SoKeyboardEvent::BACKSPACE, '.'},
        {WXK_RETURN, SoKeyboardEvent::RETURN, '.'},
        {WXK_NUMPAD_ENTER, SoKeyboardEvent::ENTER, '.'},
        {WXK_INSERT, SoKeyboardEvent::INSERT, '.'},
#ifdef HAVE_SOKEYBOARDEVENT_DELETE
        {WXK_DELETE, SoKeyboardEvent::DELETE, '.'},
#else
        {WXK_DELETE, SoKeyboardEvent::KEY_DELETE, '.'},
#endif
        {WXK_PAUSE, SoKeyboardEvent::PAUSE, '.'},
        {WXK_PRINT, SoKeyboardEvent::PRINT, '.'},
        {WXK_HOME, SoKeyboardEvent::HOME, '.'},
        {WXK_END, SoKeyboardEvent::END, '.'},
        {WXK_LEFT, SoKeyboardEvent::LEFT_ARROW, '.'},
        {WXK_UP, SoKeyboardEvent::UP_ARROW, '.'},
        {WXK_RIGHT, SoKeyboardEvent::RIGHT_ARROW, '.'},
        {WXK_DOWN, SoKeyboardEvent::DOWN_ARROW, '.'},
        {WXK_PAGEUP, SoKeyboardEvent::PRIOR, '.'},
        {WXK_PAGEUP, SoKeyboardEvent::PAGE_UP, '.'},
        {WXK_PAGEDOWN, SoKeyboardEvent::NEXT, '.'},
        {WXK_PAGEDOWN, SoKeyboardEvent::PAGE_DOWN, '.'},

        {WXK_SHIFT, SoKeyboardEvent::LEFT_SHIFT, '.'},
        {WXK_CONTROL, SoKeyboardEvent::LEFT_CONTROL, '.'},
        {WXK_ALT, SoKeyboardEvent::LEFT_ALT, '.'},
        // TODO: {WXK_CapsLock, SoKeyboardEvent::CAPS_LOCK, '.'},
        {WXK_NUMLOCK, SoKeyboardEvent::NUM_LOCK, '.'},
        {WXK_SCROLL, SoKeyboardEvent::SCROLL_LOCK, '.'},

        {WXK_F1, SoKeyboardEvent::F1, '.'},
        {WXK_F2, SoKeyboardEvent::F2, '.'},
        {WXK_F3, SoKeyboardEvent::F3, '.'},
        {WXK_F4, SoKeyboardEvent::F4, '.'},
        {WXK_F5, SoKeyboardEvent::F5, '.'},
        {WXK_F6, SoKeyboardEvent::F6, '.'},
        {WXK_F7, SoKeyboardEvent::F7, '.'},
        {WXK_F8, SoKeyboardEvent::F8, '.'},
        {WXK_F9, SoKeyboardEvent::F9, '.'},
        {WXK_F10, SoKeyboardEvent::F10, '.'},
        {WXK_F11, SoKeyboardEvent::F11, '.'},
        {WXK_F12, SoKeyboardEvent::F12, '.'},
        {WXK_SPACE, SoKeyboardEvent::SPACE, ' '},
#if 0
        {WXK_Exclam, SoKeyboardEvent::NUMBER_1, '!'},
        {WXK_QuoteDbl, SoKeyboardEvent::APOSTROPHE, '\"'},
        {WXK_NumberSign, SoKeyboardEvent::NUMBER_3, '#'},
        {WXK_Dollar, SoKeyboardEvent::NUMBER_4, '$'},
        {WXK_Percent, SoKeyboardEvent::NUMBER_5, '%'},
        {WXK_Ampersand, SoKeyboardEvent::NUMBER_6, '^'},
        {WXK_Apostrophe, SoKeyboardEvent::APOSTROPHE, '\''},
        {WXK_ParenLeft, SoKeyboardEvent::NUMBER_9, '('},
        {WXK_ParenRight, SoKeyboardEvent::NUMBER_0, ')'},
        {WXK_Asterisk, SoKeyboardEvent::NUMBER_8, '*'},
        {WXK_Plus, SoKeyboardEvent::EQUAL, '+'},
        {WXK_Comma, SoKeyboardEvent::COMMA, ','},
        {WXK_Minus, SoKeyboardEvent::MINUS, '-'},
        {WXK_Period, SoKeyboardEvent::PERIOD, '.'},
        {WXK_Slash, SoKeyboardEvent::SLASH, '/'},
        {WXK_0, SoKeyboardEvent::NUMBER_0, '0'},
        {WXK_1, SoKeyboardEvent::NUMBER_1, '1'},
        {WXK_2, SoKeyboardEvent::NUMBER_2, '2'},
        {WXK_3, SoKeyboardEvent::NUMBER_3, '3'},
        {WXK_4, SoKeyboardEvent::NUMBER_4, '4'},
        {WXK_5, SoKeyboardEvent::NUMBER_5, '5'},
        {WXK_6, SoKeyboardEvent::NUMBER_6, '6'},
        {WXK_7, SoKeyboardEvent::NUMBER_7, '7'},
        {WXK_8, SoKeyboardEvent::NUMBER_8, '8'},
        {WXK_9, SoKeyboardEvent::NUMBER_9, '9'},
        {WXK_Colon, SoKeyboardEvent::SEMICOLON, ':'},
        {WXK_Semicolon, SoKeyboardEvent::SEMICOLON, ';'},
        {WXK_Less, SoKeyboardEvent::COMMA, '<'},
        {WXK_Equal, SoKeyboardEvent::EQUAL, '='},
        {WXK_Greater, SoKeyboardEvent::PERIOD, '>'},
        {WXK_Question, SoKeyboardEvent::BACKSLASH, '?'},
        {WXK_At, SoKeyboardEvent::NUMBER_2, '@'},

        // zero means let SoKeyboardEvent handle the printable character
        {WXK_A, SoKeyboardEvent::A, 0},
        {WXK_B, SoKeyboardEvent::B, 0},
        {WXK_C, SoKeyboardEvent::C, 0},
        {WXK_D, SoKeyboardEvent::D, 0},
        {WXK_E, SoKeyboardEvent::E, 0},
        {WXK_F, SoKeyboardEvent::F, 0},
        {WXK_G, SoKeyboardEvent::G, 0},
        {WXK_H, SoKeyboardEvent::H, 0},
        {WXK_I, SoKeyboardEvent::I, 0},
        {WXK_J, SoKeyboardEvent::J, 0},
        {WXK_K, SoKeyboardEvent::K, 0},
        {WXK_L, SoKeyboardEvent::L, 0},
        {WXK_M, SoKeyboardEvent::M, 0},
        {WXK_N, SoKeyboardEvent::N, 0},
        {WXK_O, SoKeyboardEvent::O, 0},
        {WXK_P, SoKeyboardEvent::P, 0},
        {WXK_Q, SoKeyboardEvent::Q, 0},
        {WXK_R, SoKeyboardEvent::R, 0},
        {WXK_S, SoKeyboardEvent::S, 0},
        {WXK_T, SoKeyboardEvent::T, 0},
        {WXK_U, SoKeyboardEvent::U, 0},
        {WXK_V, SoKeyboardEvent::V, 0},
        {WXK_W, SoKeyboardEvent::W, 0},
        {WXK_X, SoKeyboardEvent::X, 0},
        {WXK_Y, SoKeyboardEvent::Y, 0},
        {WXK_Z, SoKeyboardEvent::Z, 0},
        {WXK_BracketLeft, SoKeyboardEvent::BRACKETLEFT, '['},
        {WXK_Backslash, SoKeyboardEvent::BACKSLASH, '\\'},
        {WXK_BracketRight, SoKeyboardEvent::BRACKETRIGHT, ']'},
        {WXK_AsciiCircum, SoKeyboardEvent::NUMBER_7, '&'},
        {WXK_Underscore, SoKeyboardEvent::MINUS, '_'},
        {WXK_BraceLeft, SoKeyboardEvent::BRACKETLEFT, '{'},
        {WXK_Bar, SoKeyboardEvent::BACKSLASH, '|'},
        {WXK_BraceRight, SoKeyboardEvent::BRACKETRIGHT, '}'},
        {WXK_AsciiTilde, SoKeyboardEvent::GRAVE, '~'},
#endif
        {WXK_NONE, SoKeyboardEvent::ANY, 0}
};

void
SoWxKeyboardP::make_translation_table(void)
{
    assert(SoWxKeyboardP::translatetable == NULL);
    SoWxKeyboardP::translatetable = new SbDict;

    int i = 0;
    while ( SoWxKeyboardP::wxToSoMapping[i].from != WXK_NONE) {
        SoWxKeyboardP::translatetable->enter((unsigned long)SoWxKeyboardP::wxToSoMapping[i].from,
                                             (void *)&SoWxKeyboardP::wxToSoMapping[i]);
        i++;
    }
}