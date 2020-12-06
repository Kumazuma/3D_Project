#pragma once
#include<memory>
#include<xmemory>
namespace Kumazuma
{
	template<typename COMClassT>
	class ComWrapper
	{
	public:
		ComWrapper(std::nullptr_t = nullptr);
		ComWrapper(COMClassT*& ptr);
		ComWrapper(COMClassT*&& ptr);
		ComWrapper(const ComWrapper&) = delete;
		ComWrapper(ComWrapper&&)noexcept;
		void operator =(const ComWrapper&) = delete;
		void operator =(ComWrapper&&) noexcept;
		~ComWrapper();
		COMClassT* operator->() const;
		COMClassT** operator&();
		operator COMClassT* ()const;
		
	private:
		COMClassT* m_ptr;
	};

	
	template<typename COMClassT>
	inline ComWrapper<COMClassT>::ComWrapper(std::nullptr_t)
		:m_ptr{nullptr}
	{
	}

	template<typename COMClassT>
	inline ComWrapper<COMClassT>::ComWrapper(COMClassT*& ptr)
		: m_ptr{ ptr }
	{
		ptr = nullptr;
	}

	template<typename COMClassT>
	inline ComWrapper<COMClassT>::ComWrapper(COMClassT*&& ptr)
		:m_ptr{ptr}
	{
		ptr = nullptr;
	}

	template<typename COMClassT>
	inline ComWrapper<COMClassT>::ComWrapper(ComWrapper&& other) noexcept:
		m_ptr{other.m_ptr}
	{
		other.m_ptr = nullptr;
	}

	template<typename COMClassT>
	inline void ComWrapper<COMClassT>::operator=(ComWrapper&& other) noexcept
	{
		ComWrapper tmp{ std::move(other) };
		this->~ComWrapper();
		new(this) ComWrapper{ std::move(tmp) };
	}

	template<typename COMClassT>
	inline ComWrapper<COMClassT>::~ComWrapper()
	{
		if (m_ptr != nullptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}

	template<typename COMClassT>
	inline COMClassT* ComWrapper<COMClassT>::operator->() const
	{
		return m_ptr;
	}

	template<typename COMClassT>
	inline COMClassT** ComWrapper<COMClassT>::operator&()
	{
		return &m_ptr;
	}

	template<typename COMClassT>
	inline ComWrapper<COMClassT>::operator COMClassT* () const
	{
		return m_ptr;
	}

}