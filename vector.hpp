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
#include <stdexcept> // for STL exceptions
#include <initializer_list>
#include <limits> // for std::numeric_limits<size_t>

#include <iostream>

namespace mqs
{

  template <typename T>
  class Vector
  {
  private:
    T* arr;
    size_t _size; 
    size_t _capacity;

    void range_check(size_t i) const {
      if(i >= _size) {
        std::string error = "mqs::Vector::range_check: The index " + std::to_string(i) + " is out of bounds.";
        throw std::out_of_range(error);
      }
    }

    void grow_vector() {
      if(_capacity != std::numeric_limits<size_t>::max() && _size == _capacity) {
        _capacity = 2*_capacity;
        _capacity = (_capacity < _size ? std::numeric_limits<size_t>::max() : _capacity);
        T* new_arr = new T[_capacity];
        std::copy(arr, arr+_size, new_arr);
        delete []arr;
        arr = new_arr;
      }
    }

    void shrink_vector()
    {
      if(size < _capacity/4) {
        _capacity /= 2;
        T* new_arr = new T[_capacity];
        for(size_t i = 0; i < _size; i++) {
          new_arr[i] = arr[i];
        }
        delete[] arr;
        arr = new_arr;
      }
    }

  public:
    /**
     * Default constructor. Creates an empty vector with max size of 1
     */
    explicit Vector() {arr = new T[16]; _size = 0; _capacity = 16;}

    /**
     * Creates a vector of size n and max size of 2n or maximum value for size_t if 2n overflows.
     */
    explicit Vector(const size_t n) 
    {
      _size = n;
      _capacity = 2*_size;
      arr = new T[_capacity];
      _capacity = (_capacity <= _size ? std::numeric_limits<size_t>::max() : _capacity);
    }

    /**
     *  Creates a vector of size n, each element is initialized to value t.
     */
    explicit Vector(const size_t n, const T& t)
    {
      _size = n;
      _capacity = 2*_size;
      arr = new T[_capacity];
      _capacity = (_capacity <= _size ? std::numeric_limits<size_t>::max() : _capacity);
      for(size_t i = 0; i < _size; i++) {
        arr[i] = t;
      }
    }

    /**
     * Copy constructor. Creates a duplicate of the input Vector v.
    */
    Vector(const Vector& v)
    {
      _size = v.size();
      _capacity = v.capacity();
      arr = new T[_capacity];
      for(size_t i = 0; i < _size; i++) {
        arr[i] = v.arr[i];
      }
    }

    /**
     *  Initializer list constructor. Creates a vector with the values of initializer list l. 
     */
    Vector(const std::initializer_list<T>& l)
    {
      _size = l.size();
      _capacity = 2*_size;
      arr = new T[_capacity];
      _capacity = (_capacity <= _size ? std::numeric_limits<size_t>::max() : _capacity);
      for(auto it = l.begin(); it < l.end(); it++) {
        arr[(it-l.begin())] = *it;
      }
    }

    /**
     *  Returns the number of elements in the vector.
     *  @return the number of elements in the Vector.
     */
    size_t size() const
    {
      return _size; 
    }

    /**
     *  Returns the current size of the array holding the elements.
     *  @return the current size of the array holding the elements.
     */
    size_t capacity() const
    {
      return _capacity;
    }

    /** 
     *  Returns true if the Vector is empty, false otherwise.
     *  @return true if the Vector is empty, false otherwise. 
     */
    bool empty() const
    {
      return _size == 0;
    }

    /**
     * Returns the element at index i, even if it is out of bounds of the Vector.
     * @param the index of the wanted element
     * @return the element at that index
     */
    T operator[](const size_t i) const
    {
      return arr[i];
    }
    

    /** 
     *  Attempts to return the element at the given index. Throws an out_of_range exception if the index is greater 
     *  than or equal to the number of elements in the array.
     *  @param the index of the wanted element
     *  @return the element held at index i in the Vector.
     */
    T at(const size_t i) const
    {
      range_check(i);
      return arr[i];
    }

    /**
     *  Adds an element on to the end of the array, increasing its size by 1.
     *  @param the element to be inserted
     */
    void push_back(const T& t)
    {
      if(_size == 0 && _capacity == std::numeric_limits<size_t>::max()) {
        throw std::length_error("mqs::Vector::push_back(): Vector exceeded max capacity.");
      }
      arr[_size++] = t;
      grow_vector();
    }

    void insert(const size_t i, const T& t)
    {
      if(i == _size) {
        push_back(t);
      } else if(_size == 0 && _capacity == std::numeric_limits<size_t>::max()) {
        throw std::length_error("mqs::Vector::insert(): Vector exceeded max capacity.");
      } else {
        for(size_t j = _size-1; j >= i; j--) {
          arr[j+1] = arr[j];
        }
        arr[i] = t;
        _size++;
        grow_vector();
      }
    }
    
    void prepend(const T& t)
    {
      insert(0, t);
    }
    
    /**
     * Removes the last element in the array and returns it.
     * @return the last element in the array
     */
    T pop()
    {
      if(_size == 0) {
        throw std::out_of_range("mqs::Vector::pop(): Can't pop() on an empty Vector.");
      }
      T top = arr[--_size];
      shrink_vector();
    }

    /**
     * Finds the index of the element t. If the element doesn't exist, the size of the array is returned.
     * If the Vector is ever at size std::numeric_limits<size_t>::max() and this method returns 0, a manual verification
     * should be performed.
     * @param the element to be found.
     * @return the index of the found element, or the size of the array if it wasn't found.
     */
    size_t find(const T& t)
    {
      for(size_t i = 0; i < _size; i++) {
        if(arr[i] == t) {
          return i;
        }
      }
      return _size;
    }
    
    /**
     * Removes the element at the given index in the array and shifts forward elements back one.
     * Throws an out_of_range expception if the index is greater than or equal to the size of the vector
     * @param the index of the element to be removed
     */
    void remove(const size_t i)
    {
      range_check(i);
      if(i == _size-1) {
        pop();
      } else {
        for(int j = i+1; j < _size-1; j++) {
          arr[j] = arr[j+1];
        }
      }
      _size--;
      shrink_vector();
    }

    /**
     * Removes any occurent of the given element in the Vector. 
     * @param the element to be removed.
     * @return the number of elements removed from the Vector.
     */
    size_t remove(const T& t)
    {
      size_t found = 0;
      for(size_t i = 0; i < size; i++) {
        if(arr[i] == t) {
          remove(i);
          found++;
        }
      }
      return found;
    }

  };

}

#endif
