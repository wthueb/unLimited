#pragma once

template <class T, class I = int>
class CUtlMemory
{
public:
    T& operator[](int i) {
        return m_pMemory[i];
    };

    T* m_pMemory;
    int m_nAllocationCount;
    int m_nGrowSize;
};
