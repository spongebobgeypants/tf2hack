#pragma once
#include <Windows.h>
#include <memory>
#include <Psapi.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include <unordered_map>

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

class CVMTHookManager
{
public:
	CVMTHookManager()
	{
		memset(this, 0, sizeof(CVMTHookManager));
	}

	CVMTHookManager(PDWORD* ppdwClassBase)
	{
		bInitialize(ppdwClassBase);
	}

	~CVMTHookManager()
	{
		UnHook();
	}

	bool bInitialize(PDWORD* ppdwClassBase)
	{
		m_ppdwClassBase = ppdwClassBase;
		m_pdwOldVMT = *ppdwClassBase;
		m_dwVMTSize = dwGetVMTCount(*ppdwClassBase);
		m_pdwNewVMT = new DWORD[m_dwVMTSize];
		memcpy(m_pdwNewVMT, m_pdwOldVMT, sizeof(DWORD) * m_dwVMTSize);
		*ppdwClassBase = m_pdwNewVMT;
		return true;
	}

	bool bInitialize(PDWORD** pppdwClassBase)
	{
		return bInitialize(*pppdwClassBase);
	}

	void UnHook()
	{
		if (m_ppdwClassBase)
		{
			*m_ppdwClassBase = m_pdwOldVMT;
		}
	}

	void ReHook()
	{
		if (m_ppdwClassBase)
		{
			*m_ppdwClassBase = m_pdwNewVMT;
		}
	}

	int iGetFuncCount()
	{
		return (int)m_dwVMTSize;
	}

	DWORD dwGetMethodAddress(int Index)
	{
		if (Index >= 0 && Index <= (int)m_dwVMTSize && m_pdwOldVMT != NULL)
		{
			return m_pdwOldVMT[Index];
		}
		return NULL;
	}

	PDWORD pdwGetOldVMT()
	{
		return m_pdwOldVMT;
	}

	DWORD dwHookMethod(DWORD dwNewFunc, unsigned int iIndex)
	{
		if (m_pdwNewVMT && m_pdwOldVMT && iIndex <= m_dwVMTSize && iIndex >= 0)
		{
			m_pdwNewVMT[iIndex] = dwNewFunc;
			return m_pdwOldVMT[iIndex];
		}

		return NULL;
	}

private:
	DWORD dwGetVMTCount(PDWORD pdwVMT)
	{
		DWORD dwIndex = 0;

		for (dwIndex = 0; pdwVMT[dwIndex]; dwIndex++)
		{
			if (IsBadCodePtr((FARPROC)pdwVMT[dwIndex]))
			{
				break;
			}
		}
		return dwIndex;
	}
	PDWORD*	m_ppdwClassBase;
	PDWORD	m_pdwNewVMT, m_pdwOldVMT;
	DWORD	m_dwVMTSize;
};

class CVMTHookManager2
{
public:
	bool IsCodePtr(void* ptr)
	{
		constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		MEMORY_BASIC_INFORMATION out;
		VirtualQuery(ptr, &out, sizeof out);

		return out.Type
			&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			&& out.Protect & protect_flags;
	}

	CVMTHookManager2(void* classptr)
	{
		this->m_class_pointer = reinterpret_cast<void***>(classptr);
		m_old_vmt = *m_class_pointer;

		size_t table_size = 0;
		while (m_old_vmt[table_size] && IsCodePtr(m_old_vmt[table_size]))
			table_size++;

		m_new_vmt = new void*[table_size + 1];
		memcpy(&m_new_vmt[1], m_old_vmt, sizeof(void*) * table_size);
		m_new_vmt[0] = m_old_vmt[-1];

		*m_class_pointer = &m_new_vmt[1];
	}

	~CVMTHookManager2()
	{
		*m_class_pointer = m_old_vmt;
		delete[] m_new_vmt;
	}

	template<typename fn = void*>
	fn hook(size_t index, void* new_function)
	{
		if (new_function)
			m_new_vmt[index + 1] = new_function;
		return reinterpret_cast<fn>(m_old_vmt[index]);
	}

private:
	void*** m_class_pointer = nullptr;
	void** m_old_vmt = nullptr;
	void** m_new_vmt = nullptr;
};