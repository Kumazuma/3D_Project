#pragma once
#include<xmemory>

template<typename COMType>
class COMPtr
{
	
public:
	explicit COMPtr(COMType*&& ptr) noexcept :
		m_ptr{ ptr }
	{
		m_ptr->AddRef();
		ptr = nullptr;
	}
	COMPtr(std::nullptr_t) :m_ptr{ nullptr } {}
	COMPtr() :
		m_ptr{ nullptr }
	{

	}
	COMPtr(COMPtr&& other) noexcept :
		m_ptr{ other.m_ptr }
	{
		other.m_ptr = nullptr;
	}
	COMPtr(const COMPtr& other) :
		m_ptr{ other.m_ptr }
	{
		if (m_ptr != nullptr)
		{
			m_ptr->AddRef();
		}
	}
	~COMPtr()
	{
		if (m_ptr != nullptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}
	void operator =(const COMPtr& other)
	{
		COMPtr tmp{ other };
		this->~COMPtr();
		new(this) COMPtr{ std::move(tmp) };
	}
	void operator =(COMPtr&& other) noexcept
	{
		COMPtr tmp{ std::move(other) };
		this->~COMPtr();
		new(this) COMPtr{ std::move(tmp) };
	}
	COMType* Reset(COMType* tmp)
	{
		COMType* old{ m_ptr };
		m_ptr = tmp;
		return old;
	}
	COMType** GetPPtrForInit()
	{
		if (m_ptr != nullptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
		return &m_ptr;
	}
	COMType* Get() const
	{
		return m_ptr;
	}
	bool operator == (std::nullptr_t) const
	{
		return m_ptr == nullptr;
	}
	bool operator != (std::nullptr_t) const
	{
		return m_ptr != nullptr;
	}
	COMType* operator ->()const
	{
		return m_ptr;
	}
	operator COMPtr<const COMPtr>() const
	{
		COMPtr<const COMPtr> tmp;
		tmp.Reset(m_ptr);
		m_ptr->AddRef();
		return tmp;
	}
	template<typename _COM>
	operator COMPtr<_COM>&()
	{
		_COM* tmp = static_cast<_COM*>(m_ptr);
		return *reinterpret_cast<COMPtr<_COM>*>(this);
	}
	COMType** operator &()
	{
		if (m_ptr != nullptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
		return &m_ptr;
	}
	unsigned long Release()
	{
		unsigned long refCount{ 0 };
		if (m_ptr != nullptr)
		{
			refCount = m_ptr->Release();
			m_ptr = nullptr;
		}
		return refCount;
	}
private:
	COMType* m_ptr;
};
template<typename ComT>
COMPtr<ComT> MakeCOMPtr(ComT*&& ptr)
{
	return COMPtr<ComT>{std::forward<ComT*>(ptr)};
}