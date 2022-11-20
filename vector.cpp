// build (only compiler n options that worked):
// also start vsc from developer cmd to be able to use 'cl'
// cl /EHsc /std:c++17 tests/basic/utest.cpp vector.cpp

// stuff von pr1 folien:
//  fn:
//      for min, max, avg...
//      insert, erase
//      find(pattern)
// operator
//      >> (cin)
//      +
// template

// von pr2:
//  template to combine Iterator and ConstIterator

#include "vector.h"
#include <stdexcept>
#include <string>

template <class value_type>
value_type *copy_array(value_type *arr, typename Vector<value_type>::size_type old_size, typename Vector<value_type>::size_type new_size)
{
    value_type *new_arr = new value_type[new_size];

    for (int i = 0; i < old_size; ++i)
    {
        {
            new_arr[i] = arr[i];
        }
    }
    return new_arr;
}

#pragma region CONSTRUCTORS
// default
template <class value_type>
Vector<value_type>::Vector()
{
    sz = 0;
    max_size = 1;
    values = new value_type[max_size];
}

// init with size n
template <class value_type>
Vector<value_type>::Vector(Vector<value_type>::size_type n)
{
    sz = 0;       // number of elements in the Vector
    max_size = n; // maximum number of elements that are possible (capacity of the Vector)

    if (n == 0)
    {
        values = new value_type[1]; // pointer to array with the values
    }
    else
    {
        values = new value_type[max_size]; // pointer to array with the values
    }
}

// Returns a Vector with specified content.
template <class value_type>
Vector<value_type>::Vector(std::initializer_list<value_type> list)
{
    sz = max_size = list.size();
    values = new value_type[max_size]; // pointer to array with the values

    int i = 0;
    for (const auto elem : list)
    {
        values[i++] = elem;
    }

    // can i directly use the initializer list ptr? or do i need to re-allocate??
    // according to doc, initializer list is stored as an array
    // did some research:
    //          https://stackoverflow.com/questions/8193102/initializer-list-and-move-semantics
    //          https://tristanbrindle.com/posts/beware-copies-initializer-list
    //      apparently initializer list only provides const access and the elements cant be moved
    //      its all copied in the std vector.
    //      dont rlly understand y its designed that way...
    // might try constructor to pass an array and take ownership
    // conclusion:
    //      initializer lists r inefficient
}

// copy
template <class value_type>
Vector<value_type>::Vector(const Vector &v)
{
    sz = v.sz;             // number of elements in the Vector
    max_size = v.max_size; // maximum number of elements that are possible (capacity of the Vector)
    values = copy_array(v.values, sz, max_size);
}

template <class value_type>
Vector<value_type>::~Vector()
{
    delete[] values;
}

#pragma endregion CONSTRUCTORS

#pragma region METHODS

template <class value_type>
typename Vector<value_type>::size_type Vector<value_type>::size() const // Returns number of saved elements.
{
    return sz;
}

template <class value_type>
bool Vector<value_type>::empty() const // Returns true if the Vector is empty, otherwise false.
{
    return (sz == 0);
}

template <class value_type>
void Vector<value_type>::clear() // Deletes all elements from Vector.
{
    sz = 0;
    // info:
    //      figured if u clear, you want to free the memory but
    //      doctest basic tests doesn't want that.
    // max_size = 1;
    // delete[] values;
    // values = new value_type[max_size];
}

template <class value_type>
void Vector<value_type>::reserve(Vector<value_type>::size_type n) // Capacity of the Vector is increased to n if it is not already at least this large.
{
    if (max_size < n)
    {
        max_size = n;
        value_type *new_arr = copy_array(values, sz, max_size);
        delete[] values;
        values = new_arr;
    }
}

template <class value_type>
void Vector<value_type>::shrink_to_fit() // Capacity is reduced to number of elements.
{
    if (sz < max_size)
    {
        max_size = sz;
        value_type *new_arr = copy_array(values, sz, max_size);
        delete[] values;
        values = new_arr;
    }
}

template <class value_type>
typename Vector<value_type>::size_type Vector<value_type>::capacity() const // Returns current capacity of the Vector.
{
    return max_size;
}

template <class value_type>
void Vector<value_type>::pop_back() // Removes the last element in the Vector. Throws an std::runtime_error exception if the Vector was empty.
{
    if (sz <= 0)
        throw runtime_error("can't use pop_back on empty vector");
    sz = sz - 1;
}

template <class value_type>
void Vector<value_type>::push_back(value_type x)
{
    // expand arr
    if (max_size == sz)
    {
        max_size *= 2;
        value_type *new_arr = copy_array(values, sz, max_size);
        delete[] values;
        values = new_arr;
    }

    values[sz] = x;
    sz++;
}

