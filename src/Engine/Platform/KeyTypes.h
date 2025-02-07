/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "Platform/PlatformCommon.h"

namespace OSRE {
namespace Platform {
        
///	@brief  This enum type is used to describe a key.
enum Key {
    /// The keyboard sym's have been cleverly chosen to map to ASCII.
    KEY_UNKNOWN = 0,
    KEY_FIRST = 0,
    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_CLEAR = 12,
    KEY_RETURN = 13,
    KEY_PAUSE = 19,
    KEY_ESCAPE = 27,
    KEY_SPACE = 32,
    KEY_EXCLAIM = 33,
    KEY_QUOTEDBL = 34,
    KEY_HASH = 35,
    KEY_DOLLAR = 36,
    KEY_AMPERSAND = 38,
    KEY_QUOTE = 39,
    KEY_LEFTPAREN = 40,
    KEY_RIGHTPAREN = 41,
    KEY_ASTERISK = 42,
    KEY_COMMA = 44,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_COLON = 58,
    KEY_SEMICOLON = 59,
    KEY_LESS = 60,
    KEY_EQUALS = 61,
    KEY_GREATER = 62,
    QUESTION = 63,
    KEY_AT = 64,

    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFTBRACKET = 91,
    KEY_BACKSLASH = 92,
    KEY_RIGHTBRACKET = 93,
    KEY_CARET = 94,
    KEY_UNDERSCommon = 95,
    KEY_BACKQUOTE = 96,
    KEY_a = 97,
    KEY_b = 98,
    KEY_c = 99,
    KEY_d = 100,
    KEY_e = 101,
    KEY_f = 102,
    KEY_g = 103,
    KEY_h = 104,
    KEY_i = 105,
    KEY_j = 106,
    KEY_k = 107,
    KEY_l = 108,
    KEY_m = 109,
    KEY_n = 110,
    KEY_o = 111,
    KEY_p = 112,
    KEY_q = 113,
    KEY_r = 114,
    KEY_s = 115,
    KEY_t = 116,
    KEY_u = 117,
    KEY_v = 118,
    KEY_w = 119,
    KEY_x = 120,
    KEY_y = 121,
    KEY_z = 122,
    KEY_DELETE = 127,
    // End of ASCII mapped key sym's

    KEY_PLUS = 187,
    KEY_MINUS = 189,

    /* Numeric keypad */
    KEY_KP_0 = 256,	///< Numpad 0
    KEY_KP_1 = 257,	///< Numpad 1
    KEY_KP_2 = 258,	///< Numpad 2
    KEY_KP_3 = 259,	///< Numpad 3
    KEY_KP_4 = 260,	///< Numpad 4
    KEY_KP_5 = 261,	///< Numpad 5
    KEY_KP_6 = 262,	///< Numpad 6
    KEY_KP_7 = 263,	///< Numpad 7
    KEY_KP_8 = 264,	///< Numpad 8
    KEY_KP_9 = 265,	///< Numpad 9
    KEY_KP_PERIOD = 266,	///< Numpad ,
    KEY_KP_DIVIDE = 267,	///< Numpad /
    KEY_KP_MULTIPLY = 268,	///< Numpad *
    KEY_KP_MINUS = 269,	///< Numpad -
    KEY_KP_PLUS = 270,	///< Numpad +
    KEY_KP_ENTER = 271,	///< Numpad ENTER
    KEY_KP_EQUALS = 272,	///< Numpad =

    /* Arrows + Home/End pad */
    KEY_UP = 273,
    KEY_DOWN = 274,
    KEY_RIGHT = 275,
    KEY_LEFT = 276,
    KEY_INSERT = 277,
    KEY_HOME = 278,
    KEY_END = 279,
    KEY_PAGEUP = 280,
    KEY_PAGEDOWN = 281,

    /* Function keys */
    KEY_F1 = 282,
    KEY_F2 = 283,
    KEY_F3 = 284,
    KEY_F4 = 285,
    KEY_F5 = 286,
    KEY_F6 = 287,
    KEY_F7 = 288,
    KEY_F8 = 289,
    KEY_F9 = 290,
    KEY_F10 = 291,
    KEY_F11 = 292,
    KEY_F12 = 293,
    KEY_F13 = 294,
    KEY_F14 = 295,
    KEY_F15 = 296,

    /* Key state modifier keys */
    KEY_NUMLOCK = 300,
    KEY_CAPSLOCK = 301,
    KEY_SCROLLOCK = 302,
    KEY_RSHIFT = 303,	///< right shift
    KEY_LSHIFT = 304,	///< left shift
    KEY_RCTRL = 305,	///< right control
    KEY_LCTRL = 306,	///< left control
    KEY_RALT = 307,	///< right alt
    KEY_LALT = 308,	///< left alt
    KEY_RMETA = 309,
    KEY_LMETA = 310,
    KEY_LSUPER = 311,	///< left Windows key
    KEY_RSUPER = 312,	///< right Windows key
    KEY_MODE = 313,	///< alt gr
    KEY_COMPOSE = 314,	/* Multi-key compose key */

    /* Miscellaneous function keys */
    KEY_HELP = 315,
    KEY_PRINT = 316,
    KEY_SYSREQ = 317,
    KEY_BREAK = 318,
    KEY_MENU = 319,
    KEY_POWER = 320,	///< Power Macintosh power key
    KEY_EURO = 321,	///< Some euro keyboards
    KEY_UNDO = 322,	///< Atari keyboard has Undo

    /* Add any other keys here */
    KEY_LAST
};

} // Namespace Platform
} // Namespace OSRE
