//==============================================================================
/*
  https://github.com/vinniefalco/LuaBridge
  
  Copyright 2012, Vinnie Falco <vinnie.falco@gmail.com>
  Copyright 2007, Nathan Reed

  License: The MIT License (http://www.opensource.org/licenses/mit-license.php)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
//==============================================================================

#ifndef LUABRIDGE_REFSTDCOUNTED_HEADER
#define LUABRIDGE_REFSTDCOUNTED_HEADER

#include <memory>
#include <unordered_map>
namespace Easy2D
	{
	//==============================================================================
	/**
	  Support for our RefCountedPtr.
	*/
	template<class T>
	struct LuaSharedBase
	{

	  struct ptr_hash
	  {
		  size_t operator () (const std::shared_ptr< T > ptr) const
		{
		  static std::hash< size_t > H;
		  return H((size_t)ptr.get());
		}
	  };
       
	  typedef std::unordered_map < const std::shared_ptr< T > , int, ptr_hash> RefCountsType;
  
	protected:

	  inline RefCountsType& getRefCounts ()
	  {
		static RefCountsType refcounts;
		return refcounts ;
	  }
	};

	//==============================================================================
	/**
	  A reference counted smart pointer.

	  The api is compatible with boost::RefCountedPtr and std::RefCountedPtr, in the
	  sense that it implements a strict subset of the functionality.

	  This implementation uses a hash table to look up the reference count
	  associated with a particular pointer.

	  @tparam T The class type.

	  @todo Decompose RefCountedPtr using a policy. At a minimum, the underlying
			reference count should be policy based (to support atomic operations)
			and the delete behavior should be policy based (to support custom
			disposal methods).

	  @todo Provide an intrusive version of RefCountedPtr.
	*/
	template <class T>
	class LuaShared : private LuaSharedBase<T>
	{

	public:

	  template <typename Other>
	  struct rebind
	  {
		typedef LuaShared <Other> other;
	  };

	  /** Construct as nullptr or from existing pointer to T.

		  @param p The optional, existing pointer to assign from.
	  */
	  LuaShared(T* const p = 0)
	  {
		  auto refs = getRefCounts();
		  m_p = std::shared_ptr< T >(p);
		  ++(refs[m_p]);
	  } 

	  LuaShared(const std::shared_ptr< T >& p = 0) : m_p(p)
	  {
		++(getRefCounts () [ m_p ]);
	  }

	  /** Construct from another LuaShared.

		  @param rhs The LuaShared to assign from.
	  */
	  LuaShared (LuaShared <T> const& rhs) : m_p (rhs.get())
	  {
		++getRefCounts () [m_p];
	  }

	  /** Construct from a LuaShared of a different type.

		  @invariant A pointer to U must be convertible to a pointer to T.

		  @param  rhs The LuaShared to assign from.
		  @tparam U   The other object type.
	  */
	  template <typename U>
	  LuaShared (LuaShared <U> const& rhs) : m_p (static_cast <T*> (rhs.get()))
	  {
		++getRefCounts () [m_p];
	  }

	  /** Release the object.

		  If there are no more references then the object is deleted.
	  */
	  ~LuaShared ()
	  {
		reset();
	  }

	  /** Assign from another LuaShared.

		  @param  rhs The LuaShared to assign from.
		  @return     A reference to the LuaShared.
	  */
	  LuaShared <T>& operator= (LuaShared <T> const& rhs)
	  {
		if (m_p != rhs.m_p)
		{
		  reset ();
		  m_p = rhs.m_p;
		  ++getRefCounts () [m_p];
		}
		return *this;
	  }

	  /** Assign from another LuaShared of a different type.

		  @note A pointer to U must be convertible to a pointer to T.

		  @tparam U   The other object type.
		  @param  rhs The other LuaShared to assign from.
		  @return     A reference to the LuaShared.
	  */
	  template <typename U>
	  LuaShared <T>& operator= (LuaShared <U> const& rhs)
	  {
		reset ();
		m_p = static_cast <T*> (rhs.get());
		++getRefCounts () [m_p];
		return *this;
	  }

	  /** Retrieve the raw pointer.

		  @return A pointer to the object.
	  */
	  T* get () const
	  {
		return m_p.get();
	  }
  
	  std::shared_ptr<T> getRef () const
	  {
		return m_p;
	  }


	  /** Retrieve the raw pointer.

		  @return A pointer to the object.
	  */
	  T* operator* () const
	  {
		return m_p.get();
	  }

	  /** Retrieve the raw pointer.

		  @return A pointer to the object.
	  */
	  T* operator-> () const
	  {
		return m_p.get();
	  }

	  /** Determine the number of references.

		  @note This is not thread-safe.

		  @return The number of active references.
	  */
	  long use_count () const
	  {
		return getRefCounts () [m_p];
	  }

	  /** Release the pointer.

		  The reference count is decremented. If the reference count reaches
		  zero, the object is deleted.
	  */
	  void reset ()
	  {
		if (m_p != nullptr)
		{
		  auto it= getRefCounts().find(m_p);
		  if (--(it->second) <= 0)
		  {
			getRefCounts().erase(it);
		  }
		  m_p = nullptr;
		}
	  }

	private:

	  std::shared_ptr<T> m_p;

	};

	//==============================================================================

	namespace luabridge
	{

	// forward declaration
	template <class T>
	struct ContainerTraits;

	template <class T>
	struct ContainerTraits < LuaShared <T> >
	{
	  typedef T Type;

	  static T* get(LuaShared <T> const& c)
	  {
		return c.get ();
	  }
	};

	}
}

#endif
