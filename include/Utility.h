#ifndef UTILITY_H
#define UTILITY_H


#include <Config.h>
#include <string>
#include <EString.h>
#include <Debug.h>

namespace Easy2D
{
namespace Utility
{

/////////////////////////////////////////////
//function and wrapper for reverse foreach
template<class T>
class const_reverse_wrapper
{
    const T& container;

public:
    const_reverse_wrapper(const T& cont) : container(cont) { }
    auto begin() const  ->  decltype(container.rbegin())
    {
        return container.rbegin();
    }
    auto end() const  ->  decltype(container.rbegin())
    {
        return container.rend();
    }
};
template<class T>
class reverse_wrapper
{
    T& container;

public:
    reverse_wrapper(T& cont) : container(cont) { }
    auto begin() ->  decltype(container.rbegin())
    {
        return container.rbegin();
    }
    auto end() ->  decltype(container.rend())
    {
        return container.rend();
    }
};
/////////////////////////////////////////////
template<class T>
const_reverse_wrapper<T> reverse(const T& cont)
{
    return const_reverse_wrapper<T>(cont);
}
template<class T>
reverse_wrapper<T> reverse(T& cont)
{
    return reverse_wrapper<T>(cont);
}
/////////////////////////////////////////////
template <class T>
class PoolAlloc
{

protected:

	class Link
	{
		size_t count;
		T *first, *last;

	public:

		Link()
			:count(0)
			, first(nullptr)
			, last(nullptr)
		{
		}
		T* append(T* np)
		{
			if (!first && !last)
			{
				first = np;
				last = np;
				np->prev = nullptr;
				np->next = nullptr;
			}
			else if (first && last)
			{
				np->prev = last;
				np->next = nullptr;

				last->next = np;
				last = np;
			}
			else 
			{ 
				DEBUG_ASSERT(0);
			}
			//count
			++count;
			//return 
			return np;
		}
		T* erase(T* np)
		{
			//if first
			if (first == np)
				first = np->next;
			//if last
			if (last == np)
				last = np->prev;
			//dt node
			if (np->next)
				np->next->prev = np->prev;
			if (np->prev)
				np->prev->next = np->next;
			//uncount
			--count;
			//unlink
			np->next = nullptr;
			np->prev = nullptr;

			return np;
		}
		size_t size()
		{
			return count;
		}
		T* getFirst()
		{
			return first;
		}
		T* getLast()
		{
			return last;
		}
		void reset()
		{
			count = 0;
			first = nullptr;
			last  = nullptr;
		}
		T* eraseTop()
		{
			return erase(first);
		}
	};
    
public:
    //node object
    class Node
    {
        friend class PoolAlloc<T>::Link;
        T* next{ nullptr };
        T* prev{ nullptr };
        
    public:
        
		virtual void init() {};
        virtual void release() {};
		T* getNext()
		{
			return next;
		}
		T* getPrev()
		{
			return prev;
		}
		const T* getNext() const
		{
			return next;
		}
		const T* getPrev() const
		{
			return prev;
		}
        
    };

	//const iterators
	class CIterator
	{

	protected:

		//friend class
		friend class PoolAlloc<T>;
		//raw ptr
		T* node;
		//init
		CIterator(T* ptr)
		{
			node = ptr;
		}

	public:
        
        CIterator operator++()
        {
            node = node->getNext();
            return *this;
        }
        
        CIterator operator--()
        {
            node = node->getPrev();
            return *this;
        }

		const T* operator*() const
		{
			return node;
		}

		bool operator==(const CIterator& rhs) const
		{
			return rhs.node == node;
		}

		bool operator!=(const CIterator& rhs) const
		{
			return rhs.node != node;
		}
	};
	//iterators
	class Iterator
	{

	protected:
		//friend class
		friend class PoolAlloc<T>;
		//raw ptr
		T* node;
		//init
		Iterator(T* ptr)
		{
			node = ptr;
		}

	public:
        
        Iterator operator++()
        {
            node = node->getNext();
            return *this;
        }
        
        Iterator operator--()
        {
            node = node->getPrev();
            return *this;
        }

