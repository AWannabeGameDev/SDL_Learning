#ifndef CLE_FREELIST_H
#define CLE_FREELIST_H

#include <new>

template<typename Type>
class Freelist {

private :

	struct m_Node {

		Type value;
		m_Node* prev;
		m_Node* next;

	};

	struct m_Arena {

		m_Node* start;
		size_t size;
		m_Arena* next;

	};

	size_t m_size; size_t m_capacity;
	m_Arena* m_headArena; m_Arena* m_lastArena;
	m_Node* m_stackPtr; m_Node m_headNode; m_Node* m_lastNode; m_Node* m_virtualLastNode;

	void f_setLastNode() {

		if(!m_lastNode->next) {

			if(m_size == m_capacity) {

				m_lastArena->next = new m_Arena{(m_Node*)::operator new(m_capacity * sizeof(m_Node)), 2 * m_capacity, nullptr};
				m_lastArena = m_lastArena->next;
				m_stackPtr = m_lastArena->start;

				m_capacity *= 2;

			}

			m_lastNode = m_stackPtr++;
			m_virtualLastNode = m_lastNode;

		} else m_lastNode = m_lastNode->next;

	}

public :

	class Iterator {

		friend class Freelist<Type>;

	private :

		m_Node* m_ptr;

	public : 

		bool operator==(Iterator other) {

			return m_ptr == other.m_ptr;

		}

		bool operator!=(Iterator other) {

			return m_ptr != other.m_ptr;

		}

		Iterator operator++() {

			m_ptr = m_ptr->next;
			return *this;

		}

		Iterator operator++(int) {

			Iterator ret = *this;
			m_ptr = m_ptr->next;
			return ret;

		}

		Iterator operator--() {

			m_ptr = m_ptr->prev;
			return *this;

		}

		Iterator operator--(int) {

			Iterator ret = *this;
			m_ptr = m_ptr->prev;
			return ret;

		}

		Type& operator*() {

			return m_ptr->value;

		}

		Type* operator->() {

			return &m_ptr->value;

		}

		Iterator next() {

			Iterator ret{};
			ret.m_ptr = m_ptr->next;
			return ret;

		}

		Iterator prev() {

			Iterator ret{};
			ret.m_ptr = m_ptr->prev;
			return ret;

		}

	};

	Freelist(size_t initCap) :

		m_size(0), m_capacity(initCap),
		m_headArena(new m_Arena{(m_Node*)::operator new(initCap * sizeof(m_Node)), initCap, nullptr}), 
		m_lastArena(m_headArena),
		m_stackPtr(m_headArena->start), m_headNode{}, m_lastNode(&m_headNode), m_virtualLastNode(m_lastNode) {}

	~Freelist() {

		for(m_Arena* arena = m_headArena; arena != nullptr; ) {

			::operator delete(arena->start);
			m_Arena* next = arena->next;
			delete arena;
			arena = next;

		}

	}

	Type* add(Type& elem) {

		m_Node* prevNode = m_lastNode;
		f_setLastNode();
		new (m_lastNode) m_Node{elem, prevNode, nullptr};

		prevNode->next = m_lastNode;

		m_size++;
		return &m_lastNode->value;

	}

	Type* add(Type&& elem) {

		m_Node* prevNode = m_lastNode;
		f_setLastNode();
		new (m_lastNode) m_Node{std::move(elem), prevNode, nullptr};

		prevNode->next = m_lastNode;

		m_size++;
		return &m_lastNode->value;

	}

	template<typename... ArgTypes>
	Type* addInPlace(ArgTypes&&... args) {

		m_Node* prevNode = m_lastNode;
		f_setLastNode();
		new (m_lastNode) m_Node{{std::forward<ArgTypes>(args)...}, prevNode, nullptr};

		prevNode->next = m_lastNode;

		m_size++;
		return &m_lastNode->value;

	}

	void remove(Type* removePtr) {

		m_Node* toRemove = (m_Node*)removePtr;

		if(toRemove->next) {

			toRemove->prev->next = toRemove->next;
			toRemove->next->prev = toRemove->prev;
			toRemove->next = nullptr;
			toRemove->prev = nullptr;

			m_virtualLastNode->next = toRemove;
			m_virtualLastNode = toRemove;

		} else {

			m_Node* newLast = m_lastNode->prev;
			newLast->next = nullptr;
			m_lastNode->prev = nullptr;
			m_lastNode = newLast;

			m_virtualLastNode = m_lastNode;

		}

		m_size--;

	}

	Iterator begin() {

		Iterator ret{};
		ret.m_ptr = m_headNode.next;
		return ret;

	}

	Iterator end() {

		Iterator ret{};
		ret.m_ptr = m_lastNode->next;
		return ret;

	}

};

#endif