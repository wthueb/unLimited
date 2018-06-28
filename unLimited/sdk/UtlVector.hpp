#pragma once

#include "UtlMemory.hpp"

template<class T, class A = CUtlMemory<T>>
class CUtlVector
{
public:
	typedef A CAllocator;

	T& operator[](int i) {
		return m_Memory[i];
	};

	inline int Size()
	{
		return m_Size;
	}

protected:
	CAllocator m_Memory;
	int m_Size;
	T* m_pData;
};
