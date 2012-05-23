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

#include "general.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace PE { namespace Editor {
	[Flags] public enum struct FileCharacteristics : ushort
	{
		RelocsStripped			= 0x0001,
		ExecutableImage			= 0x0002,
		LineNumsStripped		= 0x0004,
		LocalSymsStripped		= 0x0008,
		AggressiveWSTrim		= 0x0010,
		LargeAddressAware		= 0x0020,
		// no 0x0040
		BytesReversedLo			= 0x0080,
		Machine32Bit			= 0x0100,
		DebugStripped			= 0x0200,
		RemovableRunFromSwap	= 0x0400,
		NetRunFromSwap			= 0x0800,
		System					= 0x1000,
		DLL						= 0x2000,
		UPSystemOnly			= 0x4000,
		BytesReversedHi			= 0x8000,
	};
	public enum struct Machine : ushort
	{
		Unknown		= 0x0000,
		i386		= 0x014c,
		R3000		= 0x0162, R4000			= 0x0166, R10000	= 0x0168, MIPS_WCE_v2	= 0x0169,
		Alpha		= 0x0184,
		SH3			= 0x01a2, SH3_DSP		= 0x01a3, SH3E		= 0x01a4, SH4			= 0x01a6, SH5			= 0x01a8,
		ARM			= 0x01c0, Thumb			= 0x01c2, AM33		= 0x01d3,
		PowerPC		= 0x01f0, PowerPC_FP	= 0x01f1,
		IA64		= 0x0200,
		MIPS_16		= 0x0266, Alpha64		= 0x0284, MIPS_FPU	= 0x0366, MIPS_FPU_16	= 0x0466, Tricore		= 0x0520, CEF			= 0x0CEF,
		EBC			= 0x0EBC,
		AMD64		= 0x8664,
		M32R		= 0x9041, CEE			= 0xC0EE,
	};
	public enum struct DirectoryEntry : int {
		Export		= 0,
		Import		= 1,
		Resource	= 2,
		Exception	= 3,
		Security	= 4,
		BaseReloc	= 5,
		Debug		= 6,
		ArchitectureSpecific = 7, // on x86 this is Copyright
		GlobalPtr	= 8,
		TLS			= 9,
		LoadConfig	= 10,
		BoundImport	= 11,
		IAT			= 12,
		DelayImport	= 13,
		ComDescriptor = 14,
		Reserved	= 15,
	};
	public enum struct OptionalHeaderMagic : ushort { NT32 = 0x10b, NT64 = 0x20b, ROM = 0x107, };
	public enum struct Subsystem : ushort
	{
		Unknown			= 0,
		Native			= 1,
		WindowsGUI		= 2,
		WindowsCUI		= 3,
		OS2_CUI			= 5, POSIX_CUI = 7, NativeWindows = 8, // no 4 or 6
		WindowsCE_GUI	= 9,
		EFI_Appication	= 10, EFI_BootServiceDriver = 11, EFI_RuntimeDriver = 12, EFI_ROM = 13,
		XBOX			= 14, // no 15
		WindowsBootApplication = 16,
	};
	[Flags] public enum struct DllCharacteristics : ushort {
		_ProcessInit	= 0x0001, // reserved, LIBRARY prefix
		_ProcessTerm	= 0x0002, // reserved, LIBRARY prefix
		_ThreadInit		= 0x0004, // reserved, LIBRARY prefix
		_ThreadTerm		= 0x0008, // reserved, LIBRARY prefix
		DynamicBase		= 0x0040,
		ForceIntegrity	= 0x0080,
		NXCompat		= 0x0100,
		NoIsolation		= 0x0200,
		NoSEH			= 0x0400,
		NoBind			= 0x0800,
		// reserved		= 0x1000,
		WDMDriver		= 0x2000,
		// reserved		= 0x4000,
		TerminalServerAware = 0x8000,
	};
	[Flags] public enum struct SectionCharacteristics : uint
	{
		_TypeReg		= 0x00000000, // reserved
		TLS_ScaleIndex	= 0x00000001, // dup value
		_TypeDSect		= 0x00000001, // reserved, dup value
		_TypeNoLoad		= 0x00000002, // reserved
		_TypeGroup		= 0x00000004, // reserved
		TypeNoPad		= 0x00000008, // reserved
		_TypeCopy		= 0x00000010, // reserved
		ContainsCode	= 0x00000020,
		ContainsInitializedData		= 0x00000040,
		ContainsUninitializedData	= 0x00000080,
		LinkOther		= 0x00000100, // reserved
		LinkInfo		= 0x00000200,
		_TypeOver		= 0x00000400, // reserved
		LinkRemove		= 0x00000800,
		LinkComdat		= 0x00001000,
		// reserved		= 0x00002000,
		NoDeferSpecExc	= 0x00004000, // dup value
		_MemProtected	= 0x00004000, // obsolete, dup value
		GPRelative		= 0x00008000, // dup value
		MemFarData		= 0x00008000, // dup value
		_MemSysHeap		= 0x00010000, // obsolete
		MemPurgeale		= 0x00020000, // dup value
		Mem16Bit		= 0x00020000, // dup value
		MemLocked		= 0x00040000,
		MemPreload		= 0x00080000,
		Align1Bytes		= 0x00100000,
		Align2Bytes		= 0x00200000,
		Align4Bytes		= 0x00300000,
		Align8Bytes		= 0x00400000,
		Align16Bytes	= 0x00500000, // default
		Align32Bytes	= 0x00600000,
		Align64Bytes	= 0x00700000,
		Align128Bytes	= 0x00800000,
		Align256Bytes	= 0x00900000,
		Align512Bytes	= 0x00A00000,
		Align1024Bytes	= 0x00B00000,
		Align2048Bytes	= 0x00C00000,
		Align4096Bytes	= 0x00D00000,
		Align8192Bytes	= 0x00E00000,
		// unused		= 0x00F00000,
		AlignMask		= 0x00F00000,
		LinkRelocOverflow = 0x01000000,
		MemDiscardable	= 0x02000000,
		MemNotCached	= 0x04000000,
		MemNotPaged		= 0x08000000,
		MemShared		= 0x10000000,
		MemExecute		= 0x20000000,
		MemRead			= 0x40000000,
		MemWrite		= 0x80000000,

		// Add special combinations
		CodeSection							= ContainsCode | MemExecute | MemRead,
		InitializedDataSection				= ContainsInitializedData | MemRead,
		WritableInitializedDataSection		= ContainsInitializedData | MemRead | MemWrite,
		DiscardableInitializedDataSection	= ContainsInitializedData | MemRead | MemDiscardable,
	};
	public interface class HexInt : IComparable, IFormattable, IConvertible
	{
		property int Size { int get(); }
		property string DefaultFormat { string get(); }
		property ulong Value { ulong get(); }
	};
	public value struct HexInt32 : HexInt, IComparable<UInt32>, IEquatable<UInt32>, IComparable<HexInt32>, IEquatable<HexInt32>
	{
	private:
		UInt32 value;
	public:
		HexInt32(UInt32 value) { this->value = value; }
		property virtual int Size { int get() { return 4; } }
		property virtual string DefaultFormat { string get() { return L"X8"; } }
		property virtual ulong Value { ulong get() { return this->value; } }
		static operator UInt32(HexInt32 x) { return x.value; }
		static operator HexInt32(UInt32 x) { return HexInt32(x); }
		virtual string ToString() override { return this->value.ToString(L"X8"); }
		string ToString(string format) { return this->value.ToString(String::IsNullOrEmpty(format) ? L"X8" : format); }
		virtual string ToString(IFormatProvider^ formatProvider) { return this->value.ToString(L"X8", formatProvider); }
		virtual string ToString(string format, IFormatProvider^ formatProvider) { return this->value.ToString(String::IsNullOrEmpty(format) ? L"X8" : format, formatProvider); }

		virtual bool Equals(HexInt32 o) { return this->value == o.value; }
		virtual bool Equals(UInt32 o) { return this->value == o; }

		virtual int CompareTo(HexInt32 o) { return this->value.CompareTo(o.value); }
		virtual int CompareTo(UInt32 o) { return this->value.CompareTo(o); }

		virtual int CompareTo(object o) = IComparable::CompareTo
		{
			if (o == nullptr) return 1;
			Type^ t = o->GetType();
			if (HexInt32::typeid->IsAssignableFrom(t))					return this->value.CompareTo(((HexInt32)o).value);
			else if (UInt32::typeid->IsAssignableFrom(t))				return this->value.CompareTo((UInt32)o);
			else if (IComparable<UInt32>::typeid->IsAssignableFrom(t))	return -((IComparable<UInt32>^)o)->CompareTo(this->value);
			else														return this->value.CompareTo(Convert::ToUInt32(o));
		}

		virtual TypeCode GetTypeCode() { return TypeCode::UInt32; }
		virtual bool		ToBoolean (IFormatProvider^ provider) = IConvertible::ToBoolean  { return Convert::ToBoolean (this->value, provider); }
		virtual Byte		ToByte    (IFormatProvider^ provider) = IConvertible::ToByte     { return Convert::ToByte    (this->value, provider); }
		virtual signed char	ToSByte   (IFormatProvider^ provider) = IConvertible::ToSByte    { return Convert::ToSByte   (this->value, provider); }
		virtual Char		ToChar    (IFormatProvider^ provider) = IConvertible::ToChar     { return Convert::ToChar    (this->value, provider); }
		virtual short		ToInt16   (IFormatProvider^ provider) = IConvertible::ToInt16    { return Convert::ToInt16   (this->value, provider); }
		virtual ushort		ToUInt16  (IFormatProvider^ provider) = IConvertible::ToUInt16   { return Convert::ToUInt16  (this->value, provider); }
		virtual int			ToInt32   (IFormatProvider^ provider) = IConvertible::ToInt32    { return Convert::ToInt32   (this->value, provider); }
		virtual uint		ToUInt32  (IFormatProvider^ provider) = IConvertible::ToUInt32   { return Convert::ToUInt32  (this->value, provider); }
		virtual slong		ToInt64   (IFormatProvider^ provider) = IConvertible::ToInt64    { return Convert::ToInt64   (this->value, provider); }
		virtual ulong		ToUInt64  (IFormatProvider^ provider) = IConvertible::ToUInt64   { return Convert::ToUInt64  (this->value, provider); }
		virtual float		ToSingle  (IFormatProvider^ provider) = IConvertible::ToSingle   { return Convert::ToSingle  (this->value, provider); }
		virtual double		ToDouble  (IFormatProvider^ provider) = IConvertible::ToDouble   { return Convert::ToDouble  (this->value, provider); }
		virtual decimal		ToDecimal (IFormatProvider^ provider) = IConvertible::ToDecimal	 { return Convert::ToDecimal (this->value, provider); }
		virtual DateTime	ToDateTime(IFormatProvider^ provider) = IConvertible::ToDateTime { return Convert::ToDateTime(this->value, provider); }
		virtual object ToType(Type^ conversionType, IFormatProvider^ provider) = IConvertible::ToType { return Convert::ChangeType(this->value, conversionType, provider); }
	};

	public value struct HexInt64 : HexInt, IComparable<UInt64>, IEquatable<UInt64>, IComparable<HexInt64>, IEquatable<HexInt64>
	{
	private:
		UInt64 value;
	public:
		HexInt64(UInt64 value) { this->value = value; }
		property virtual int Size { int get() { return 8; } }
		property virtual string DefaultFormat { string get() { return L"X8"; } }
		property virtual ulong Value { ulong get() { return this->value; } }
		static operator UInt64(HexInt64 x) { return x.value; }
		static operator HexInt64(UInt64 x) { return HexInt64(x); }
		virtual string ToString() override { return this->value.ToString(L"X16"); }
		string ToString(string format) { return this->value.ToString(String::IsNullOrEmpty(format) ? L"X16" : format); }
		virtual string ToString(IFormatProvider^ formatProvider) { return this->value.ToString(L"X16", formatProvider); }
		virtual string ToString(string format, IFormatProvider^ formatProvider) { return this->value.ToString(String::IsNullOrEmpty(format) ? L"X16" : format, formatProvider); }

		virtual bool Equals(HexInt64 o) { return this->value == o.value; }
		virtual bool Equals(UInt64 o) { return this->value == o; }

		virtual int CompareTo(HexInt64 o) { return this->value.CompareTo(o.value); }
		virtual int CompareTo(UInt64 o) { return this->value.CompareTo(o); }
		
		virtual int CompareTo(object o) = IComparable::CompareTo
		{
			if (o == nullptr) return 1;
			Type^ t = o->GetType();
			if (HexInt64::typeid->IsAssignableFrom(t))					return this->value.CompareTo(((HexInt64)o).value);
			else if (UInt64::typeid->IsAssignableFrom(t))				return this->value.CompareTo((UInt64)o);
			else if (IComparable<UInt64>::typeid->IsAssignableFrom(t))	return -((IComparable<UInt64>^)o)->CompareTo(this->value);
			else														return this->value.CompareTo(Convert::ToUInt64(o));
		}
		
		virtual TypeCode GetTypeCode() { return TypeCode::UInt64; }
		virtual bool		ToBoolean (IFormatProvider^ provider) = IConvertible::ToBoolean  { return Convert::ToBoolean (this->value, provider); }
		virtual Byte		ToByte    (IFormatProvider^ provider) = IConvertible::ToByte     { return Convert::ToByte    (this->value, provider); }
		virtual signed char	ToSByte   (IFormatProvider^ provider) = IConvertible::ToSByte    { return Convert::ToSByte   (this->value, provider); }
		virtual Char		ToChar    (IFormatProvider^ provider) = IConvertible::ToChar     { return Convert::ToChar    (this->value, provider); }
		virtual short		ToInt16   (IFormatProvider^ provider) = IConvertible::ToInt16    { return Convert::ToInt16   (this->value, provider); }
		virtual ushort		ToUInt16  (IFormatProvider^ provider) = IConvertible::ToUInt16   { return Convert::ToUInt16  (this->value, provider); }
		virtual int			ToInt32   (IFormatProvider^ provider) = IConvertible::ToInt32    { return Convert::ToInt32   (this->value, provider); }
		virtual uint		ToUInt32  (IFormatProvider^ provider) = IConvertible::ToUInt32   { return Convert::ToUInt32  (this->value, provider); }
		virtual slong		ToInt64   (IFormatProvider^ provider) = IConvertible::ToInt64    { return Convert::ToInt64   (this->value, provider); }
		virtual ulong		ToUInt64  (IFormatProvider^ provider) = IConvertible::ToUInt64   { return Convert::ToUInt64  (this->value, provider); }
		virtual float		ToSingle  (IFormatProvider^ provider) = IConvertible::ToSingle   { return Convert::ToSingle  (this->value, provider); }
		virtual double		ToDouble  (IFormatProvider^ provider) = IConvertible::ToDouble   { return Convert::ToDouble  (this->value, provider); }
		virtual decimal		ToDecimal (IFormatProvider^ provider) = IConvertible::ToDecimal	 { return Convert::ToDecimal (this->value, provider); }
		virtual DateTime	ToDateTime(IFormatProvider^ provider) = IConvertible::ToDateTime { return Convert::ToDateTime(this->value, provider); }
		virtual object ToType(Type^ conversionType, IFormatProvider^ provider) = IConvertible::ToType { return Convert::ChangeType(this->value, conversionType, provider); }
	};

	public ref class RawData
	{
	private:
		byte *x;
		uint n;
		bool readOnly;
		void checkGet(uint i, uint n) { if (i+n < i || i+n >= this->n) { throw gcnew ArgumentOutOfRangeException(); } }
		void checkSet(uint i, uint n) { if (this->IsReadOnly) { throw gcnew InvalidOperationException(); } this->checkGet(i, n); }
	internal:
		RawData(File ^f, byte *x, uint n);
		void UpdateMemory(ptrdiff_t dist) { x += dist; }
	public:
		static explicit operator array<byte>^(RawData ^d) { return d->Bytes; }
		property array<byte>^ Bytes { array<byte>^ get() { return to_managed(this->x, this->n); } }
		void Set(array<byte> ^lpBuffer, uint dwOffset)
		{
			this->checkSet(dwOffset, lpBuffer->Length);
			Marshal::Copy(lpBuffer, 0, IntPtr(this->x+dwOffset), lpBuffer->Length);
		}
		void Zero(uint dwSize, uint dwOffset)
		{
			this->checkSet(dwOffset, dwSize);
			memset(this->x + dwOffset, 0, dwSize);
		}
		void Move(uint dwOffset, uint dwSize, int dwDistanceToMove)
		{
			this->checkSet(dwOffset + dwDistanceToMove, dwSize);
			memmove(this->x + dwOffset + dwDistanceToMove, this->x + dwOffset, dwSize);
		}
		void Shift(uint dwOffset, int dwDistanceToMove)
		{
			return this->Move(dwOffset, this->n - dwOffset - dwDistanceToMove, dwDistanceToMove);
		}
		property bool IsReadOnly { bool get() { return this->readOnly; } }
		property IntPtr Pointer { IntPtr get() { return IntPtr(this->x); } }
		property uint Length { uint get() { return this->n; } }
		property byte default[uint]
		{
			byte get(uint i)			{ this->checkGet(i, 1); return this->x[i]; }
			void set(uint i, byte x)	{ this->checkSet(i, 1); this->x[i] = x; }
		}
		property virtual byte default[int]
		{
			byte get(int i)			{ this->checkGet(i, 1); return this->x[i]; }
			void set(int i, byte x)	{ this->checkSet(i, 1); this->x[i] = x; }
		}

		virtual int IndexOf(byte b) {
			void *x = memchr(this->x, b, this->n);
			return x == NULL ? -1 : (int)(((byte*)x)-this->x);
		}
		virtual void CopyTo(array<byte> ^a, int i) {
			if (a == nullptr)					{ throw gcnew ArgumentNullException(); }
			if (i < 0)							{ throw gcnew ArgumentOutOfRangeException(); }
			if (a->Length + (uint)i > this->n)	{ throw gcnew ArgumentException(); }
			Marshal::Copy(IntPtr(this->x), a, i, this->n);
		}
		
		/*property virtual string HexString
		{
			string get()
			{
				StringBuilder ^sb = gcnew StringBuilder(this->n*2);
				for (uint i = 0; i < this->n; ++i)
					sb->AppendFormat(L"{0:X2}",this->x[i]);
				return sb->ToString();
			}
		}*/
		virtual string ToString() override
		{
			StringBuilder ^sb = gcnew StringBuilder(L"{");
			bool trunc = this->n > 10;
			int len = trunc ? 10 : (int)this->n;
			for (int i = 0; i < len; ++i)
				sb->AppendFormat(L"{0:X2}, ",this->x[i]);
			string s = trunc ? sb->ToString()+L"..." : sb->ToString()->TrimEnd(L',', L' ');
			return s+L"}";
		}
	};

	public value struct SectionName
	{
	public:
		static const SectionName TEXT = ".text", RDATA = ".rdata", DATA = ".data", IDATA = ".idata", RSRC = ".rsrc", RELOC = ".reloc";

	private:
		array<byte> ^n;
		void CopyFrom(array<byte> ^s)
		{
			int len = Math::Min(s->Length, LENGTH);
			Array::Copy(s, n, len);
			if (len < LENGTH)
				Array::Clear(n, len, LENGTH - len);
		}
		void CopyFrom(byte *s, int len)
		{
			if (len > LENGTH)
				len = LENGTH;
			System::Runtime::InteropServices::Marshal::Copy(IntPtr(s), n, 0, len);
			if (len < LENGTH)
				Array::Clear(n, len, LENGTH - len);
		}

	internal:
		SectionName(byte *s) : n(gcnew array<byte>(LENGTH))				{ System::Runtime::InteropServices::Marshal::Copy(IntPtr(s), n, 0, LENGTH); }
		SectionName(byte *s, int len) : n(gcnew array<byte>(LENGTH))	{ CopyFrom(s, len); }
		SectionName(char *s) : n(gcnew array<byte>(LENGTH))				{ System::Runtime::InteropServices::Marshal::Copy(IntPtr(s), n, 0, LENGTH); }
		SectionName(char *s, int len) : n(gcnew array<byte>(LENGTH))	{ CopyFrom((byte*)s, len); }

	public:
		SectionName(array<byte> ^s) : n(gcnew array<byte>(LENGTH))	{ this->CopyFrom(s); }
		SectionName(array<char> ^s) : n(gcnew array<byte>(LENGTH))	{ this->CopyFrom((array<byte>^)s); }
		SectionName(string s) : n(gcnew array<byte>(LENGTH))		{ this->CopyFrom(System::Text::Encoding::ASCII->GetBytes(s)); }

		literal int LENGTH = 8;

		virtual string ToString() override
		{
			int i;
			for (i = 0; i < LENGTH && this->n[i] != 0; ++i);
			return System::Text::Encoding::ASCII->GetString(this->n, 0, i);
		}
		
		bool Equals(array<byte> ^o)
		{
			if (this->n == o)			return true;
			int len = o->Length;
			if (len > LENGTH || (len < LENGTH && this->n[len] != 0))
				return false;
			for (int i = 0; i < len; ++i)
				if (this->n[i] != o[i])	return false;
			return true;
		}
		bool Equals(array<char> ^o) { return this->Equals((array<byte>^)o); }
		bool Equals(string s) { return this->ToString() == s; }
		virtual bool Equals(Object ^o) override
		{
			Type ^t = o->GetType();
			if (SectionName::typeid->IsAssignableFrom(t))
				return this->Equals(((SectionName)o).n);
			else if (array<byte>::typeid->IsAssignableFrom(t))
				return this->Equals((array<byte>^)o);
			else if (array<char>::typeid->IsAssignableFrom(t))
				return this->Equals((array<byte>^)(array<char>^)o);
			else if (String::typeid->IsAssignableFrom(t))
				return this->Equals((string)o);
			return false;
		}

		static operator SectionName(array<byte> ^s)	{ return SectionName(s); }
		static operator SectionName(array<char> ^s)	{ return SectionName(s); }
		static operator SectionName(string s)		{ return SectionName(s); }
		static operator array<byte>^(SectionName s)	{ return (array<byte>^)s.n->Clone(); }
		static operator array<char>^(SectionName s)	{ return (array<char>^)(array<byte>^)s.n->Clone(); }
		static operator string(SectionName s)		{ return s.ToString(); }

		static bool operator==(SectionName l, array<byte> ^r)	{ return l.Equals(r); }
		static bool operator==(SectionName l, array<char> ^r)	{ return l.Equals((array<byte>^)r); }
		static bool operator==(SectionName l, string r)			{ return l.Equals(r); }
		static bool operator==(SectionName l, SectionName r)	{ return l.Equals(r.n); }
		static bool operator==(array<byte> ^l, SectionName r)	{ return r.Equals(l); }
		static bool operator==(array<char> ^l, SectionName r)	{ return r.Equals((array<byte>^)l); }
		static bool operator==(string l, SectionName r)			{ return r.Equals(l); }

	internal:
		static operator SectionName(char *s)			{ return SectionName(s); }
		static operator SectionName(byte *s)			{ return SectionName(s); }

		void CopyTo(byte *b) { System::Runtime::InteropServices::Marshal::Copy(this->n, 0, IntPtr(b), LENGTH); }
		void CopyTo(char *b) { System::Runtime::InteropServices::Marshal::Copy(this->n, 0, IntPtr(b), LENGTH); }

		bool Equals(byte *o)
		{
			for (int i = 0; i < LENGTH; ++i)
			{
				if (this->n[i] != o[i])	return false;
				if (this->n[i] == 0)	break;
			}
			return true;
		}
		bool Equals(char *o) { return this->Equals((byte*)o); }
	};
	public value struct ResID
	{
	public:
		static const ResID CURSOR = 1, BITMAP = 2, ICON = 3;
		static const ResID MENU = 4, DIALOG = 5, STRING = 6;
		static const ResID FONTDIR = 7, FONT = 8, ACCELERATOR = 9;
		static const ResID RCDATA = 10;
		static const ResID MESSAGETABLE = 11, GROUP_CURSOR = 12, GROUP_ICON = 14;
		static const ResID VERSION = 16, DLGINCLUDE = 17, PLUGPLAY = 19;
		static const ResID VXD = 20, ANICURSOR = 21, ANIICON = 22;
		static const ResID HTML = 23, MANIFEST = 24;

	private:
		IntPtr p;
		string s;

	public:
		ResID(ushort i) : s(nullptr), p(i) { }
		ResID(string s) : s(s), p(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s)) { }
		virtual string ToString() override { return this->s == nullptr ? p.ToString() : this->s; }
		static operator ResID(int i)	{ return ResID(i); }
		static operator ResID(string s)	{ return ResID(s); }
		static operator IntPtr(ResID t)	{ return t.p; }
		static operator string(ResID t)	{ return t.ToString(); }
	internal:
		static operator void*(ResID t)	{ return t.p.ToPointer(); }
		static operator PE::resid(ResID t)	{ return (PE::resid)t.p.ToPointer(); }
	};
} }