		T* operator*() const
		{
			return node;
		}

		bool operator==(const Iterator& rhs) const
		{
			return rhs.node == node;
		}

		bool operator!=(const Iterator& rhs) const
		{
			return rhs.node != node;
		}
	};
	//init pool allocator
	void init(uint size, DFUNCTION<void(T& node)> allocInit = nullptr)
	{
		DEBUG_ASSERT(size);
		buffer.resize(0);
		buffer.resize(size);
		//linking
		reset(allocInit);
	}
	//reset
	void reset(DFUNCTION<void(T& node)> allocInit = nullptr)
	{
		//unlink
		allocNodes.reset();
		freeNodes.reset();
		//linking
		for (size_t i = 0; i != buffer.size(); ++i)
		{
			freeNodes.append(&buffer[i]);
			if (allocInit) allocInit(buffer[i]);
		}
	}
	//alloc node
	T* newNode()
	{
		if (!freeNodes.size()) return nullptr;
		//alloc
		T* newnode=freeNodes.eraseTop();
		//init
		newnode->init();
		//return
		return allocNodes.append(newnode);
	}
	void deleteNode(T* node)
	{
		node->release();
		//erese
		allocNodes.erase(node);
		//append
		freeNodes.append(node);
	}
	//count of allocated nodes
	size_t allocated()
	{
		return allocNodes.size();
	}
	//count of free nodes
	size_t remaining()
	{
		return freeNodes.size();
	}
	//for each
	void foreachAllocNoodes(DFUNCTION<void(T& node)> callback)
	{
		for (T* b = allocNodes.getFirst(); b != nullptr; b = b->getNext())
		{
			callback(*b);
		}
	}	
	//for each not standard
	void foreachFreeNodes(DFUNCTION<void(T& node)> callback)
	{
		for (T* b = freeNodes.getFirst(); b != nullptr; b = b->getNext())
		{
			callback(*b);
		}
	}
	//c++11 for each utils
	Iterator begin()
	{
		return Iterator(allocNodes.getFirst());
	}
	Iterator end()
	{
		return Iterator(nullptr);
    }
    Iterator last()
    {
        return Iterator(allocNodes.getLast());
    }
	CIterator begin() const
	{
		return CIterator(allocNodes.getFirst());
    }
    CIterator end() const
    {
        return CIterator(nullptr);
    }
    CIterator last() const
    {
        return CIterator(allocNodes.getLast());
    }
	//list allocated nodes
	Link& listAllocated()
	{
		return allocNodes;
	}	
	//list free nodes
	Link& listFree()
	{
		return freeNodes;
	}

protected:

	std::vector<T> buffer;
	Link allocNodes;
	Link freeNodes;

};

class Path
{

public:

    Path(const char* path);
    Path(const String& path);
    Path(const std::string& path);
    DFORCEINLINE const String& getPath() const
    {
        return path;
    }
    DFORCEINLINE const String& getDirectory() const
    {
        return directory;
    }
    DFORCEINLINE const String& getFilename() const
    {
        return file;
    }
    DFORCEINLINE const String& getFilebasename() const
    {
        return filebasename;
    }
    DFORCEINLINE const String& getExtension() const
    {
        return ext;
    }
    DFORCEINLINE const String& getLeftExtension() const
    {
        return leftext;
    }
    DFORCEINLINE const bool isAbsolute() const
    {
        return isabs;
    }
    //return relative path
    Path getRelativePathTo(const Path& path) const;
    Path getRelativePath() const;
    Path getAbsolute() const;
    //
    bool existsFile();
    bool existsDirectory();
    //
    std::vector<String> getFiles() const;
    std::vector<String> getSubDirs() const;
    //
    static String getCanonicalPath(const String& path);
    static bool convertToCanonicalPath(String& path);
    //
    //cast
    operator const String& () const
    {
        return this->path;
    }
    operator const char* () const
    {
        return this->path;
    }

private:

    DFORCEINLINE void costructor(const String& path);
    String path;
    String directory;
    String file;
    String filebasename;
    String ext;
    String leftext;
    bool   isabs;
};

};
};

#endif