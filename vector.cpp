// build:
// cl /EHsc /std:c++17 tests/basic/utest.cpp vector.cpp

#include "vector.h"
#include <stdexcept>
#include <string>

value_type *copy_array(value_type *arr, size_t old_size, size_t new_size)
{
    value_type *new_arr = new value_type[new_size];

    for (int i = 0; i < old_size; i++)
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
    for (int i = 0; i < sz; i++)
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

value_type *Vector::begin()
{
    return values;
}

value_type *Vector::end()
{
    return values + sz;
}

#pragma endregion ITERATOR