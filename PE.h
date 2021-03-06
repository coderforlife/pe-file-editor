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

#include "Utilities.h"
#include "BasicTypes.h"

using namespace System;
using namespace System::Collections::Generic;

using namespace PE::Editor::Utilities;

#define NATIVE_WRAPPER_BASICS(C, NT) /* C is the managed class, NT is a native type */ \
	protected: File ^file; NT *n; \
	internal: C(File ^f, NT *n); \
		void UpdateMemory(ptrdiff_t dist) { this->n = (NT*)(((byte*)this->n) + dist); } \
		inline void CopyTo(NT *n) { *n = *this->n; }

#define NATIVE_WRAPPER_CAST_TO_NATIVE(C, NT) /* C is the managed class, NT is the native type */ \
	internal: static operator NT*(C ^c) { return c->n; }

#define NATIVE_WRAPPER_PROP(N, T) /* N is the property name, T is the type of the property */ \
	property T N { T get() { return this->n->N; } void set(T x) { this->n->N = x; } }

#define NATIVE_WRAPPER_PROP_ABSTRACT(N, T) /* N is the property name, T is the type of the property */ \
	property virtual T N { T get() = 0; void set(T x) = 0; }

#define NATIVE_WRAPPER_PROP_OVERRIDE(N, T) /* N is the property name, T is the type of the property */ \
	property virtual T N { T get() override { return (T)this->n->N; } void set(T x) override { this->n->N = x; } }

#define NATIVE_WRAPPER_PROP_OVERRIDE_RT(N, RT, T) /* N is the property name, RT is the return type, T is the type of the property */ \
	property virtual RT N { RT get() override { return (RT)(T)this->n->N; } void set(RT x) override { this->n->N = (T)x; } }

#define NATIVE_WRAPPER_NAMED_PROP(N, NN, T) /* N is the property name, NN is the native property name, T is the type of the property */ \
	property T N { T get() { return this->n->NN; } void set(T x) { this->n->NN = x; } }

#define NATIVE_WRAPPER_NAMED_PROP_OVERRIDE_RT(N, NN, RT, T) /* N is the property name, NN is the native property name, RT is the return type, T is the type of the property */ \
	property virtual RT N { RT get() override { return (RT)(T)this->n->NN; } void set(RT x) override { this->n->NN = (T)x; } }

#define NATIVE_WRAPPER_ENUM_PROP(N, E, T) /* N is the property name, E is the enum type, T is the underlying type of the property */ \
	property E N { E get() { return (E)this->n->N; } void set(E x) { this->n->N = (T)x; } }

#define NATIVE_WRAPPER_ENUM_PROP_ABSTRACT(N, E, T) /* N is the property name, E is the enum type, T is the underlying type of the property */ \
	property virtual E N { E get() = 0; void set(E x) = 0; }

#define NATIVE_WRAPPER_ENUM_PROP_OVERRIDE(N, E, T) /* N is the property name, E is the enum type, T is the underlying type of the property */ \
	property virtual E N { E get() override { return (E)this->n->N; } void set(E x) override { this->n->N = (T)x; } }

#define NATIVE_WRAPPER_NAMED_ENUM_PROP(N, NN, E, T) /* N is the property name, NN is the native property name, E is the enum type, T is the underlying type of the property */ \
	property E N { E get() { return (E)this->n->NN; } void set(E x) { this->n->NN = (T)x; } }