template <class value_type>
value_type &Vector<value_type>::operator[](Vector<value_type>::size_type index) // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
{
    if (index < 0 || index >= sz)
        throw runtime_error(string("index: ") + std::to_string(index) + string(" out of bounds, size is: ") + std::to_string(sz));
    return values[index];
}

template <class value_type>
const value_type &Vector<value_type>::operator[](Vector<value_type>::size_type index) const // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
{
    if (index < 0 || index >= sz)
        throw runtime_error(string("index: ") + std::to_string(index) + string(" out of bounds, size is: ") + std::to_string(sz));
    return values[index];
}

// copy assignment overload
template <class value_type>
const Vector<value_type> &Vector<value_type>::operator=(const Vector &other)
{
    // no self assignment
    if (this != &other)
    {
        sz = other.sz;
        max_size = other.max_size;
        values = copy_array(other.values, sz, max_size);
    }
    return *this;
}

template <class value_type>
ostream &Vector<value_type>::print(ostream &o) const
{
    // ostream &temp = "";
    o << "[";
    for (int i = 0; i < sz; ++i)
    {
        {
            o << values[i];
            if (i != sz - 1)
            {
                o << ", ";
            }
        }
    }
    return o << ']';
}

// output

template <class value_type>
ostream &operator<<(ostream &o, const Vector<value_type> &v)
{
    return v.print(o);
}

#pragma endregion METHODS

template <class value_type>
typename Vector<value_type>::iterator Vector<value_type>::begin()
{
    bool inc = true;
    bool deref = true;
    // empty vector
    if (sz == 0)
    {
        deref = false;
        inc = false;
    }
    return iterator(values, this);
}

template <class value_type>
typename Vector<value_type>::iterator Vector<value_type>::end()
{
    return iterator(values + sz, this);
}

template <class value_type>
typename Vector<value_type>::const_iterator Vector<value_type>::begin() const
{
    // todo: y does that count as Iterator
    // return values;

    // info:
    // initialize w `=` instead of bracket syntax
    //  gets optimized, actually compared the assembly and its the same
    //  i just hate how `{}` looks and know how `=` behaves
    bool inc = true;
    bool deref = true;
    // empty vector
    if (sz == 0)
    {
        deref = false;
        inc = false;
    } // todotodo: remove that shit

    return const_iterator(values, this);

    // return const_iterator(values, (sz > 0), inc, this);
}

template <class value_type>
typename Vector<value_type>::const_iterator Vector<value_type>::end() const
{
    return const_iterator(values + sz, this);
}

#pragma region ITERATOR

// Returns an iterator on nullptr.
template <class value_type>
Vector<value_type>::Iterator::Iterator() : ptr{nullptr},
                                           vec{nullptr} {}

// Returns an iterator which sets the instance variable to ptr.
template <class value_type>
Vector<value_type>::Iterator::Iterator(pointer ptr) : ptr{ptr},
                                                      vec{nullptr}
{
    cout << "iter constr called";
}

// Returns a "safe" iterator with boundary and deref checks.
// todo: y const vector
template <class value_type>
Vector<value_type>::Iterator::Iterator(pointer ptr, const Vector *vec) : ptr{ptr},
                                                                         vec{vec}
{
    cout << "iter full constr called";
}

// to get the ptr for internal fns (==, boundary checks)
template <class value_type>
typename Vector<value_type>::Iterator::pointer Vector<value_type>::Iterator::get_ptr_unsafe() const
{
    return ptr;
}

template <class value_type>
bool Vector<value_type>::Iterator::checkIncrementable() const
{
    return (vec != nullptr) && (ptr != vec->end().get_ptr_unsafe());
}

template <class value_type>
bool Vector<value_type>::Iterator::checkDereferencable() const
{
    return vec != nullptr && ptr != vec->end().get_ptr_unsafe();
}

// todo: check if const is necessary
template <class value_type>
typename Vector<value_type>::Iterator::reference Vector<value_type>::Iterator::operator*() const // Returns the value of the value referenced by ptr.
{
    // if (!dereferencable)
    //     throw runtime_error("iterator is not dereferencable");
    if (!checkIncrementable())
        throw runtime_error("end is not dereferencable");
    return *ptr;
}
// should throw on end (utest_secure_iterators 63)
template <class value_type>
typename Vector<value_type>::Iterator::pointer Vector<value_type>::Iterator::operator->() const // Returns a pointer to the referenced value.
{
    if (!checkIncrementable())
        throw runtime_error("iterator is not dereferencable");
    return ptr;
}

template <class value_type>
bool Vector<value_type>::Iterator::operator==(const const_iterator &it) const // Compares the pointers for equality. (A global function may be a better choice).
{
    return ptr == it.get_ptr_unsafe();
}

template <class value_type>
bool Vector<value_type>::Iterator::operator!=(const const_iterator &it) const // Compares the pointers for inequality. (A global function may be a better choice).
{
    return !this->operator==(it);
}

