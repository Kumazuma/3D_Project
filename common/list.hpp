#pragma once
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>
#include<xmemory>
namespace Kumazuma
{
	template<typename Container>
	class ConstLinkedListIterator
	{
	protected:
		using node_type = typename Container::node_type;
		friend Container;
	public:
		using value_type = typename Container::value_type;
	protected:
		ConstLinkedListIterator(node_type* pNode, const Container* const pContainer, size_t index = 0) :m_pNode{ pNode }, m_pContainer{ pContainer }, index{ index } {};
	public:
		ConstLinkedListIterator() {};
		const value_type& operator*()const
		{
			return m_pNode->value;
		}
		const value_type* operator->()const
		{
			return &m_pNode->value;
		}
		ConstLinkedListIterator& operator ++()
		{
			assert(m_pNode != m_pContainer->m_pBackDummy);
			m_pNode = m_pNode->pNext;

			++index;

			return *this;
		}
		ConstLinkedListIterator& operator --()
		{
			assert(m_pNode->pPrev != m_pContainer->m_pFrontDummy);
			m_pNode = m_pNode->pPrev;

			--index;

			return *this;
		}
		bool operator ==(const ConstLinkedListIterator& other)
		{
			return other.m_pNode == m_pNode;
		}
		bool operator !=(const ConstLinkedListIterator& other)
		{
			return other.m_pNode != m_pNode;
		}
		void operator = (const ConstLinkedListIterator& other)
		{
			ConstLinkedListIterator copy{ other.m_pNode, other.m_pContrainer };
			m_pNode = copy.m_pNode;
			m_pContainer = copy.m_pContrainer;
		}
	public:
		size_t index;
		node_type* m_pNode;
		const Container* m_pContainer;
	};
	template<typename Container>
	class LinkedListIterator :public ConstLinkedListIterator<Container>
	{
		using my_base = ConstLinkedListIterator<Container>;
		friend Container;
	protected:
		using node_type = typename ConstLinkedListIterator<Container>::node_type;
	public:
		using value_type = typename ConstLinkedListIterator<Container>::value_type;
	protected:
		LinkedListIterator(node_type* pNode, const Container* const pContainer, size_t index = 0) :ConstLinkedListIterator<Container>{ pNode,pContainer,index } {};
	public:
		LinkedListIterator() {};
		void operator = (const LinkedListIterator& other)
		{
			my_base::m_pNode = other.m_pNode;
			my_base::m_pContainer = other.m_pContainer;
		}
		value_type& operator*()const
		{
			return my_base::m_pNode->value;
		}
		value_type* operator->()const
		{
			return &my_base::m_pNode->value;
		}
		LinkedListIterator& operator ++()
		{
			ConstLinkedListIterator<Container>::operator++();
			return *this;
		}
		LinkedListIterator& operator --()
		{
			ConstLinkedListIterator<Container>::operator--();
			return *this;
		}
		bool operator ==(const LinkedListIterator& other)
		{
			return ConstLinkedListIterator<Container>::operator==(other);
		}
		bool operator !=(const LinkedListIterator& other)
		{
			return ConstLinkedListIterator<Container>::operator!=(other);
		}
	};


	template<typename T, size_t CHUNK_SIZE = 16>
	class LinkedList
	{
		struct NodeChunk;
		struct Node
		{
			Node* pPrev;
			Node* pNext;
			NodeChunk* pChunk;
			T value;
		};
		struct NodeChunk
		{
			NodeChunk* pPrev;
			Node nodes[CHUNK_SIZE];
			int32_t count;
			NodeChunk* pNext;
		};

	public:
		using node_type = Node;
		using value_type = T;
		friend class ConstLinkedListIterator<LinkedList>;
		friend class LinkedListIterator<LinkedList>;
		using const_iterator = ConstLinkedListIterator<LinkedList>;
		using iterator = LinkedListIterator<LinkedList>;
	public:
		LinkedList();
		LinkedList(const LinkedList& other) :LinkedList{ other.begin(), other.end() } {}
		template<size_t _CH_SIZE>
		LinkedList(const LinkedList<T, _CH_SIZE>& other);
		template<typename It>
		LinkedList(It, It);

