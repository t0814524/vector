// Checks if _CUSTOM_VECTOR_ IF DECLARED
#ifndef _CUSTOM_VECTOR_

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

        // boundary and * checks
        //  as far as my understanding goes it makes sense for iterators to NOT implement these checks but move them outside of the cls n handle in the context of the container
        //  also it doesnt rlly add any guarantees as there is a constructor with a pointer which has no way of checking anything.

        // thought:
        //  keep track of end, only use case would be a begin ptr that is incremented
        //  would be more easy to store a ptr to the vec and on increment, deref get the end ptr

        // cases:
        // default constructor
        //      no deref or inc
        //      only valid if assignment happens. assign:
        //          iterator w ref
        //              deref and inc: safety
        //          iterator with ptr constructor
        //              deref and inc: no safety (have to allow for it to work and have faith)
        // ptr constructor
        //      always deref and inc (on faith) - this is what makes no sense but its in the requirement
        // ...

        // whats the usefullness of these checks?
        // are there actually any real world cases where this would provide incresed safety?
        // was thinking in a wrong directon.. figuring out how to make the iterator never get invalidated.
        // but thats not what they for or how they used..
        // if u have a for loop rlly all u can do is loop from begin() to end()
        // only thing it could prevent is
        // <= instead of <
        // if u then push and loop again u need new iterators and the old ones r invalidated and thats how its supposed to be.
        //  thought im thinking to much and got hung up on shit they didnt even intend but they want this!!!
        //  > Für die grundsätzliche Absicherung reicht das Bestehen der Testcases ohne "Strongly" im Namen.
        //  > Für die Testcases mit "Strongly" im Namen muss man eine Verbindung zwischen dem Iterator und dem zugehörigen Vector-Objekt einrichten,
        //  > so dass Änderungen im Vektor auch im Iterator reflektiert werden.

        // todo: research nested cls in cpp, is there any point in putting iter cls inside? other thatn hinting at its relation (with which its intended to be used with)

        const Vector *vec;

        bool checkIncrementable() const;
        bool checkDereferencable() const;

    public:
        Iterator();                               // Returns an iterator on nullptr.
        Iterator(pointer ptr);                    // Returns an iterator which sets the instance variable to ptr.
        Iterator(pointer ptr, const Vector *vec); // Returns a "safe" iterator with boundary and deref checks.

        pointer get_ptr_unsafe() const;
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
        const Vector *vec;
        bool checkIncrementable() const;
        bool checkDereferencable() const;

    public:
        ConstIterator();                               // Returns a ConstIterator on nullptr.
        ConstIterator(pointer ptr);                    // Returns a ConstIterator which sets the instance variable to ptr.
        ConstIterator(pointer ptr, const Vector *vec); // Returns a "safe" iterator with boundary and deref checks.

        pointer get_ptr_unsafe() const;
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
