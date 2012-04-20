#pragma once

#include "general.h"

#include "Utilities.h"
#include "BasicTypes.h"

using namespace System;
using namespace System::Collections::Generic;

using namespace PE::Utilities;

#define NATIVE_WRAPPER_BASICS(C, NT) /* C is the managed class, NT is a native type */ \
	protected: File ^file; NT *n; \
	internal: C(File ^f, NT *n); \
		void UpdateMemory(ptrdiff_t dist) { this->n = (NT*)(((byte*)this->n) + dist); } \
		inline void CopyTo(NT *n) { *n = *this->n; }

#define NATIVE_WRAPPER_CAST_TO_NATIVE(C, NT) /* C is the managed class, NT is the native type */ \
	internal: static operator NT*(C ^c) { return c->n; }

#define NATIVE_WRAPPER_PROP(N, T) /* N is the property name, T is the type of the property */ \
	property T N { T get() { return this->n->N; } void set(T x) { this->n->N = x; } }

#define NATIVE_WRAPPER_NAMED_PROP(N, NN, T) /* N is the property name, NN is the native property name, T is the type of the property */ \
	property T N { T get() { return this->n->NN; } void set(T x) { this->n->NN = x; } }

#define NATIVE_WRAPPER_ENUM_PROP(N, E, T) /* N is the property name, E is the enum type, T is the underlying type of the property */ \
	property E N { E get() { return (E)this->n->N; } void set(E x) { this->n->N = (T)x; } }

#define NATIVE_WRAPPER_NAMED_ENUM_PROP(N, NN, E, T) /* N is the property name, NN is the native property name, E is the enum type, T is the underlying type of the property */ \
	property E N { E get() { return (E)this->n->NN; } void set(E x) { this->n->NN = (T)x; } }