		LinkedList(LinkedList&&) noexcept;
		~LinkedList();
		void operator =(LinkedList&& other) noexcept;
		void operator =(const LinkedList& other) ;
		void Insert(const T& val);
		const_iterator begin() const { return const_iterator{ m_pFrontDummy->pNext, this, 0}; }
		const_iterator end() const { return const_iterator{ m_pBackDummy, this, m_nodeCount }; }
		iterator begin() { return iterator{ m_pFrontDummy->pNext, this, 0  }; }
		iterator end() { return iterator{ m_pBackDummy, this, m_nodeCount }; }
		iterator erase(iterator where) { return iterator{ erase(where.m_pNode), this,where.index + 1 }; }
		const_iterator erase(const_iterator where) { return const_iterator{ erase(where.m_pNode), this,where.index + 1 }; }
		size_t size() const { return m_nodeCount; }
		void push_back(const T& val) { Insert(val); }
		void push_back(T&& val);
		iterator insert(iterator where, const T& val) { return { Insert(where.m_pNode, val), this }; }
		const_iterator insert(const_iterator where, const T& val) { return { Insert(where.m_pNode, val), this }; }
		value_type& front() { return m_pFrontDummy->pNext->value; }
		value_type& back() { return m_pBackDummy->pPrev->value; }
		void pop_front() { erase(begin()); }
		void sort() { sort(m_pFrontDummy, m_pBackDummy); }
		bool empty() const { return m_nodeCount == 0; }
		void clear();
	private:
		void sort(Node* begin, Node* end);
		Node* Insert(Node* where, const T& val);
		Node* GetNewNode();
		Node* GetEmptyNode(NodeChunk* chunk);
		Node* erase(Node* where);
	private:
		NodeChunk* m_pChunkHead;
		NodeChunk* m_pChunkTail;
		size_t m_nodeCount;
		size_t m_chunkCount;
		Node* m_pFrontDummy;
		Node* m_pBackDummy;
	};

	template<typename T, size_t CHUNK_SIZE> inline LinkedList<T, CHUNK_SIZE>::LinkedList() :
		m_pChunkHead{ nullptr },
		m_pChunkTail{ nullptr },
		m_chunkCount{ 0 },
		m_nodeCount{ 0 },
		m_pFrontDummy{ nullptr },
		m_pBackDummy{ nullptr }