#define NATIVE_WRAPPED_VERSION_PROPS(N, T) /* N is the property name (without Major/Minor), T is the type of the Major/Minor properties */ \
	protected: NATIVE_WRAPPER_PROP(Major##N, T) NATIVE_WRAPPER_PROP(Minor##N, T) \
	public: property System::Version ^N { System::Version ^get() { return gcnew System::Version(this->n->Major##N, this->n->Minor##N); } void set(System::Version ^x) { this->n->Major##N = x->Major; this->n->Minor##N = x->Minor; } }

#define NATIVE_WRAPPED_VERSION_PROPS_ABSTRACT(N, T) /* N is the property name (without Major/Minor), T is the type of the Major/Minor properties */ \
	protected: NATIVE_WRAPPER_PROP_ABSTRACT(Major##N, T) NATIVE_WRAPPER_PROP_ABSTRACT(Minor##N, T) \
	public: property System::Version ^N { System::Version ^get() { return gcnew System::Version(this->Major##N, this->Minor##N); } void set(System::Version ^x) { this->Major##N = x->Major; this->Minor##N = x->Minor; } }

#define NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE(N, T) /* N is the property name (without Major/Minor), T is the type of the Major/Minor properties */ \
	protected: NATIVE_WRAPPER_PROP_OVERRIDE(Major##N, T) NATIVE_WRAPPER_PROP_OVERRIDE(Minor##N, T) public:

#define NATIVE_WRAPPED_WRAPPED_PROP(N, T) /* N is the property name, T is the wrapped type */ \
	property T ^N { T ^get() { return gcnew T(this->file, &this->n->N); } void set(T ^x) { x->CopyTo(&this->n->N); } }

#define NATIVE_WRAPPED_WRAPPED_PROP_ABSTRACT(N, T) /* N is the property name, T is the wrapped type */ \
	property virtual T ^N { T ^get() = 0; void set(T ^x) = 0; }

#define NATIVE_WRAPPED_WRAPPED_PROP_OVERRIDE(N, RT, T) /* N is the property name, RT is the return type, T is the wrapped type */ \
	property virtual RT ^N { RT ^get() override { return (RT^)(gcnew T(this->file, &this->n->N)); } void set(RT ^x) override { ((T^)x)->CopyTo(&this->n->N); } }

#define NATIVE_WRAPPED_ARRAY_BASICS(C, T, NT) /* C is the managed class, T is the managed type, NT is the native type; T must have: static operator NT*(T); void CopyTo(NT*) */ \
	protected: NT *a; \
		virtual T ^Get(int i) override			{ return gcnew T(this->file, this->a+i); } \
		virtual void Set(int i, T ^x) override	{ x->CopyTo(this->a+i); } \
	internal: C(File ^f, NT *a, int n) : ArrayPointerBase(f, n), a(a) { } \
		virtual void UpdateMemory(ptrdiff_t dist) override { this->a = (NT*)(((byte*)this->a) + dist); } \
	public: \
		virtual bool Contains(T ^d) override	{ NT *x = d; return (x >= this->a && x < (this->a+this->n)); } \
		virtual int IndexOf(T ^d) override		{ NT *x = d; return (x >= this->a && x < (this->a+this->n)) ? (int)(x - this->a) : -1; }


namespace PE { namespace Editor {
	ref class File; // forward declaration

	public ref class FileHeader
	{
		NATIVE_WRAPPER_BASICS(FileHeader, PE::Image::FileHeader)
	public:
		NATIVE_WRAPPER_ENUM_PROP(Machine,				PE::Editor::Machine, PE::Image::FileHeader::MachineType)
		NATIVE_WRAPPER_PROP		(NumberOfSections,		ushort)
		property DateTime TimeDateStamp {
			DateTime get() { return ToDateTime(this->n->TimeDateStamp); }
			void set(DateTime x) { this->n->TimeDateStamp = ToPEFileTime(x); }
		}
		NATIVE_WRAPPER_PROP		(PointerToSymbolTable,	HexInt32)
		NATIVE_WRAPPER_PROP		(NumberOfSymbols,		uint)
		NATIVE_WRAPPER_PROP		(SizeOfOptionalHeader,	HexInt16)
		NATIVE_WRAPPER_ENUM_PROP(Characteristics,		FileCharacteristics, PE::Image::FileHeader::CharacteristicFlags)
		virtual string ToString() override {
			return this->Machine.ToString()+L", "+this->NumberOfSections+L" Sections, "+this->Characteristics.ToString();
		}
	};
	public ref class DataDirectory
	{
		NATIVE_WRAPPER_BASICS(DataDirectory, PE::Image::DataDirectory)
		NATIVE_WRAPPER_CAST_TO_NATIVE(DataDirectory, PE::Image::DataDirectory)
	public:
		NATIVE_WRAPPER_PROP(VirtualAddress,	HexInt32)
		NATIVE_WRAPPER_PROP(Size,			HexInt32)
		virtual string ToString() override {
			return String::Format(L"{0:X8}-{1:X8}", this->VirtualAddress, this->VirtualAddress+this->Size);
		}
	};
	public ref class DataDirectories : ArrayPointerBase<DataDirectory^>
	{
		NATIVE_WRAPPED_ARRAY_BASICS(DataDirectories, DataDirectory, PE::Image::DataDirectory)
	public:
		property DataDirectory ^default[DirectoryEntry] { DataDirectory ^get(DirectoryEntry i) { return this[(int)i]; } void set(DirectoryEntry i ,DataDirectory ^d) { this[(int)i] = d; } }
	};

	public ref class OptionalHeader abstract
	{
	public:
		NATIVE_WRAPPER_ENUM_PROP_ABSTRACT		(Magic,						OptionalHeaderMagic,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS_ABSTRACT	(LinkerVersion,				byte)
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfCode,				HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfInitializedData,		HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfUninitializedData,	HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(AddressOfEntryPoint,		HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(BaseOfCode,				HexInt32)
		//x86-only NATIVE_WRAPPER_PROP			(BaseOfData,				HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(ImageBase,					HexInt^) // either 32/64
		NATIVE_WRAPPER_PROP_ABSTRACT			(SectionAlignment,			HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(FileAlignment,				HexInt32)
		NATIVE_WRAPPED_VERSION_PROPS_ABSTRACT	(OperatingSystemVersion,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS_ABSTRACT	(ImageVersion,				ushort)
		NATIVE_WRAPPED_VERSION_PROPS_ABSTRACT	(SubsystemVersion,			ushort)
		NATIVE_WRAPPER_PROP_ABSTRACT			(Win32VersionValue,			HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfImage,				HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfHeaders,				HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(CheckSum,					HexInt32)
		NATIVE_WRAPPER_ENUM_PROP_ABSTRACT		(Subsystem,					PE::Editor::Subsystem,			ushort)
		NATIVE_WRAPPER_ENUM_PROP_ABSTRACT		(DllCharacteristics,		PE::Editor::DllCharacteristics,	ushort)
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfStackReserve,		HexInt^) // either 32/64
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfStackCommit,			HexInt^) // either 32/64
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfHeapReserve,			HexInt^) // either 32/64
		NATIVE_WRAPPER_PROP_ABSTRACT			(SizeOfHeapCommit,			HexInt^) // either 32/64
		NATIVE_WRAPPER_PROP_ABSTRACT			(LoaderFlags,				HexInt32)
		NATIVE_WRAPPER_PROP_ABSTRACT			(NumberOfRvaAndSizes,		uint)

		property virtual PE::Editor::DataDirectories ^DataDirectories { PE::Editor::DataDirectories ^get() = 0; }
		virtual string ToString() override {
			return this->Magic.ToString()+/*L", Base: "+this->ImageBase+*/L", "+this->Subsystem.ToString()+L", "+this->DllCharacteristics.ToString();
		}
	};
	public ref class OptionalHeader32 : OptionalHeader
	{
		NATIVE_WRAPPER_BASICS(OptionalHeader32, PE::Image::OptionalHeader32)
	public:
		NATIVE_WRAPPER_ENUM_PROP_OVERRIDE		(Magic,						OptionalHeaderMagic,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(LinkerVersion,				byte)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfCode,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfInitializedData,		HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfUninitializedData,	HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(AddressOfEntryPoint,		HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(BaseOfCode,				HexInt32)
		NATIVE_WRAPPER_PROP						(BaseOfData,				HexInt32)
		NATIVE_WRAPPER_NAMED_PROP_OVERRIDE_RT	(ImageBase,	ImageBase32,	HexInt^, HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SectionAlignment,			HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(FileAlignment,				HexInt32)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(OperatingSystemVersion,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(ImageVersion,				ushort)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(SubsystemVersion,			ushort)
		NATIVE_WRAPPER_PROP_OVERRIDE			(Win32VersionValue,			HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfImage,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfHeaders,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(CheckSum,					HexInt32)
		NATIVE_WRAPPER_ENUM_PROP_OVERRIDE		(Subsystem,					PE::Editor::Subsystem,			PE::Image::OptionalHeader::SubsystemType)
		NATIVE_WRAPPER_ENUM_PROP_OVERRIDE		(DllCharacteristics,		PE::Editor::DllCharacteristics,	PE::Image::OptionalHeader::DllCharacteristicFlags)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfStackReserve,		HexInt^, HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfStackCommit,			HexInt^, HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfHeapReserve,			HexInt^, HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfHeapCommit,			HexInt^, HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(LoaderFlags,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(NumberOfRvaAndSizes,		uint)
		property virtual PE::Editor::DataDirectories ^DataDirectories { PE::Editor::DataDirectories ^get() override {
			return gcnew PE::Editor::DataDirectories(this->file, this->n->DataDirectory, this->n->NumberOfRvaAndSizes);
		} }
	};
	public ref class OptionalHeader64 : OptionalHeader
	{
		NATIVE_WRAPPER_BASICS(OptionalHeader64, PE::Image::OptionalHeader64)
	public:
		NATIVE_WRAPPER_ENUM_PROP_OVERRIDE		(Magic,						OptionalHeaderMagic,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(LinkerVersion,				byte)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfCode,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfInitializedData,		HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfUninitializedData,	HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(AddressOfEntryPoint,		HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(BaseOfCode,				HexInt32)
		NATIVE_WRAPPER_NAMED_PROP_OVERRIDE_RT	(ImageBase,	ImageBase64,	HexInt^, HexInt64)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SectionAlignment,			HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(FileAlignment,				HexInt32)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(OperatingSystemVersion,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(ImageVersion,				ushort)
		NATIVE_WRAPPED_VERSION_PROPS_OVERRIDE	(SubsystemVersion,			ushort)
		NATIVE_WRAPPER_PROP_OVERRIDE			(Win32VersionValue,			HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfImage,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(SizeOfHeaders,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(CheckSum,					HexInt32)
		NATIVE_WRAPPER_ENUM_PROP_OVERRIDE		(Subsystem,					PE::Editor::Subsystem,			PE::Image::OptionalHeader::SubsystemType)
		NATIVE_WRAPPER_ENUM_PROP_OVERRIDE		(DllCharacteristics,		PE::Editor::DllCharacteristics,	PE::Image::OptionalHeader::DllCharacteristicFlags)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfStackReserve,		HexInt^, HexInt64)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfStackCommit,			HexInt^, HexInt64)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfHeapReserve,			HexInt^, HexInt64)
		NATIVE_WRAPPER_PROP_OVERRIDE_RT			(SizeOfHeapCommit,			HexInt^, HexInt64)
		NATIVE_WRAPPER_PROP_OVERRIDE			(LoaderFlags,				HexInt32)
		NATIVE_WRAPPER_PROP_OVERRIDE			(NumberOfRvaAndSizes,		uint)
		property virtual PE::Editor::DataDirectories ^DataDirectories { PE::Editor::DataDirectories ^get() override {
			return gcnew PE::Editor::DataDirectories(this->file, this->n->DataDirectory, this->n->NumberOfRvaAndSizes);
		} }
	};
	public ref class NtHeaders abstract
	{
	public:
		NATIVE_WRAPPER_PROP_ABSTRACT			(Signature,			HexInt32)
		NATIVE_WRAPPED_WRAPPED_PROP_ABSTRACT	(FileHeader,		PE::Editor::FileHeader)
		NATIVE_WRAPPED_WRAPPED_PROP_ABSTRACT	(OptionalHeader,	PE::Editor::OptionalHeader)
	};
	public ref class NtHeaders32 : NtHeaders
	{
		NATIVE_WRAPPER_BASICS(NtHeaders32, PE::Image::NTHeaders32)
	public:
		NATIVE_WRAPPER_PROP_OVERRIDE			(Signature,			HexInt32)
		NATIVE_WRAPPED_WRAPPED_PROP_OVERRIDE	(FileHeader,		PE::Editor::FileHeader,		PE::Editor::FileHeader)
		NATIVE_WRAPPED_WRAPPED_PROP_OVERRIDE	(OptionalHeader,	PE::Editor::OptionalHeader,	OptionalHeader32)
	};
	public ref class NtHeaders64 : NtHeaders
	{
		NATIVE_WRAPPER_BASICS(NtHeaders64, PE::Image::NTHeaders64)
	public:
		NATIVE_WRAPPER_PROP_OVERRIDE			(Signature,			HexInt32)
		NATIVE_WRAPPED_WRAPPED_PROP_OVERRIDE	(FileHeader,		PE::Editor::FileHeader,		PE::Editor::FileHeader)
		NATIVE_WRAPPED_WRAPPED_PROP_OVERRIDE	(OptionalHeader,	PE::Editor::OptionalHeader,	OptionalHeader64)
	};
	public ref class SectionHeader
	{
		NATIVE_WRAPPER_BASICS(SectionHeader, PE::Image::SectionHeader)
		NATIVE_WRAPPER_CAST_TO_NATIVE(SectionHeader, PE::Image::SectionHeader)
	public:
		property int Index { int get(); };
		property SectionName Name	{
			SectionName get() { return this->n->Name; }
			void set(SectionName x) { x.CopyTo(this->n->Name); }
		}
		NATIVE_WRAPPER_PROP			(VirtualSize,			HexInt32)
		NATIVE_WRAPPER_PROP			(VirtualAddress,		HexInt32)
		NATIVE_WRAPPER_PROP			(SizeOfRawData,			HexInt32)
		NATIVE_WRAPPER_PROP			(PointerToRawData,		HexInt32)
		NATIVE_WRAPPER_PROP			(PointerToRelocations,	HexInt32)
		NATIVE_WRAPPER_PROP			(PointerToLinenumbers,	HexInt32)
		NATIVE_WRAPPER_PROP			(NumberOfRelocations,	ushort)
		NATIVE_WRAPPER_PROP			(NumberOfLinenumbers,	ushort)
		NATIVE_WRAPPER_ENUM_PROP	(Characteristics,		SectionCharacteristics,	uint)
		void Expand(uint room);
		SectionHeader ^AddBefore(SectionName name, uint room, SectionCharacteristics chars);
		virtual string ToString() override {
			return this->Name.ToString()+L", "+this->Characteristics.ToString()+
				String::Format(L", VA: {0:X8}-{1:X8}", this->VirtualAddress, this->VirtualAddress+this->VirtualSize)+
				String::Format(L", Data: {0:X8}-{1:X8}", this->PointerToRawData, this->PointerToRawData+this->SizeOfRawData);
		}
	};
	public ref class SectionHeaders : ArrayPointerBase<SectionHeader^>
	{
		NATIVE_WRAPPED_ARRAY_BASICS(SectionHeaders, SectionHeader, PE::Image::SectionHeader)
	public:
		property SectionHeader ^default[SectionName] { SectionHeader ^get(SectionName n) {
			for (int i = 0; i < this->n; ++i)
				if (n == this->a[i].Name)
					return this->Get(i);
			throw gcnew KeyNotFoundException();
		} }
		property SectionHeader ^default[string] { SectionHeader ^get(string n) { return this[SectionName(n)]; } }

		SectionHeader ^GetByRVA(uint rva) {
			for (int i = 0; i < this->n; ++i)
				if (this->a[i].VirtualAddress <= rva && rva < this->a[i].VirtualAddress + this->a[i].VirtualSize)
					return this->Get(i);
			throw gcnew ArgumentOutOfRangeException();
		}
		SectionHeader ^GetByVA(ulong va);
		SectionHeader ^Add(SectionName name, uint room, SectionCharacteristics chars);
		SectionHeader ^Add(string name, uint room, SectionCharacteristics chars) { return this->Add(SectionName(name), room, chars); }
	};

	public ref class File
	{
	private:
		byte *prev_data_pntr;
		PE::File *f;

		LinkedList<System::Runtime::InteropServices::GCHandle> ^references;
		void Revalidate()
		{
			byte *cur_data_pntr = this->f->get();
			if (cur_data_pntr == this->prev_data_pntr) { return; }

			ptrdiff_t dist = cur_data_pntr - this->prev_data_pntr;

			LinkedListNode<System::Runtime::InteropServices::GCHandle> ^i = this->references->First;
			while (i != nullptr)
			{
				LinkedListNode<System::Runtime::InteropServices::GCHandle> ^n = i->Next;
				System::Runtime::InteropServices::GCHandle h = i->Value;
				if (h.Target != nullptr)
					((UpdateMovableMemory^)h.Target)(dist);
				else
					this->references->Remove(i);
				i = n;
			}
			this->prev_data_pntr = cur_data_pntr;
		}
		void Init(PE::File *f) {
			references = gcnew LinkedList<System::Runtime::InteropServices::GCHandle>();
			this->f = f;
			this->prev_data_pntr = this->f->get();
		}

	internal:
		void AddRef(UpdateMovableMemory ^umm) { this->references->AddLast(System::Runtime::InteropServices::GCHandle::Alloc(umm, System::Runtime::InteropServices::GCHandleType::Weak)); }
		inline int GetSectionHeaderIndex(PE::Image::SectionHeader *s)	{ return (int)(s - this->f->getSectionHeader(0)); }
		inline int GetDataDirectoryIndex(PE::Image::DataDirectory *d)	{ return (int)(d - this->f->getDataDirectory(0)); }
		void ExpandSectionHdr(PE::Image::SectionHeader *s, uint room)	{
			PE::Image::SectionHeader *n = this->f->getExpandedSectionHdr(this->GetSectionHeaderIndex(s), room);
			if (!n) throw gcnew Exception();
			this->Revalidate();
		}
		SectionHeader ^CreateSection(PE::Image::SectionHeader *s, SectionName Name, uint room, SectionCharacteristics chars)	{
			char name[SectionName::LENGTH];
			Name.CopyTo(name);
			PE::Image::SectionHeader *n = s == NULL ?
				this->f->createSection(name, room, (PE::Image::SectionHeader::CharacteristicFlags)chars) :
				this->f->createSection(this->GetSectionHeaderIndex(s), name, room, (PE::Image::SectionHeader::CharacteristicFlags)chars);
			if (!n) throw gcnew Exception();
			this->Revalidate();
			return gcnew SectionHeader(this, n);
		}

	public:
		//File(LPVOID data, size_t size, bool readonly) { this->Init(new PE::File(?, ?, readonly)); }
		//File(LPVOID data, size_t size)			{ this->Init(new PE::File(?, ?, false)); }
		File(string filename, bool readonly)		{ this->Init(new PE::File(as_native(filename), readonly)); }
		File(string filename)						{ this->Init(new PE::File(as_native(filename), false)); }
		~File()										{ delete f; f = NULL; }
		property bool IsLoaded						{ bool get() { return this->f->isLoaded(); } }
		property bool IsReadOnly					{ bool get() { return this->f->isReadOnly(); } }

		bool Save()									{ return this->f->save(); }
		
		property bool Is32Bit						{ bool get() { return this->f->is32bit(); } }
		property bool Is64Bit						{ bool get() { return this->f->is64bit(); } }
		property HexInt64 ImageBase					{ HexInt64 get() { return this->f->getImageBase(); } }

		property PE::Editor::NtHeaders ^NtHeaders			{ PE::Editor::NtHeaders ^get() { return this->Is32Bit ? (PE::Editor::NtHeaders^)gcnew PE::Editor::NtHeaders32(this, this->f->getNtHeaders32()) : (PE::Editor::NtHeaders^)gcnew PE::Editor::NtHeaders64(this, this->f->getNtHeaders64()); } }
		property PE::Editor::FileHeader ^FileHeader			{ PE::Editor::FileHeader ^get() { return gcnew PE::Editor::FileHeader(this, this->f->getFileHeader()); } }
		property PE::Editor::OptionalHeader ^OptionalHeader	{ PE::Editor::OptionalHeader ^get() { return this->NtHeaders->OptionalHeader; } }

		property PE::Editor::DataDirectories ^DataDirectories	{ PE::Editor::DataDirectories ^get() { return gcnew PE::Editor::DataDirectories(this, this->f->getDataDirectory(0), this->f->getDataDirectoryCount()); } }
		property PE::Editor::SectionHeaders ^SectionHeaders		{ PE::Editor::SectionHeaders  ^get() { return gcnew PE::Editor::SectionHeaders (this, this->f->getSectionHeader(0), this->f->getSectionHeaderCount()); } }

		property HexInt32 Size { HexInt32 get() { return (HexInt32)(uint)this->f->getSize(); } void set(HexInt32 x) { if (!this->f->setSize(x, false)) throw gcnew Exception(); this->Revalidate(); } }

		property RawData^ Data { RawData^ get() { uint32_t n; byte *x = this->f->get(0, &n); return gcnew RawData(this, x, n); } }
		RawData^ GetSectionData(SectionHeader^ s) { uint32_t n; byte *x = this->f->get(s->PointerToRawData, &n); return gcnew RawData(this, x, Math::Min(n, s->SizeOfRawData)); }
		RawData^ GetSectionData(SectionName s)	{ return this->GetSectionData(this->SectionHeaders[s]); }
		RawData^ GetSectionData(string s)		{ return this->GetSectionData(this->SectionHeaders[s]); }

		bool Flush()												{ return this->f->flush(); }

		bool UpdatePEChkSum()										{ return this->f->updatePEChkSum(); }
		property RawData^ ExtraData									{ RawData^ get() { if (!this->f->hasExtraData()) { return nullptr; } uint32_t n; byte *x = (byte*)(void*)this->f->getExtraData(&n); return gcnew RawData(this, x, n); } }
		bool CreateExtraData()										{ uint32_t n; return this->f->getExtraData(&n) != null; }
		bool ClearCertificateTable()								{ if (this->f->clearCertificateTable()) { this->Revalidate(); return true; } return false; }
		property System::Version ^FileVersion						{ System::Version ^get() { PE::Version::Version v = this->f->getFileVersion(); return gcnew System::Version(v.Major, v.Minor, v.Build, v.Revision); } }
		property bool IsModified { bool get() { return this->f->isAlreadyModified(); } void set(bool x) { if (!x) throw gcnew ArgumentException(); this->f->setModifiedFlag(); } }
		bool RemoveRelocs(uint start, uint end, bool reverse)		{ return this->f->removeRelocs(start, end, reverse); }
		bool RemoveRelocs(uint start, uint end)						{ return this->f->removeRelocs(start, end, false); }

		bool ResourceExists(ResID type, ResID name)						{ return this->f->resourceExists(type, name, (ushort*)NULL); }
		bool ResourceExists(ResID type, ResID name, ushort lang)		{ return this->f->resourceExists(type, name, lang); }
		bool ResourceExists(ResID type, ResID name, ushort %lang)		{ return this->f->resourceExists(type, name, wptr(lang)); }
		array<byte> ^GetResource(ResID type, ResID name)				{ size_t size = 0; return to_managed((bytes)this->f->getResource(type, name, (ushort*)NULL, &size), size); }
		array<byte> ^GetResource(ResID type, ResID name, ushort lang)	{ size_t size = 0; return to_managed((bytes)this->f->getResource(type, name, lang, &size), size); }
		array<byte> ^GetResource(ResID type, ResID name, ushort %lang)	{ size_t size = 0; return to_managed((bytes)this->f->getResource(type, name, wptr(lang), &size), size); }
		bool RemoveResource(ResID type, ResID name, ushort lang)		{ return this->f->removeResource(type, name, lang); }
		bool AddResource(ResID type, ResID name, ushort lang, array<byte> ^data, Overwrite overwrite)	{ return this->f->addResource(type, name, lang, NATIVE(data), overwrite); }
		bool AddResource(ResID type, ResID name, ushort lang, array<byte> ^data)						{ return this->f->addResource(type, name, lang, NATIVE(data), PE::ALWAYS); }

		//static LPVOID GetResourceDirect(LPVOID data, ResID type, ResID name); // must be freed, massively performance enhanced for a single retrieval and no editing // lang? size?
		//static bool UpdatePEChkSum(LPBYTE data, size_t dwSize, size_t peOffset, uint dwOldCheck);
		//static bool GetVersionInfo(LPVOID ver, string query, LPVOID %buffer, uint %len);
		//static VS_FIXEDFILEINFO *GetVersionInfo(LPVOID ver);
		//static void UnmapAllViewsOfFile(string file)				{ PE::File::UnmapAllViewsOfFile(as_native(file)); }
	};
} }
