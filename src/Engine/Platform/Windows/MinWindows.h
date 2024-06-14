/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#if defined(_WINDOWS_) && !defined(OSRE_MINIMAL_WINDOWS_INCLUDE)
#   pragma message ( " " )
#   pragma message ( "You have included windows.h before MinWindows.h" )
#   pragma message ( "All useless stuff from the windows headers won't be excluded !!!" )
#   pragma message ( " " )
#endif // _WINDOWS_

#define OSRE_MINIMAL_WINDOWS_INCLUDE

// WIN32_LEAN_AND_MEAN excludes rarely-used services from windows headers.
#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

// The below excludes some other unused services from the windows headers -- see windows.h for details.
#define NOGDICAPMASKS			// CC_*, LC_*, PC_*, CP_*, TC_*, RC_
//#define NOVIRTUALKEYCODES		// VK_*
//#define NOWINMESSAGES			// WM_*, EM_*, LB_*, CB_*
//#define NOWINSTYLES			// WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
//#define NOSYSMETRICS			// SM_*
// #define NOMENUS				// MF_*
//#define NOICONS				// IDI_*
//#define NOKEYSTATES			// MK_*
//#define NOSYSCOMMANDS			// SC_*
//#define NORASTEROPS			// Binary and Tertiary raster ops
//#define NOSHOWWINDOW			// SW_*
#define OEMRESOURCE				// OEM Resource values
#define NOATOM					// Atom Manager routines
//#define NOCLIPBOARD			// Clipboard routines
//#define NOCOLOR				// Screen colors
//#define NOCTLMGR				// Control and Dialog routines
#define NODRAWTEXT				// DrawText() and DT_*
//#define NOGDI					// All GDI #defines and routines
#define NOKERNEL				// All KERNEL #defines and routines
//#define NOUSER				// All USER #defines and routines
//#define NONLS					// All NLS #defines and routines
//#define NOMB					// MB_* and MessageBox()
#define NOMEMMGR				// GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE				// typedef METAFILEPICT
#define NOMINMAX				// Macros min(a,b) and max(a,b)
//#define NOMSG					// typedef MSG and associated routines
#define NOOPENFILE				// OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL				// SB_* and scrolling routines
#define NOSERVICE				// All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND					// Sound driver routines
//#define NOTEXTMETRIC			// typedef TEXTMETRIC and associated routines
//#define NOWH					// SetWindowsHook and WH_*
//#define NOWINOFFSETS			// GWL_*, GCL_*, associated routines
#define NOCOMM					// COMM driver routines
#define NOKANJI					// Kanji support stuff.
#define NOHELP					// Help engine interface.
#define NOPROFILER				// Profiler interface.
#define NODEFERWINDOWPOS		// DeferWindowPos routines
#define NOMCX					// Modem Configuration Extensions
#define NOCRYPT
#define NOTAPE
#define NOIMAGE
#define NOPROXYSTUB
#define NORPC

struct IUnknown;

// Finally now we can include windows.h
#include <intrin.h>
#include <windows.h>
