// Checks if _CUSTOM_VECTOR_ IF DECLARED
#ifndef _CUSTOM_VECTOR_

// info for this bs commit:
//  what this exercise wants is probably way simpler than im thinking, doesnt rlly make sense as it is no real improvement and only covers some cases
//  otherwise the predefined constructor prototypes would need to be changed

// Defines _CUSTOM_VECTOR_ if above
// conditions fails
#define _CUSTOM_VECTOR_

#include <iostream>
using namespace std;

using value_type = double; // this will make it easier to transition to templates

class Vector
{
    // iterator stuff
public:
    class ConstIterator;
    class Iterator;
    using value_type = double;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

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

    // Copy assignment operator: The this object takes the values from the parameter. (Necessary
    // because of the use of dynamically allocated memory).
    const Vector &operator=(const Vector &other);

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

    // iterator
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    iterator erase(const_iterator pos);
    iterator insert(const_iterator pos, const_reference val);

    class Iterator
    {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr; // Points to an element in Vector.
        bool incrementable;
        bool dereferencable;
        Vector &vec;

    public:
        Iterator();                       // Returns an iterator on nullptr.
        Iterator(pointer ptr, Vector &v); // Returns an iterator which sets the instance variable to ptr.

        // todo: check if const is necessary
        reference operator*() const;                     // Returns the value of the value referenced by ptr.
        pointer operator->() const;                      // Returns a pointer to the referenced value.
        bool operator==(const const_iterator &it) const; // Compares the pointers for equality. (A global function may be a better choice).
        bool operator!=(const const_iterator &it) const; // Compares the pointers for inequality. (A global function may be a better choice).
        iterator &operator++();                          // (Prefix) Iterator points to next element and (a reference to it) is returned.
        iterator operator++(int);                        // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
        operator const_iterator() const;                 // (Type conversion) Allows to convert Iterator to ConstIterator
    };

    class ConstIterator
    {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr; // Points to an element in Vector.
        const Vector &vec;

    public:
        ConstIterator();                             // Returns a ConstIterator on nullptr.
        ConstIterator(pointer ptr, const Vector &v); // Returns a ConstIterator which sets the instance variable to ptr.

        reference operator*() const;                   // Returns the value of the value referenced by ptr.
        pointer operator->() const;                    // Returns a pointer to the referenced value.
        bool operator==(const const_iterator &) const; // Compares the pointers for equality. (A global function may be a better choice).
        bool operator!=(const const_iterator &) const; // Compares the pointers for inequality. (A global function may be a better choice).
        const_iterator &operator++();                  // (Prefix) Iterator points to next element and (a reference to it) is returned.
        const_iterator operator++(int);                // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.

        friend Vector::difference_type operator-(const Vector::ConstIterator &lop,
                                                 const Vector::ConstIterator &rop);
    };
};

ostream &operator<<(ostream &o, const Vector &v);

#endif // _CUSTOM_VECTOR_
