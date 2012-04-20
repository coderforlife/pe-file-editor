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
template<typename T>
static array<T> ^to_managed(T *a, size_t l) { array<T> ^m = gcnew array<T>(l); System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)a, m, 0, l); return m; }

#include <vcclr.h>
__forceinline const wchar_t *as_native(System::String ^s) { pin_ptr<const wchar_t> p = PtrToStringChars(s); return p; }
__forceinline System::String ^as_managed(wchar_t *s) { return System::Runtime::InteropServices::Marshal::PtrToStringUni((System::IntPtr)s); }

