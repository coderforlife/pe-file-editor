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

#include "PE.h"

using namespace PE::Editor;
using namespace PE::Editor::Utilities;

generic<class T>
ArrayPointerBase<T>::ArrayPointerBase(File ^f, int n) : file(f), n(n) { f->AddRef(gcnew UpdateMovableMemory(this, &ArrayPointerBase::UpdateMemory)); }

RawData::RawData(File ^f, byte *x, uint n) : /*file(f),*/ x(x), n(n), readOnly(f->IsReadOnly) { f->AddRef(gcnew UpdateMovableMemory(this, &RawData::UpdateMemory)); }

#undef NATIVE_WRAPPER_BASICS
#define NATIVE_WRAPPER_BASICS(C, NT) /* C is the managed class to create, NT is a native type */ \
	C::C(File ^f, NT *n) : file(f), n(n) { f->AddRef(gcnew UpdateMovableMemory(this, &C::UpdateMemory)); }

NATIVE_WRAPPER_BASICS(FileHeader,		PE::Image::FileHeader)
NATIVE_WRAPPER_BASICS(DataDirectory,	PE::Image::DataDirectory)
NATIVE_WRAPPER_BASICS(OptionalHeader32,	PE::Image::OptionalHeader32)
NATIVE_WRAPPER_BASICS(OptionalHeader64,	PE::Image::OptionalHeader64)
NATIVE_WRAPPER_BASICS(NtHeaders32,		PE::Image::NTHeaders32)
NATIVE_WRAPPER_BASICS(NtHeaders64,		PE::Image::NTHeaders64)

NATIVE_WRAPPER_BASICS(SectionHeader,	PE::Image::SectionHeader)
int SectionHeader::Index::get()			{ return this->file->GetSectionHeaderIndex(this->n); }
void SectionHeader::Expand(uint room)	{ this->file->ExpandSectionHdr(this->n, room); }
SectionHeader ^SectionHeader::AddBefore(SectionName name, uint room, SectionCharacteristics chars)	{ return this->file->CreateSection(this->n, name, room, chars); } // increment section header count?

SectionHeader ^SectionHeaders::GetByVA(ulong va) { return this->GetByRVA((uint)(va-this->file->ImageBase)); }
SectionHeader ^SectionHeaders::Add(SectionName name, uint room, SectionCharacteristics chars) { SectionHeader ^s = this->file->CreateSection(NULL, name, room, chars); if (s != nullptr) { ++this->n; } return s; }
