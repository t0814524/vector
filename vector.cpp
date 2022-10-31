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

value_type *copy_array(value_type *arr, size_t old_size, size_t new_size)
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
Vector::Vector()
{
    sz = 0;
    max_size = 1;
    values = new value_type[max_size];
}

// init with size n
Vector::Vector(size_t n)
{
    sz = 0;       // number of elements in the Vector
    max_size = 1; // maximum number of elements that are possible (capacity of the Vector)

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
Vector::Vector(std::initializer_list<value_type> list)
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
Vector::Vector(const Vector &v)
{
    sz = v.sz;             // number of elements in the Vector
    max_size = v.max_size; // maximum number of elements that are possible (capacity of the Vector)
    values = copy_array(v.values, sz, max_size);
}

Vector::~Vector()
{
    delete[] values;
}

#pragma endregion CONSTRUCTORS

#pragma region METHODS

size_t Vector::size() const // Returns number of saved elements.
{
    return sz;
}

bool Vector::empty() const // Returns true if the Vector is empty, otherwise false.
{
    return (sz == 0);
}

void Vector::clear() // Deletes all elements from Vector.
{
    sz = 0;
    // info:
    //      figured if u clear, you want to free the memory but
    //      doctest basic tests doesn't want that.
    // max_size = 1;
    // delete[] values;
    // values = new value_type[max_size];
}

void Vector::reserve(size_t n) // Capacity of the Vector is increased to n if it is not already at least this large.
{
    if (max_size < n)
    {
        max_size = n;
        value_type *new_arr = copy_array(values, sz, max_size);
        delete[] values;
        values = new_arr;
    }
}

void Vector::shrink_to_fit() // Capacity is reduced to number of elements.
{
    if (sz < max_size)
    {
        max_size = sz;
        value_type *new_arr = copy_array(values, sz, max_size);
        delete[] values;
        values = new_arr;
    }
}

size_t Vector::capacity() const // Returns current capacity of the Vector.
{
    return max_size;
}

void Vector::pop_back() // Removes the last element in the Vector. Throws an std::runtime_error exception if the Vector was empty.
{
    if (sz <= 0)
        throw runtime_error("can't use pop_back on empty vector");
    sz = sz - 1;
}

void Vector::push_back(value_type x)
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

value_type &Vector::operator[](size_t index) // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
{
    if (index < 0 || index >= sz)
        throw runtime_error(string("index: ") + std::to_string(index) + string(" out of bounds, size is: ") + std::to_string(sz));
    return values[index];
}

const double &Vector::operator[](size_t index) const // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
{
    if (index < 0 || index >= sz)
        throw runtime_error(string("index: ") + std::to_string(index) + string(" out of bounds, size is: ") + std::to_string(sz));
    return values[index];
}

// copy assignment overload
const Vector &Vector::operator=(const Vector &other)
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

ostream &Vector::print(ostream &o) const
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
ostream &operator<<(ostream &o, const Vector &v)
{
    return v.print(o);
}

#pragma endregion METHODS
#pragma region ITERATOR

Vector::iterator Vector::begin()
{
    return iterator(values);
}

Vector::iterator Vector::end()
{
    return iterator(values + sz);
}

Vector::const_iterator Vector::begin() const
{
    // todo: y does that count as Iterator
    return iterator(values);
}

Vector::const_iterator Vector::end() const
{
    return Iterator(values + sz);
}

Vector::Iterator::Iterator() // Returns an iterator on nullptr.
{
    // todo: idk what return supposed to mean...
    ptr = nullptr;
}
Vector::Iterator::Iterator(pointer ptr) : ptr{ptr} {} // Returns an iterator which sets the instance variable to ptr.

// todo: check if const is necessary
Vector::Iterator::reference Vector::Iterator::operator*() const // Returns the value of the value referenced by ptr.
{
    return *ptr;
}
Vector::Iterator::pointer Vector::Iterator::operator->() const // Returns a pointer to the referenced value.
{
    // todo: should the ptr be a copy?
    return ptr;
}
bool Vector::Iterator::operator==(const const_iterator &it) const // Compares the pointers for equality. (A global function may be a better choice).
{
    // todo: can this be done nicer? would use -> if it would work without param. also if its supposed to be (*ptr).sth its wrong
    // return (this->ptr == &*it);
    // probably like this:
    return ptr == it.operator->();
}
bool Vector::Iterator::operator!=(const const_iterator &it) const // Compares the pointers for inequality. (A global function may be a better choice).
{
    // todo
    // return ptr == it.operator->();
    return !this->operator==(it);
}
Vector::iterator &Vector::Iterator::operator++() // (Prefix) Iterator points to next element and (a reference to it) is returned.
{
    ++ptr;
    return *this;
}
Vector::iterator Vector::Iterator::operator++(int) // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
{
    Vector::iterator pre = ptr++;
    return pre;
}
// todo: vecror::???
Vector::Iterator::operator Vector::const_iterator() const // (Type conversion) Allows to convert Iterator to ConstIterator
{
    return ConstIterator(ptr);
}

Vector::iterator Vector::insert(const_iterator pos, const_reference val)
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
    return iterator{values + current};
}
Vector::iterator Vector::erase(const_iterator pos)
{
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= sz)
        throw std::runtime_error("Iteratoroutofbounds");
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i < sz - 1; ++i)
        values[i] = values[i + 1];
    --sz;
    return iterator{values + current};
}

// todo: vector:: on operator??
Vector::difference_type operator-(const Vector::ConstIterator &lop,
                                  const Vector::ConstIterator &rop)
{
    return lop.ptr - rop.ptr;
}

#pragma endregion ITERATOR

#pragma region CONSTITERATOR
Vector::ConstIterator::ConstIterator() // Returns a ConstIterator on nullptr.
{
    ptr = nullptr;
}
Vector::ConstIterator::ConstIterator(pointer ptr) : ptr{ptr} {} // Returns a ConstIterator which sets the instance variable to ptr.

Vector::ConstIterator::reference Vector::ConstIterator::operator*() const // Returns the value of the value referenced by ptr.
{
    return *ptr;
}
Vector::ConstIterator::pointer Vector::ConstIterator::operator->() const // Returns a pointer to the referenced value.
{
    return ptr;
}
bool Vector::ConstIterator::operator==(const const_iterator &it) const // Compares the pointers for equality. (A global function may be a better choice).
{
    return ptr == it.operator->();
}
bool Vector::ConstIterator::operator!=(const const_iterator &it) const // Compares the pointers for inequality. (A global function may be a better choice).
{
    return !this->operator==(it);
}
Vector::const_iterator &Vector::ConstIterator::operator++() // (Prefix) Iterator points to next element and (a reference to it) is returned.
{
    ++ptr;
    return *this;
}
Vector::const_iterator Vector::ConstIterator::operator++(int) // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
{
    // todo: research:
    //  sufficient to return ptr or should it be a instance of the cls
    //  class init w constructor or set value. guess constructor
    Vector::const_iterator pre(ptr++);
    return pre;
}

#pragma endregion CONSTITERATOR