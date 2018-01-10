/**
 *  vector.hpp
 *  An implementation of a resizable array
 *
 *  @author Marquess Valdez
 *  @version 1.0
 */
#ifndef MQS_VECTOR_HPP
#define MQS_VECTOR_HPP

#include <cstddef> //for std::size_t
#include <initializer_list>
#include <limits> // for std::numeric_limits<size_t>

namespace mqs
{

  template <typename T>
  class Vector
  {
  private:
    T* arr;
    size_t _size; 
    size_t maxSize;
  public:
    /**
     * Default constructor. Creates an empty vector with max size of 1
     */
    explicit Vector() {arr = new T[1]; _size = 0; maxSize = 1;}

    /**
     * Creates a vector of size n and max size of 2n or maximum value for size_t if 2n overflows.
     */
    explicit Vector(const size_t n) 
    {
      arr = new T[2*n];
      _size = n;
      maxSize = 2*_size;
      maxSize = (max_size <= _size ? std::numeric_limits<size_t>::max() : max_size);
    }

    /**
     *  Creates a vector of size n, each element is initialized to value t.
     */
    explicit Vector(const size_t n, const T& t)
    {
      arr = new T[n];
      _size = n;
      maxSize = 2*_size;
      maxSize = (max_size <= _size ? std::numeric_limits<size_t>::max() : max_size);
      for(size_t i = 0; i < _size; i++) {
        arr[i] = t;
      }
    }

    /**
     * Copy constructor. Creates a duplicate of the input Vector v.
    */
    Vector(const Vector& v)
    {
      arr = new T[v.size()];
      _size = v.size();
      maxSize = v.max_size;
      for(size_t i = 0; i < _size; i++) {
        arr[i] = v[i];
      }
    }

    /**
     *  Initializer list. Creates a vector with the values of initializer list l. 
     */
    explicit Vector(const std::initializer_list<T>& l)
    {
      arr = new T[l.size()];
      _size = l.size();
      maxSize = 2*_size;
      maxSize = (max_size <= _size ? std::numeric_limits<size_t>::max() : max_size);
      for(size_t i = 0; i < _size; i++) {
        arr[i] = l[i];
      }
    }

    size_t size() const
    {
      return _size; 
    }
  };

}

#endif
