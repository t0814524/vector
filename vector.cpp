#include "vector.h"
#include <stdexcept>
// #include <iostream>
#include <string>

value_type *copy_array(value_type *arr, size_t old_size, size_t new_size)
{
    value_type *new_arr = new value_type[new_size];

    // todo: check if that works, does potentially copy some random elements but only len of old arr. or just dont work bc sizeof ptr is only mem for the ptr
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

// copy
Vector::Vector(const Vector &v)
{
    sz = v.sz;             // number of elements in the Vector
    max_size = v.max_size; // maximum number of elements that are possible (capacity of the Vector)
    // values = v.values;     // pointer, deep copy i guess

    // for (int i = 0; i < sz; i++)
    // {
    //     values[i] = v.values[i];
    // }
    values = copy_array(v.values, sz, max_size);

    // todo: try range based loop with iterators
    // https://www.reddit.com/r/learnprogramming/comments/pdyukg/range_loop_on_array_not_working_within_a_function/

    // for (value_type elem : v.values)
    // {
    //     values =
    // }
}

// init with size n
Vector::Vector(size_t n)
{
    sz = 0;                            // number of elements in the Vector
    max_size = n;                      // maximum number of elements that are possible (capacity of the Vector)
    values = new value_type[max_size]; // pointer
}

// Returns a Vector with specified content.
// todo
Vector::Vector(std::initializer_list<value_type> list)
{
    // sz = list.size();                               // number of elements in the Vector
    // max_size = list.size();                         // maximum number of elements that are possible (capacity of the Vector)
    // values = new value_type[max_size];              // pointer
    // auto *array{new value_type[list.size()]{list}}; // initializer list

    // can i directly use the initializer list ptr? or do i need to re-allocate??
    // did some research:
    //          https://stackoverflow.com/questions/8193102/initializer-list-and-move-semantics
    //          https://tristanbrindle.com/posts/beware-copies-initializer-list
    //      apparently initializer list only provides const access and the elements cant be moved
    //      its all copied in the std vector. y??
    //      dont understand y they designed that way...
    // might try constructor to pass an array and take ownership
    // conclusion:
    //      initializer lists r inefficient
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
    max_size = 1;
    delete[] values;
    values = new value_type[max_size];
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

void Vector::pop_back() // Removes the last element in the Vector. Throws an std::runtime_error exception if the Vector was empty.
{
    if (sz <= 0)
        throw runtime_error("can't use pop_back on empty vector"); // check error msg and also if thats how u throw in cpp todo
    sz = sz - 1;
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

size_t Vector::capacity() const // Returns current capacity of the Vector.
{
    return 999;
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

// OUTPUT
ostream &operator<<(ostream &o, const Vector &v)
{
    return v.print(o);
}

value_type *Vector::begin()
{
    return values;
}

value_type *Vector::end()
{
    return values + sz;
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

#pragma endregion METHODS