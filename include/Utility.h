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
			//else{ wrong }
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
			first = nullptr;
			last = nullptr;
		}
		T* eraseTop()
		{
			return erase(first);
		}
	};
    
public:
    
    class Node
    {
        friend class PoolAlloc<T>::Link;
        T* next{ nullptr };
        T* prev{ nullptr };
        
    public:
        
        virtual void init() = 0;
        virtual void release() = 0;
        T* getNext()
        {
            return next;
        }
        T* getPrev()
        {
            return prev;
        }
        
    };
    
protected:
    
	std::vector<T> buffer;
	Link allocNodes;
	Link freeNodes;

	void init(uint size, DFUNCTION<void(T& node)> allocInit = nullptr)
	{
		DEBUG_ASSERT(size);
		buffer.resize(0);
		buffer.resize(size);
		//unlink
		allocNodes.reset();
		freeNodes.reset();
		//linking
		for (size_t i = 0; i != size; ++i)
		{
			freeNodes.append(&buffer[i]);
			if (allocInit) allocInit(buffer[i]);
		}
	}
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
	void foreachAllocNoodes(DFUNCTION<void(T& node)> callback)
	{
		for (T* b = allocNodes.getFirst(); b != nullptr; b = b->getNext())
		{
			callback(*b);
		}
	}	
	void foreachFreeNodes(DFUNCTION<void(T& node)> callback)
	{
		for (T* b = freeNodes.getFirst(); b != nullptr; b = b->getNext())
		{
			callback(*b);
		}
	}
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