#define NATIVE_WRAPPED_VERSION_PROPS(N, T) /* N is the property name (without Major/Minor), T is the type of the Major/Minor properties */ \
	NATIVE_WRAPPER_PROP(Major##N, T) NATIVE_WRAPPER_PROP(Minor##N, T) \
	property Version ^N { Version ^get() { return gcnew Version(this->n->Major##N, this->n->Minor##N); } void set(Version ^x) { this->n->Major##N = x->Major; this->n->Minor##N = x->Minor; } }

#define NATIVE_WRAPPED_WRAPPED_PROP(N, T) /* N is the property name, T is the wrapped type */ \
	property T ^N { T ^get() { return gcnew T(this->file, &this->n->N); } void set(T ^x) { x->CopyTo(&this->n->N); } }

#define NATIVE_WRAPPED_ARRAY_BASICS(C, T, NT) /* C is the managed class, T is the managed type, NT is the native type; T must have: static operator NT*(T); void CopyTo(NT*) */ \
	protected: NT *a; \
		virtual T ^Get(int i) override			{ return gcnew T(this->file, this->a+i); } \
		virtual void Set(int i, T ^x) override	{ x->CopyTo(this->a+i); } \
	internal: C(File ^f, NT *a, int n) : ArrayPointerBase(f, n), a(a) { } \
		virtual void UpdateMemory(ptrdiff_t dist) override { this->a = (NT*)(((byte*)this->a) + dist); } \
	public: \
		virtual bool Contains(T ^d) override	{ NT *x = d; return (x >= this->a && x < (this->a+this->n)); } \
		virtual int IndexOf(T ^d) override		{ NT *x = d; return (x >= this->a && x < (this->a+this->n)) ? (int)(x - this->a) : -1; }


namespace PE {
	ref class File; // forward declaration

	public ref class FileHeader
	{
		NATIVE_WRAPPER_BASICS(FileHeader, IMAGE_FILE_HEADER)
	public:
		NATIVE_WRAPPER_ENUM_PROP(Machine,				PE::Machine, ushort)
		NATIVE_WRAPPER_PROP		(NumberOfSections,		ushort)
		property DateTime TimeDateStamp {
			DateTime get() { return ToDateTime(this->n->TimeDateStamp); }
			void set(DateTime x) { this->n->TimeDateStamp = ToPEFileTime(x); }
		}
		NATIVE_WRAPPER_PROP		(PointerToSymbolTable,	uint)
		NATIVE_WRAPPER_PROP		(NumberOfSymbols,		uint)
		NATIVE_WRAPPER_PROP		(SizeOfOptionalHeader,	ushort)
		NATIVE_WRAPPER_ENUM_PROP(Characteristics,		FileCharacteristics, ushort)
		virtual string ToString() override {
			return this->Machine.ToString()+L", "+this->NumberOfSections+L" Sections, "+this->Characteristics.ToString();
		}
	};
	public ref class DataDirectory
	{
		NATIVE_WRAPPER_BASICS(DataDirectory, IMAGE_DATA_DIRECTORY)
		NATIVE_WRAPPER_CAST_TO_NATIVE(DataDirectory, IMAGE_DATA_DIRECTORY)
	public:
		NATIVE_WRAPPER_PROP(VirtualAddress,	uint)
		NATIVE_WRAPPER_PROP(Size,			uint)
		virtual string ToString() override {
			return String::Format(L"{0:X8}-{1:X8}", this->VirtualAddress, this->VirtualAddress+this->Size);
		}
	};
	public ref class DataDirectories : ArrayPointerBase<DataDirectory^>
	{
		NATIVE_WRAPPED_ARRAY_BASICS(DataDirectories, DataDirectory, IMAGE_DATA_DIRECTORY)
	public:
		property DataDirectory ^default[DirectoryEntry] { DataDirectory ^get(DirectoryEntry i) { return this[(int)i]; } void set(DirectoryEntry i ,DataDirectory ^d) { this[(int)i] = d; } }
	};
	public ref class OptionalHeader32
	{
		NATIVE_WRAPPER_BASICS(OptionalHeader32, IMAGE_OPTIONAL_HEADER32)
	public:
		NATIVE_WRAPPER_ENUM_PROP	(Magic,						OptionalHeaderMagic,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS(LinkerVersion,				byte)
		NATIVE_WRAPPER_PROP			(SizeOfCode,				uint)
		NATIVE_WRAPPER_PROP			(SizeOfInitializedData,		uint)
		NATIVE_WRAPPER_PROP			(SizeOfUninitializedData,	uint)
		NATIVE_WRAPPER_PROP			(AddressOfEntryPoint,		uint)
		NATIVE_WRAPPER_PROP			(BaseOfCode,				uint)
		NATIVE_WRAPPER_PROP			(BaseOfData,				uint)
		NATIVE_WRAPPER_PROP			(ImageBase,					uint)
		NATIVE_WRAPPER_PROP			(SectionAlignment,			uint)
		NATIVE_WRAPPER_PROP			(FileAlignment,				uint)
		NATIVE_WRAPPED_VERSION_PROPS(OperatingSystemVersion,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS(ImageVersion,				ushort)
		NATIVE_WRAPPED_VERSION_PROPS(SubsystemVersion,			ushort)
		NATIVE_WRAPPER_PROP			(Win32VersionValue,			uint)
		NATIVE_WRAPPER_PROP			(SizeOfImage,				uint)
		NATIVE_WRAPPER_PROP			(SizeOfHeaders,				uint)
		NATIVE_WRAPPER_PROP			(CheckSum,					uint)
		NATIVE_WRAPPER_ENUM_PROP	(Subsystem,					PE::Subsystem,			ushort)
		NATIVE_WRAPPER_ENUM_PROP	(DllCharacteristics,		PE::DllCharacteristics,	ushort)
		NATIVE_WRAPPER_PROP			(SizeOfStackReserve,		uint)
		NATIVE_WRAPPER_PROP			(SizeOfStackCommit,			uint)
		NATIVE_WRAPPER_PROP			(SizeOfHeapReserve,			uint)
		NATIVE_WRAPPER_PROP			(SizeOfHeapCommit,			uint)
		NATIVE_WRAPPER_PROP			(LoaderFlags,				uint)
		NATIVE_WRAPPER_PROP			(NumberOfRvaAndSizes,		uint)
		property PE::DataDirectories ^DataDirectories { PE::DataDirectories ^get() {
			return gcnew PE::DataDirectories(this->file, this->n->DataDirectory, this->n->NumberOfRvaAndSizes);
		} }
		virtual string ToString() override {
			return this->Magic.ToString()+String::Format(L", Base: {0:X8}, ", this->ImageBase)+this->Subsystem.ToString()+L", "+this->DllCharacteristics.ToString();
		}
	};
	public ref class OptionalHeader64
	{
		NATIVE_WRAPPER_BASICS(OptionalHeader64, IMAGE_OPTIONAL_HEADER64)
	public:
		NATIVE_WRAPPER_ENUM_PROP	(Magic,						OptionalHeaderMagic,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS(LinkerVersion,				byte)
		NATIVE_WRAPPER_PROP			(SizeOfCode,				uint)
		NATIVE_WRAPPER_PROP			(SizeOfInitializedData,		uint)
		NATIVE_WRAPPER_PROP			(SizeOfUninitializedData,	uint)
		NATIVE_WRAPPER_PROP			(AddressOfEntryPoint,		uint)
		NATIVE_WRAPPER_PROP			(BaseOfCode,				uint)
		NATIVE_WRAPPER_PROP			(ImageBase,					ulong)
		NATIVE_WRAPPER_PROP			(SectionAlignment,			uint)
		NATIVE_WRAPPER_PROP			(FileAlignment,				uint)
		NATIVE_WRAPPED_VERSION_PROPS(OperatingSystemVersion,	ushort)
		NATIVE_WRAPPED_VERSION_PROPS(ImageVersion,				ushort)
		NATIVE_WRAPPED_VERSION_PROPS(SubsystemVersion,			ushort)
		NATIVE_WRAPPER_PROP			(Win32VersionValue,			uint)
		NATIVE_WRAPPER_PROP			(SizeOfImage,				uint)
		NATIVE_WRAPPER_PROP			(SizeOfHeaders,				uint)
		NATIVE_WRAPPER_PROP			(CheckSum,					uint)
		NATIVE_WRAPPER_ENUM_PROP	(Subsystem,					PE::Subsystem,			ushort)
		NATIVE_WRAPPER_ENUM_PROP	(DllCharacteristics,		PE::DllCharacteristics,	ushort)
		NATIVE_WRAPPER_PROP			(SizeOfStackReserve,		ulong)
		NATIVE_WRAPPER_PROP			(SizeOfStackCommit,			ulong)
		NATIVE_WRAPPER_PROP			(SizeOfHeapReserve,			ulong)
		NATIVE_WRAPPER_PROP			(SizeOfHeapCommit,			ulong)
		NATIVE_WRAPPER_PROP			(LoaderFlags,				uint)
		NATIVE_WRAPPER_PROP			(NumberOfRvaAndSizes,		uint)
		property PE::DataDirectories ^DataDirectories { PE::DataDirectories ^get() {
			return gcnew PE::DataDirectories(this->file, this->n->DataDirectory, this->n->NumberOfRvaAndSizes);
		} }
		virtual string ToString() override {
			return this->Magic.ToString()+String::Format(L", Base: {0:X16}, ", this->ImageBase)+this->Subsystem.ToString()+L", "+this->DllCharacteristics.ToString();
		}
	};
	public ref class NtHeaders32
	{
		NATIVE_WRAPPER_BASICS(NtHeaders32, IMAGE_NT_HEADERS32)
	public:
		NATIVE_WRAPPER_PROP			(Signature,			uint)
		NATIVE_WRAPPED_WRAPPED_PROP	(FileHeader,		PE::FileHeader)
		NATIVE_WRAPPED_WRAPPED_PROP	(OptionalHeader,	OptionalHeader32)
	};
	public ref class NtHeaders64
	{
		NATIVE_WRAPPER_BASICS(NtHeaders64, IMAGE_NT_HEADERS64)
	public:
		NATIVE_WRAPPER_PROP			(Signature,			uint)
		NATIVE_WRAPPED_WRAPPED_PROP	(FileHeader,		PE::FileHeader)
		NATIVE_WRAPPED_WRAPPED_PROP	(OptionalHeader,	OptionalHeader64)
	};
	public ref class SectionHeader
	{
		NATIVE_WRAPPER_BASICS(SectionHeader, IMAGE_SECTION_HEADER)
		NATIVE_WRAPPER_CAST_TO_NATIVE(SectionHeader, IMAGE_SECTION_HEADER)
	public:
		property int Index { int get(); };
		property SectionName Name	{
			SectionName get() { return this->n->Name; }
			void set(SectionName x) { x.CopyTo(this->n->Name); }
		}
		NATIVE_WRAPPER_NAMED_PROP	(VirtualSize,			Misc.VirtualSize,		uint)
		NATIVE_WRAPPER_PROP			(VirtualAddress,		uint)
		NATIVE_WRAPPER_PROP			(SizeOfRawData,			uint)
		NATIVE_WRAPPER_PROP			(PointerToRawData,		uint)
		NATIVE_WRAPPER_PROP			(PointerToRelocations,	uint)
		NATIVE_WRAPPER_PROP			(PointerToLinenumbers,	uint)
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
		NATIVE_WRAPPED_ARRAY_BASICS(SectionHeaders, SectionHeader, IMAGE_SECTION_HEADER)
	public:
		property SectionHeader ^default[SectionName] { SectionHeader ^get(SectionName n) {
			for (int i = 0; i < this->n; ++i)
				if (n == this->a[i].Name)
					return this->Get(i);
			return nullptr;
		} }
		property SectionHeader ^default[string] { SectionHeader ^get(string n) { return this[SectionName(n)]; } }

		SectionHeader ^GetByRVA(uint rva) {
			for (int i = 0; i < this->n; ++i)
				if (this->a[i].VirtualAddress <= rva && rva < this->a[i].VirtualAddress + this->a[i].Misc.VirtualSize)
					return this->Get(i);
			return nullptr;
		}
		SectionHeader ^GetByVA(ulong va);
		SectionHeader ^Add(SectionName name, uint room, SectionCharacteristics chars);
		SectionHeader ^Add(string name, uint room, SectionCharacteristics chars) { return this->Add(SectionName(name), room, chars); }
	};

	public ref class File
	{
	private:
		byte *prev_data_pntr;
		Native::PE::File *f;

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
		void Init(Native::PE::File *f) {
			references = gcnew LinkedList<System::Runtime::InteropServices::GCHandle>();
			this->f = f;
			this->prev_data_pntr = this->f->get();
		}

	internal:
		void AddRef(UpdateMovableMemory ^umm) { this->references->AddLast(System::Runtime::InteropServices::GCHandle::Alloc(umm, System::Runtime::InteropServices::GCHandleType::Weak)); }
		inline int GetSectionHeaderIndex(IMAGE_SECTION_HEADER *s)	{ return (int)(s - this->f->getSectionHeader(0)); }
		inline int GetDataDirectoryIndex(IMAGE_DATA_DIRECTORY *d)	{ return (int)(d - this->f->getDataDirectory(0)); }
		void ExpandSectionHdr(IMAGE_SECTION_HEADER *s, uint room)	{
			IMAGE_SECTION_HEADER *n = this->f->getExpandedSectionHdr(this->GetSectionHeaderIndex(s), room);
			if (!n) throw gcnew Exception();
			this->Revalidate();
		}
		SectionHeader ^CreateSection(IMAGE_SECTION_HEADER *s, SectionName Name, uint room, SectionCharacteristics chars)	{
			char name[SectionName::LENGTH];
			Name.CopyTo(name);
			IMAGE_SECTION_HEADER *n = s == NULL ? this->f->createSection(name, room, (uint)chars) : this->f->createSection(this->GetSectionHeaderIndex(s), name, room, (uint)chars);
			if (!n) throw gcnew Exception();
			this->Revalidate();
			return gcnew SectionHeader(this, n);
		}

	public:
		//File(LPVOID data, size_t size, bool readonly) { this->Init(new Native::PE::File(?, ?, readonly)); }
		//File(LPVOID data, size_t size)			{ this->Init(new Native::PE::File(?, ?, false)); }
		File(string filename, bool readonly)		{ this->Init(new Native::PE::File(as_native(filename), readonly)); }
		File(string filename)						{ this->Init(new Native::PE::File(as_native(filename), false)); }
		~File()										{ delete f; f = NULL; }
		property bool IsLoaded						{ bool get() { return this->f->isLoaded(); } }
		property bool IsReadOnly					{ bool get() { return this->f->isReadOnly(); } }

		bool Save()									{ return this->f->save(); }
		
		property bool Is32Bit						{ bool get() { return this->f->is32bit(); } }
		property bool Is64Bit						{ bool get() { return this->f->is64bit(); } }
		property ulong ImageBase					{ ulong get() { return this->f->getImageBase(); } }

		property PE::FileHeader ^FileHeader			{ PE::FileHeader ^get() { return gcnew PE::FileHeader(this, this->f->getFileHeader()); } }
		property PE::NtHeaders32 ^NtHeaders32		{ PE::NtHeaders32 ^get() { return gcnew PE::NtHeaders32(this, this->f->getNtHeaders32()); } }
		property PE::NtHeaders64 ^NtHeaders64		{ PE::NtHeaders64 ^get() { return gcnew PE::NtHeaders64(this, this->f->getNtHeaders64()); } }

		property PE::DataDirectories ^DataDirectories	{ PE::DataDirectories ^get() { return gcnew PE::DataDirectories(this, this->f->getDataDirectory(0), this->f->getDataDirectoryCount()); } }
		property PE::SectionHeaders ^SectionHeaders		{ PE::SectionHeaders  ^get() { return gcnew PE::SectionHeaders (this, this->f->getSectionHeader(0), this->f->getSectionHeaderCount()); } }

		property uint Size { uint get() { return (uint)this->f->getSize(); } void set(uint x) { if (!this->f->setSize(x, false)) throw gcnew Exception(); this->Revalidate(); } }

		property RawData Data { RawData get() { DWORD n; byte *x = this->f->get(0, &n); return RawData(this, x, n); } }
/**/	bool Set(array<byte> ^lpBuffer, uint dwOffset)				{ return this->f->set(NATIVE(lpBuffer), dwOffset); }
/**/	bool Zero(uint dwSize, uint dwOffset)						{ return this->f->zero(dwSize, dwOffset); }
/**/	bool Move(uint dwOffset, uint dwSize, int dwDistanceToMove)	{ return this->f->move(dwOffset, dwSize, dwDistanceToMove); }
/**/	bool Shift(uint dwOffset, int dwDistanceToMove)				{ return this->f->shift(dwOffset, dwDistanceToMove); }
		bool Flush()												{ return this->f->flush(); }

		bool UpdatePEChkSum()										{ return this->f->updatePEChkSum(); }
		property bool HasExtraData									{ bool get() { return this->f->hasExtraData(); } }
/**/	//LPVOID GetExtraData(uint %size);	// pointer can modify the file
		bool ClearCertificateTable()								{ if (!this->f->clearCertificateTable()) throw gcnew Exception(); this->Revalidate(); return true; }
		property Version ^FileVersion								{ Version ^get() { ulong v = this->f->getFileVersion(); return gcnew Version((int)((v>>48)&0xFFFF), (int)((v>>32)&0xFFFF), (int)((v>>16)&0xFFFF), (int)(v&0xFFFF)); } }
		property bool ModifiedFlag { bool get() { return this->f->isAlreadyModified(); } void set(bool x) { if (!x) throw gcnew ArgumentException(); this->f->setModifiedFlag(); } }
		bool RemoveRelocs(uint start, uint end, bool reverse)		{ return this->f->removeRelocs(start, end, reverse); }
		bool RemoveRelocs(uint start, uint end)						{ return this->f->removeRelocs(start, end, false); }

		bool ResourceExists(ResID type, ResID name, ushort lang)		{ return this->f->resourceExists(type, name, lang); }
		//bool ResourceExists(ResID type, ResID name, ushort %lang)		{ return this->f->resourceExists(type, name, wptr(lang)); }
		array<byte> ^GetResource(ResID type, ResID name, ushort lang)	{ size_t size = 0; return ToManaged(this->f->getResource(type, name, lang, &size), size); }
		//array<byte> ^GetResource(ResID type, ResID name, ushort %lang)	{ size_t size = 0; return ToManaged(this->f->getResource(type, name, wptr(lang), &size), size); }
		bool RemoveResource(ResID type, ResID name, ushort lang)		{ return this->f->removeResource(type, name, lang); }
		bool AddResource(ResID type, ResID name, ushort lang, array<byte> ^data, Overwrite overwrite)	{ return this->f->addResource(type, name, lang, NATIVE(data), (uint)overwrite); }
		bool AddResource(ResID type, ResID name, ushort lang, array<byte> ^data)						{ return this->f->addResource(type, name, lang, NATIVE(data), OVERWRITE_ALWAYS); }

		//static LPVOID GetResourceDirect(LPVOID data, ResID type, ResID name); // must be freed, massively performance enhanced for a single retrieval and no editing // lang? size?
		//static bool UpdatePEChkSum(LPBYTE data, size_t dwSize, size_t peOffset, uint dwOldCheck);
		//static bool GetVersionInfo(LPVOID ver, string query, LPVOID %buffer, uint %len);
		//static VS_FIXEDFILEINFO *GetVersionInfo(LPVOID ver);
		static void UnmapAllViewsOfFile(string file)				{ Native::PE::File::UnmapAllViewsOfFile(as_native(file)); }
	};
}