template <class value_type>
typename Vector<value_type>::iterator &Vector<value_type>::Iterator::operator++() // (Prefix) Iterator points to next element and (a reference to it) is returned.
{
    if (checkIncrementable())
    {
        ++ptr;
    }
    // utest expects no increment and no throw
    // else
    //     throw runtime_error("ptr not incrementable");

    return *this;
}

template <class value_type>
typename Vector<value_type>::iterator Vector<value_type>::Iterator::operator++(int) // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
{
    Vector<value_type>::iterator pre = checkIncrementable() ? Vector<value_type>::iterator(ptr++, vec) : *this;
    return pre;
}

// todo: vecror::???
template <class value_type>
Vector<value_type>::Iterator::operator typename Vector<value_type>::const_iterator() const // (Type conversion) Allows to convert Iterator to ConstIterator
{
    return ConstIterator(ptr, vec);
}

#pragma endregion ITERATOR

template <class value_type>
typename Vector<value_type>::iterator typename Vector<value_type>::insert(const_iterator pos, const_reference val)
{
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) > sz)
        throw std::runtime_error("Iteratoroutofbounds");
    size_type current{static_cast<size_type>(diff)};
    if (sz >= max_size)
        reserve(max_size * 2); // Attentionspecialcase,ifnominimumsizeisdefined
    for (auto i{sz}; i-- > current;)
        values[i + 1] = values[i];
    values[current] = val;
    ++sz;
    return iterator{values + current, this};
}

template <class value_type>
typename Vector<value_type>::iterator Vector<value_type>::erase(const_iterator pos)
{
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= sz)
        throw std::runtime_error("Iteratoroutofbounds");
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i < sz - 1; ++i)
        values[i] = values[i + 1];
    --sz;
    return iterator{values + current, this};
}

// todo: vector:: on operator??
template <class value_type>
typename Vector<value_type>::difference_type operator-(const typename Vector<value_type>::ConstIterator &lop,
                                                       const typename Vector<value_type>::ConstIterator &rop)
{
    return lop.ptr - rop.ptr;
}

#pragma region CONSTITERATOR

// Returns a ConstIterator on nullptr.
template <class value_type>
Vector<value_type>::ConstIterator::ConstIterator() : ptr{nullptr},
                                                     vec{nullptr}
{
    cout << "basic constructor ";
}

// Returns a ConstIterator which sets the instance variable to ptr.
template <class value_type>
Vector<value_type>::ConstIterator::ConstIterator(pointer ptr) : ptr{ptr},
                                                                vec{nullptr}
{
    cout << "constiter single arg constructor called";
}

template <class value_type>
Vector<value_type>::ConstIterator::ConstIterator(pointer ptr, const Vector *vec) : ptr{ptr},
                                                                                   vec{vec}
{
    cout << "constiter full constructor";
}
// to get the ptr for internal fns (==, boundary checks)
template <class value_type>
typename Vector<value_type>::ConstIterator::pointer Vector<value_type>::ConstIterator::get_ptr_unsafe() const
{
    return ptr;
}

template <class value_type>
bool Vector<value_type>::ConstIterator::checkIncrementable() const
{
    return vec != nullptr && ptr != vec->end().get_ptr_unsafe();
}

template <class value_type>
typename Vector<value_type>::ConstIterator::reference Vector<value_type>::ConstIterator::operator*() const // Returns the value of the value referenced by ptr.
{
    if (!checkIncrementable())
        throw runtime_error("constiterator is not dereferencable**");
    return *ptr;
}

template <class value_type>
typename Vector<value_type>::ConstIterator::pointer Vector<value_type>::ConstIterator::operator->() const // Returns a pointer to the referenced value.
{
    if (!checkIncrementable())
        throw runtime_error("constiterator is not dereferencable--");
    return &*ptr;
}

template <class value_type>
bool Vector<value_type>::ConstIterator::operator==(const const_iterator &it) const // Compares the pointers for equality. (A global function may be a better choice).
{
    return ptr == it.get_ptr_unsafe();
}

template <class value_type>
bool Vector<value_type>::ConstIterator::operator!=(const const_iterator &it) const // Compares the pointers for inequality. (A global function may be a better choice).
{
    return !this->operator==(it);
}

template <class value_type>
typename Vector<value_type>::const_iterator &Vector<value_type>::ConstIterator::operator++() // (Prefix) Iterator points to next element and (a reference to it) is returned.
{
    if (checkIncrementable())
    {
        ++ptr;
    }
    return *this;
}

template <class value_type>
typename Vector<value_type>::const_iterator Vector<value_type>::ConstIterator::operator++(int) // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
{
    Vector<value_type>::const_iterator pre = checkIncrementable() ? Vector<value_type>::const_iterator(ptr++, vec) : *this;
    return pre;
}

#pragma endregion CONSTITERATOR