	{
		//우선 백 더미에 집어넣을 초기화 안 된 Node 객체 두 개를 만든다.
		m_pFrontDummy = (Node*)malloc(sizeof(Node) * 2);
		m_pBackDummy = m_pFrontDummy + 1;
		memset(m_pFrontDummy, 0, sizeof(Node) * 2);
		//서로 이어준다. [front] <-> [back]
		m_pFrontDummy->pNext = m_pBackDummy;
		m_pFrontDummy->pPrev = nullptr;
		m_pBackDummy->pPrev = m_pFrontDummy;
		m_pBackDummy->pNext = nullptr;
	}
	template<typename T, size_t CHUNK_SIZE>
	inline LinkedList<T, CHUNK_SIZE>::LinkedList(LinkedList&& other) noexcept
	{
		m_pChunkHead = other.m_pChunkHead;
		m_pChunkTail = other.m_pChunkTail;
		m_nodeCount = other.m_nodeCount;
		m_chunkCount = other.m_chunkCount;
		m_pFrontDummy = other.m_pFrontDummy;
		m_pBackDummy = other.m_pBackDummy;
		new(&other) LinkedList{};
	}
	template<typename T, size_t CHUNK_SIZE> inline LinkedList<T, CHUNK_SIZE>::~LinkedList()
	{
		if (m_pFrontDummy == nullptr)
		{
			return;
		}
		Node* pNode = m_pFrontDummy->pNext;
		while (pNode != m_pBackDummy)
		{
			assert(pNode->pChunk != nullptr);
			if (pNode->pChunk != nullptr)
			{
				NodeChunk* pChunk = pNode->pChunk;
				--pChunk->count;
			}
			pNode->value.~T();
			pNode->pPrev = nullptr;
			pNode->pChunk = nullptr;
			pNode = pNode->pNext;
		}
		NodeChunk* pChunk = m_pChunkHead;
		//해제 안된 노드들을 마지막으로 정리한다.
		while (pChunk != nullptr)
		{
			assert(pChunk->count == 0);
			if (pChunk->count != 0)
			{
				Node(&nodes)[CHUNK_SIZE] = pChunk->nodes;
				for (Node* node = nodes; node != nodes + CHUNK_SIZE; ++node)
				{
					if (pNode->pChunk != nullptr)
					{
						node->value.~T();
						break;
					}
				}
			}
			NodeChunk* temp = pChunk->pNext;
			free(pChunk);
			pChunk = temp;
		}
		free(m_pFrontDummy);
	}
	template<typename T, size_t CHUNK_SIZE>
	inline auto LinkedList<T, CHUNK_SIZE>::GetEmptyNode(NodeChunk* chunk)->Node*
	{
		assert(chunk != nullptr);
		Node(&nodes)[CHUNK_SIZE] = chunk->nodes;
		Node* node = nodes;
		for (; node != nodes + CHUNK_SIZE; ++node)
		{
			if (node->pChunk == nullptr)
			{
				break;
			}
		}
		assert(node != nodes + CHUNK_SIZE);
		++chunk->count;
		memset(node, 0, sizeof(Node));
		node->pChunk = chunk;
		return node;
	}
	template<typename T, size_t CHUNK_SIZE> inline auto LinkedList<T, CHUNK_SIZE>::GetNewNode()->Node*
	{
		NodeChunk* pChunkNode = nullptr;
		pChunkNode = m_pChunkTail;
		while (pChunkNode != nullptr)
		{
			assert(pChunkNode != nullptr);
			if (pChunkNode != nullptr && pChunkNode->count < CHUNK_SIZE)
			{
				return GetEmptyNode(pChunkNode);
			}
			pChunkNode = pChunkNode->pPrev;
		}
		//빈 청크가 없으면 청크를 새로 만든다.
		++m_chunkCount;
		NodeChunk* newChunk = (NodeChunk*)malloc(sizeof(NodeChunk));
		memset(newChunk, 0, sizeof(NodeChunk));
		if (m_pChunkTail != nullptr)
		{
			m_pChunkTail->pNext = newChunk;
		}
		if (m_pChunkHead == nullptr)
		{
			m_pChunkHead = newChunk;
		}
		newChunk->pPrev = m_pChunkTail;
		m_pChunkTail = newChunk;
		return GetEmptyNode(newChunk);
	}
	template<typename T, size_t CHUNK_SIZE> inline void LinkedList<T, CHUNK_SIZE>::Insert(const T& val)
	{
		Node* pNewNode = GetNewNode();
		++m_nodeCount;
		new(&pNewNode->value) T{std::forward<const T&>(val) };
		pNewNode->pNext = m_pBackDummy;//맨 끝 노드의 back은 항상 back dummy를 가리키노록 만든다.
		pNewNode->pPrev = m_pBackDummy->pPrev;
		m_pBackDummy->pPrev->pNext = pNewNode;
		m_pBackDummy->pPrev = pNewNode;
		// [a node]<->[back dummy]에서
		// [a node]<-[a new node]->[back dummy]
		// [a node]<->[a new node]->[back dummy]
	}
	template<typename T, size_t CHUNK_SIZE> inline void LinkedList<T, CHUNK_SIZE>::push_back(T&& val)
	{
		Node* pNewNode = GetNewNode();
		++m_nodeCount;
		new(&pNewNode->value) T{ std::forward<T&&>(val) };
		pNewNode->pNext = m_pBackDummy;//맨 끝 노드의 back은 항상 back dummy를 가리키노록 만든다.
		pNewNode->pPrev = m_pBackDummy->pPrev;
		m_pBackDummy->pPrev->pNext = pNewNode;
		m_pBackDummy->pPrev = pNewNode;
	}
	template<typename T> inline void node_remove(T* pNode)
	{
		if (pNode->pPrev != nullptr)
		{
			pNode->pPrev->pNext = pNode->pNext;
		}
		if (pNode->pNext != nullptr)
		{
			pNode->pNext->pPrev = pNode->pPrev;
		}
		pNode->pNext = nullptr;
		pNode->pPrev = nullptr;
	}
	template<typename T, size_t CHUNK_SIZE>
	inline auto LinkedList<T, CHUNK_SIZE>::erase(Node* where)->Node*
	{
		if (where == nullptr)
		{
			return nullptr;
		}
		if (where == m_pBackDummy || where == m_pFrontDummy)
		{
			return where;
		}
		assert(where->pChunk != nullptr);
		NodeChunk* pChunk = where->pChunk;
		size_t index = where - pChunk->nodes;
		Node* res = where->pNext;
		//우선 노드를 이어준다.
		node_remove(where);
		//소멸자를 호출한다.
		where->value.~T();
		//안 쓴다고 플래그를 설정해주고
		memset(where, 0, sizeof(Node));
		--pChunk->count;
		--m_nodeCount;
		//카운트를 줄인다.
		if (pChunk->count <= 0)
		{
			//만약에 카운트가 0이면 해당 청크를 해제해 준다.
			if (pChunk == m_pChunkTail)
			{
				m_pChunkTail = pChunk->pPrev;
			}
			if (pChunk == m_pChunkHead)
			{
				m_pChunkHead = pChunk->pNext;
			}
			node_remove(pChunk);
			free(pChunk);

			--m_chunkCount;
		}
		if (m_chunkCount == 0)
		{
			m_pChunkHead = nullptr;
			m_pChunkTail = nullptr;
		}
		return res;
	}
	template<typename T, size_t CHUNK_SIZE>
	inline void LinkedList<T, CHUNK_SIZE>::clear()
	{
		//auto tmp{ std::move(*this) };
		this->~LinkedList();
		new(this) LinkedList{};
	}
	template<typename T, size_t CHUNK_SIZE>
	void LinkedList<T, CHUNK_SIZE>::sort(Node* begin, Node* end)
	{
		if (begin->pNext == end)
		{
			return;
		}
		Node* pivot = begin->pNext;
		for (Node* it = begin->pNext; it != end; it = it->pNext)
		{
			//작으면 왼쪽.
			if (it->value < pivot->value)
			{
				Node* pivotLeft = pivot->pPrev;
				it->pPrev->pNext = it->pNext;
				it->pNext->pPrev = it->pPrev;
				pivotLeft->pNext = it;
				it->pPrev = pivotLeft;
				pivot->pPrev = it;
				it->pNext = pivot;
			}
		}

	}
	template<typename T, size_t CHUNK_SIZE>
	inline auto LinkedList<T, CHUNK_SIZE>::Insert(Node* where, const T& val)->Node*
	{
		Node* pNewNode = GetNewNode();
		++m_nodeCount;
		//1.
		//[a prev]<->[target]<->[a next]
		//2.
		//[a prev]<->[target]<->[a next]
		//[a new ] ->
		pNewNode->pNext = where;
		/*3.
		[a prev]<------------>[target]<->[a next]
				<- [a new ] ->
		*/
		pNewNode->pPrev = where->pPrev;
		/*
		4.
		[a prev]<->[a new]->[target]<->[a next]
				<-----------
		*/
		pNewNode->pPrev->pNext = pNewNode;
		/*
		5.
		[a prev]<->[a new]<->[target]<->[a next]
		*/
		where->pPrev = pNewNode;
		new(&pNewNode->value) T{ val };
		return pNewNode;
	}
	template<typename T, size_t CHUNK_SIZE>
	template<typename It>
	inline LinkedList<T, CHUNK_SIZE>::LinkedList(It begin, It end) :
		LinkedList{}
	{
		for (; begin != end; ++begin)
		{
			push_back(*begin);
		}
	}
	template<typename T, size_t CHUNK_SIZE>
	template<size_t _CH_SIZE>
	inline LinkedList<T, CHUNK_SIZE>::LinkedList(const LinkedList<T, _CH_SIZE>& other) :
		LinkedList{ other.begin(), other.end() }
	{
	}
	template<typename T, size_t CHUNK_SIZE>
	inline void LinkedList<T, CHUNK_SIZE>::operator =(LinkedList&& other) noexcept
	{
		this->~LinkedList();
		new(this) LinkedList{ std::move(other) };
	}
	template<typename T, size_t CHUNK_SIZE>
	inline void LinkedList<T, CHUNK_SIZE>::operator =(const LinkedList& other) 
	{
		this->~LinkedList();
		new(this) LinkedList{ other.begin(), other.end() };
	}
}