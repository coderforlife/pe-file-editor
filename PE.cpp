#include "PE.h"

using namespace PE;
using namespace PE::Utilities;

generic<class T>
ArrayPointerBase<T>::ArrayPointerBase(File ^f, int n) : file(f), n(n) { f->AddRef(gcnew UpdateMovableMemory(this, &ArrayPointerBase::UpdateMemory)); }

RawData::RawData(File ^f, byte *x, uint n) : file(f), x(x), n(n) { f->AddRef(gcnew UpdateMovableMemory(*this, &RawData::UpdateMemory)); }

#undef NATIVE_WRAPPER_BASICS
#define NATIVE_WRAPPER_BASICS(C, NT) /* C is the managed class to create, NT is a native type */ \
	C::C(File ^f, NT *n) : file(f), n(n) { f->AddRef(gcnew UpdateMovableMemory(this, &C::UpdateMemory)); }

NATIVE_WRAPPER_BASICS(FileHeader,		IMAGE_FILE_HEADER)
NATIVE_WRAPPER_BASICS(DataDirectory,	IMAGE_DATA_DIRECTORY)
NATIVE_WRAPPER_BASICS(OptionalHeader32,	IMAGE_OPTIONAL_HEADER32)
NATIVE_WRAPPER_BASICS(OptionalHeader64,	IMAGE_OPTIONAL_HEADER64)
NATIVE_WRAPPER_BASICS(NtHeaders32,		IMAGE_NT_HEADERS32)
NATIVE_WRAPPER_BASICS(NtHeaders64,		IMAGE_NT_HEADERS64)

NATIVE_WRAPPER_BASICS(SectionHeader,	IMAGE_SECTION_HEADER)
int SectionHeader::Index::get()			{ return this->file->GetSectionHeaderIndex(this->n); }
void SectionHeader::Expand(uint room)	{ this->file->ExpandSectionHdr(this->n, room); }
SectionHeader ^SectionHeader::AddBefore(SectionName name, uint room, SectionCharacteristics chars)	{ return this->file->CreateSection(this->n, name, room, chars); } // increment section header count?

SectionHeader ^SectionHeaders::GetByVA(ulong va) { return this->GetByRVA((uint)(va-this->file->ImageBase)); }
SectionHeader ^SectionHeaders::Add(SectionName name, uint room, SectionCharacteristics chars) { SectionHeader ^s = this->file->CreateSection(NULL, name, room, chars); if (s != nullptr) { ++this->n; } return s; }
