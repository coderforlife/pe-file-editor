// pe-file-editor: wrapper for many pe-file functions in a .NET library
// Copyright (C) 2012  Jeffrey Bush  jeff@coderforlife.com
//
// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

// Types and Utilities for making C++/CLR more like C#
typedef System::String ^string;
typedef System::Object ^object;

typedef System::Byte byte;
typedef System::UInt16 ushort;
typedef System::UInt32 uint;
typedef System::UInt64 ulong;

typedef System::SByte sbyte;
//typedef System::Int16 short; // same
//typedef System::Int32 int;   // same
typedef System::Int64 slong;   // supposed to be "long" but that conflicts with C++ long

//typedef System::Single float;  // same
//typedef System::Double double; // same
typedef System::Decimal decimal;

//typedef System::Char char; // in C++ this is wchar_t
//typedef System::Boolean bool;  // same
//typedef System::Void void;     // same



// Interop
template<typename T> __forceinline T *as_native(array<T> ^a) { pin_ptr<T> p = &a[0]; return p; }
#define NATIVE(a) as_native(a), a->Length
template<typename T> __forceinline
static array<T> ^to_managed(T *a, size_t l) { array<T> ^m = gcnew array<T>(l); System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)a, m, 0, l); return m; }

#include <vcclr.h>
__forceinline const wchar_t *as_native(System::String ^s) { pin_ptr<const wchar_t> p = PtrToStringChars(s); return p; }
__forceinline System::String ^as_managed(wchar_t *s) { return System::Runtime::InteropServices::Marshal::PtrToStringUni((System::IntPtr)s); }

template<typename T> __forceinline T *ptr(T %a) { pin_ptr<T> p = &a; return p; }
#define dwptr(a) (uint32_t*)ptr(a)
#define wptr(a) (uint16_t*)ptr(a)
