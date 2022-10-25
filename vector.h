// Checks if _VECTOR_ IF DECLARED
#ifndef _VECTOR_

// Defines _VECTOR_ if above
// conditions fails
#define _VECTOR_

#include <iostream>
using namespace std;

using value_type = double; // this will make it easier to transition to templates

class Vector
{
private:
    size_t sz;          // number of elements in the Vector
    size_t max_size;    // maximum number of elements that are possible (capacity of the Vector)
    value_type *values; // pointer to array containing the elements of the Vector

    // static constexpr size_t min_sz; // allowed but not required by this exercise to define min size. allowing empty vec has pros and cons and its up to me

public:
    Vector();                                  // default
    Vector(const Vector &v);                   // copy
    Vector(size_t n);                          // Returns a Vector with space for n elements.
    Vector(std::initializer_list<value_type>); // Returns a Vector with specified content.

    // todo: test valgrind and test special cases like Vector(0) and Vector({})
    ~Vector(); // destructor

    // todo: idk what that is
    // Copy assignment operator: The this object takes the values from the parameter. (Necessary
    // because of the use of dynamically allocated memory).
    size_t size() const;                          // Returns number of saved elements.
    bool empty() const;                           // Returns true if the Vector is empty, otherwise false.
    void clear();                                 // Deletes all elements from Vector.
    void reserve(size_t n);                       // Capacity of the Vector is increased to n if it is not already at least this large.
    void shrink_to_fit();                         // Capacity is reduced to number of elements.
    void push_back(value_type x);                 // Adds a copy of x to the end of the Vector.
    void pop_back();                              // Removes the last element in the Vector. Throws an std::runtime_error exception if the Vector was empty.
    double &operator[](size_t index);             // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
    const double &operator[](size_t index) const; // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
    size_t capacity() const;                      // Returns current capacity of the Vector.
    ostream &print(ostream &o) const;

    value_type *begin();
    value_type *end();
};

ostream &operator<<(ostream &o, const Vector &v);

#endif // _VECTOR_
