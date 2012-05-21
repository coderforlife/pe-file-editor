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

#include <stdlib.h>

using namespace System;
using namespace System::Collections::Generic;

namespace PE {
	ref class File; // forward declaration
namespace Utilities {
	static array<byte> ^ToManaged(void *x, size_t s)
	{
		array<byte> ^a = gcnew array<byte>((int)s);
		System::Runtime::InteropServices::Marshal::Copy(IntPtr(x), a, 0, (int)s);
		free(x);
		return a;
	}
	static DateTime ToDateTime(uint t) { return DateTime::FromFileTimeUtc(((__int64)((ulong)t * (ulong)10000000u)) + 116444736000000000); }
	static uint ToPEFileTime(DateTime d) { return (uint)((d.ToFileTimeUtc() - 116444736000000000) / 10000000u); }
	
	delegate void UpdateMovableMemory(ptrdiff_t dist);
	
	generic<class T>
	public ref class ArrayPointerBase abstract : IList<T>, ICollection<T>, IEnumerable<T>
	{
	protected:
		ref class Enumerator : IEnumerator<T>
		{
		protected:
			ArrayPointerBase<T> ^p;
			int i;
			property object System_Collections_IEnumerator_Current { virtual object get() sealed = System::Collections::IEnumerator::Current::get { return (T)((this->i < 0 || this->i >= p->n) ? nullptr : p->Get(i)); } };
		internal:
			Enumerator(ArrayPointerBase<T> ^p) : p(p), i(-1) { }
		public:
			property T Current { virtual T get () { return (T)((this->i < 0 || this->i >= p->n) ? nullptr : p->Get(i)); } }
			virtual bool MoveNext() { return ++this->i < p->n; }
			virtual void Reset() { this->i = -1; }
			~Enumerator() {}
		};
		File ^file;
		int n;
		virtual System::Collections::IEnumerator ^System_Collections_IEnumerable_GetEnumerator() sealed = System::Collections::IEnumerable::GetEnumerator { return gcnew Enumerator(this); }
		void Check(int i) { if (i < 0 || i >= this->n) throw gcnew IndexOutOfRangeException(); }
		virtual T Get(int i) = 0;			// does not bounds check
		virtual void Set(int i, T x) = 0;	// does not bounds check
	internal:
		ArrayPointerBase(File ^f, int n);
		virtual void UpdateMemory(ptrdiff_t dist) = 0;
	public:
		virtual IEnumerator<T> ^GetEnumerator() { return gcnew Enumerator(this); }
		property int Count			{ virtual int get() { return (int)this->n; } }
		property bool IsReadOnly	{ virtual bool get() { return true; } }
		property T default[int]		{
			virtual T get(int i)			{ Check(i); return Get(i); }
			virtual void set(int i, T x)	{ Check(i); Set(i, x); }
		}
		virtual bool Contains(T d) = 0;
		virtual int IndexOf(T d) = 0;
		virtual void CopyTo(array<T> ^a, int i) {
			if (a == nullptr)				{ throw gcnew ArgumentNullException(); }
			if (i < 0)						{ throw gcnew ArgumentOutOfRangeException(); }
			if (a->Length + i > this->n)	{ throw gcnew ArgumentException(); }
			for (int j = 0; j < this->n; ++i)	a[i+j] = Get(i);
		}

		virtual void Add(T)			{ throw gcnew NotSupportedException(); }
		virtual void Clear()		{ throw gcnew NotSupportedException(); }
		virtual void Insert(int,T)	{ throw gcnew NotSupportedException(); }
		virtual bool Remove(T)		{ throw gcnew NotSupportedException(); }
		virtual void RemoveAt(int)	{ throw gcnew NotSupportedException(); }
	};
} }
