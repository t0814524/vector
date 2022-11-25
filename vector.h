
// PROTOTYPES
// class Vector
// {
// public:
//     Vector();                                  // default
//     Vector(const Vector &v);                   // copy
//     Vector(size_t n);                          // Returns a Vector with space for n elements.
//     Vector(std::initializer_list<value_type>); // Returns a Vector with specified content.
//     ~Vector();                                 // destructor

//     // Copy assignment operator: The this object takes the values from the parameter. (Necessary
//     const Vector &operator=(const Vector &other);

//     size_t size() const;                          // Returns number of saved elements.
//     bool empty() const;                           // Returns true if the Vector is empty, otherwise false.
//     void clear();                                 // Deletes all elements from Vector.
//     void reserve(size_t n);                       // Capacity of the Vector is increased to n if it is not already at least this large.
//     void shrink_to_fit();                         // Capacity is reduced to number of elements.
//     void push_back(value_type x);                 // Adds a copy of x to the end of the Vector.
//     void pop_back();                              // Removes the last element in the Vector. Throws an std::runtime_error exception if the Vector was empty.
//     double &operator[](size_t index);             // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
//     const double &operator[](size_t index) const; // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
//     size_t capacity() const;                      // Returns current capacity of the Vector.
//     ostream &print(ostream &o) const;

//     // iterator
//     iterator begin();
//     iterator end();
//     const_iterator begin() const;
//     const_iterator end() const;

//     iterator erase(const_iterator pos);
//     iterator insert(const_iterator pos, const_reference val);

//     class Iterator
//     {
//     public:
//         Iterator();                               // Returns an iterator on nullptr.
//         Iterator(pointer ptr);                    // Returns an iterator which sets the instance variable to ptr.
//         Iterator(pointer ptr, const Vector *vec); // Returns a "safe" iterator with boundary and deref checks.

//         pointer get_ptr_unsafe() const;
//         reference operator*() const;                     // Returns the value of the value referenced by ptr.
//         pointer operator->() const;                      // Returns a pointer to the referenced value.
//         bool operator==(const const_iterator &it) const; // Compares the pointers for equality. (A global function may be a better choice).
//         bool operator!=(const const_iterator &it) const; // Compares the pointers for inequality. (A global function may be a better choice).
//         iterator &operator++();                          // (Prefix) Iterator points to next element and (a reference to it) is returned.
//         iterator operator++(int);                        // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
//         operator const_iterator() const;                 // (Type conversion) Allows to convert Iterator to ConstIterator
//     };

//     class ConstIterator
//     {
//     public:
//         ConstIterator();                               // Returns a ConstIterator on nullptr.
//         ConstIterator(pointer ptr);                    // Returns a ConstIterator which sets the instance variable to ptr.
//         ConstIterator(pointer ptr, const Vector *vec); // Returns a "safe" iterator with boundary and deref checks.

//         pointer get_ptr_unsafe() const;
//         reference operator*() const;                   // Returns the value of the value referenced by ptr.
//         pointer operator->() const;                    // Returns a pointer to the referenced value.
//         bool operator==(const const_iterator &) const; // Compares the pointers for equality. (A global function may be a better choice).
//         bool operator!=(const const_iterator &) const; // Compares the pointers for inequality. (A global function may be a better choice).
//         const_iterator &operator++();                  // (Prefix) Iterator points to next element and (a reference to it) is returned.
//         const_iterator operator++(int);                // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.

//         friend Vector::difference_type operator-(const Vector::ConstIterator &lop,
//                                                  const Vector::ConstIterator &rop);
//     };
// };

// ostream &operator<<(ostream &o, const Vector &v);

// Checks if _CUSTOM_VECTOR_ IF DECLARED
#ifndef _CUSTOM_VECTOR_

// Defines _CUSTOM_VECTOR_ if above
// conditions fails
#define _CUSTOM_VECTOR_

#include <iostream>
using namespace std;

template <typename T>
class Vector
{
    // iterator stuff
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:
    size_type sz;       // number of elements in the Vector
    size_type max_size; // maximum number of elements that are possible (capacity of the Vector)
    value_type *values; // pointer to array containing the elements of the Vector

    // static constexpr size_t min_sz; // allowed but not required by this exercise to define min size. allowing empty vec has pros and cons and its up to me

public:
    // default
    Vector()
    {
        sz = 0;
        max_size = 1;
        values = new value_type[max_size];
    }
    // copy
    Vector(const Vector &v)
    {
        sz = v.sz;             // number of elements in the Vector
        max_size = v.max_size; // maximum number of elements that are possible (capacity of the Vector)
        values = copy_array(v.values, sz, max_size);
    }
    // init with size n
    Vector(size_type n)
    {
        sz = 0; // number of elements in the Vector
        max_size = n == 0 ? 1 : n;
        values = new value_type[max_size]; // pointer to array with the values
    }
    // Returns a Vector with specified content.
    Vector(std::initializer_list<value_type> list)
    {
        sz = list.size();
        max_size = sz == 0 ? 1 : sz;
        values = new value_type[max_size]; // pointer to array with the values

        int i = 0;
        for (const auto elem : list)
        {
            values[i++] = elem;
        }
    }

    ~Vector()
    {
        delete[] values;
    }

    // Copy assignment operator: The this object takes the values from the parameter. (Necessary
    // because of the use of dynamically allocated memory).
    const Vector &operator=(const Vector &other)
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
    size_type size() const // Returns number of saved elements.
    {
        return sz;
    }

    bool empty() const // Returns true if the Vector is empty, otherwise false.
    {
        return (sz == 0);
    }

    void clear() // Deletes all elements from Vector.
    {
        sz = 0;
        // info:
        //      figured if u clear, you want to free the memory but
        //      doctest basic tests doesn't want that.
        // max_size = 1;
        // delete[] values;
        // values = new value_type[max_size];
    }

    void reserve(size_type n) // Capacity of the Vector is increased to n if it is not already at least this large.
    {
        if (max_size < n)
        {
            max_size = n;
            value_type *new_arr = copy_array(values, sz, max_size);
            delete[] values;
            values = new_arr;
        }
    }

    void shrink_to_fit() // Capacity is reduced to number of elements.
    {
        if (sz < max_size)
        {
            max_size = sz;
            value_type *new_arr = copy_array(values, sz, max_size);
            delete[] values;
            values = new_arr;
        }
    }

    size_type capacity() const // Returns current capacity of the Vector.
    {
        return max_size;
    }

    void pop_back() // Removes the last element in the Vector. Throws an std::runtime_error exception if the Vector was empty.
    {
        if (sz <= 0)
            throw runtime_error("can't use pop_back on empty vector");
        sz = sz - 1;
    }

    void push_back(value_type x)
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

    value_type &operator[](size_type index) // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
    {
        if (index < 0 || index >= sz)
            throw runtime_error(string("index: ") + std::to_string(index) + string(" out of bounds, size is: ") + std::to_string(sz));
        return values[index];
    }

    const value_type &operator[](size_type index) const // Returns the element at the given position (index). If index is out of bounds, throws an std::runtime_error exception
    {
        if (index < 0 || index >= sz)
            throw runtime_error(string("index: ") + std::to_string(index) + string(" out of bounds, size is: ") + std::to_string(sz));
        return values[index];
    }

    ostream &print(ostream &o) const
    {
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

    // iterators

    iterator begin()
    {
        return iterator(values, this);
    }

    iterator end()
    {
        return iterator(values + sz, this);
    }

    const_iterator begin() const
    {
        return const_iterator(values, this);
    }

    const_iterator end() const
    {
        return const_iterator(values + sz, this);
    }

    iterator insert(const_iterator pos, const_reference val)
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

    iterator erase(const_iterator pos)
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

        const Vector *vec;

        bool check_deref_inc() const
        {
            return (vec != nullptr) && (ptr != vec->end().get_ptr_unsafe());
        }

    public:
        // Returns an iterator on nullptr.
        Iterator() : ptr{nullptr},
                     vec{nullptr} {}

        // Returns an iterator which sets the instance variable to ptr.
        Iterator(pointer ptr) : ptr{ptr},
                                vec{nullptr} {}

        // Returns a "safe" iterator with boundary and deref checks.
        Iterator(pointer ptr, const Vector *vec) : ptr{ptr},
                                                   vec{vec} {}

        // to get the ptr for internal fns (==, boundary checks)
        pointer get_ptr_unsafe() const
        {
            return ptr;
        }

        reference operator*() const // Returns the value of the value referenced by ptr.
        {
            if (!check_deref_inc())
                throw runtime_error("end is not dereferencable");
            return *ptr;
        }

        pointer operator->() const // Returns a pointer to the referenced value.
        {
            if (!check_deref_inc())
                throw runtime_error("iterator is not dereferencable");
            return ptr;
        }

        bool operator==(const const_iterator &it) const // Compares the pointers for equality. (A global function may be a better choice).
        {
            return ptr == it.get_ptr_unsafe();
        }

        bool operator!=(const const_iterator &it) const // Compares the pointers for inequality. (A global function may be a better choice).
        {
            return !this->operator==(it);
        }

        iterator &operator++() // (Prefix) Iterator points to next element and (a reference to it) is returned.
        {
            if (check_deref_inc())
            {
                ++ptr;
            }
            // utest expects no increment and no throw
            // else
            //     throw runtime_error("ptr not incrementable");

            return *this;
        }

        iterator operator++(int) // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
        {
            iterator pre = check_deref_inc() ? iterator(ptr++, vec) : *this;
            return pre;
        }

        operator const_iterator() const // (Type conversion) Allows to convert Iterator to ConstIterator
        {
            return ConstIterator(ptr, vec);
        }
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
        bool check_deref_inc() const
        {
            return vec != nullptr && ptr != vec->end().get_ptr_unsafe();
        }

    public:
        // Returns a ConstIterator on nullptr.
        ConstIterator() : ptr{nullptr},
                          vec{nullptr} {}

        // Returns a ConstIterator which sets the instance variable to ptr.
        ConstIterator(pointer ptr) : ptr{ptr},
                                     vec{nullptr} {}

        ConstIterator(pointer ptr, const Vector *vec) : ptr{ptr},
                                                        vec{vec} {}
        // to get the ptr for internal fns (==, boundary checks)
        pointer get_ptr_unsafe() const
        {
            return ptr;
        }

        reference operator*() const // Returns the value of the value referenced by ptr.
        {
            if (!check_deref_inc())
                throw runtime_error("constiterator is not dereferencable**");
            return *ptr;
        }

        pointer operator->() const // Returns a pointer to the referenced value.
        {
            if (!check_deref_inc())
                throw runtime_error("constiterator is not dereferencable--");
            return &*ptr;
        }

        bool operator==(const const_iterator &it) const // Compares the pointers for equality. (A global function may be a better choice).
        {
            return ptr == it.get_ptr_unsafe();
        }

        bool operator!=(const const_iterator &it) const // Compares the pointers for inequality. (A global function may be a better choice).
        {
            return !this->operator==(it);
        }

        const_iterator &operator++() // (Prefix) Iterator points to next element and (a reference to it) is returned.
        {
            if (check_deref_inc())
            {
                ++ptr;
            }
            return *this;
        }

        const_iterator operator++(int) // (Postfix) Iterator points to next element. Copy of iterator before increment is returned.
        {
            const_iterator pre = check_deref_inc() ? const_iterator(ptr++, vec) : *this;
            return pre;
        }

        friend difference_type operator-(const const_iterator &lop,
                                         const const_iterator &rop)
        {
            return lop.ptr - rop.ptr; // this is important, doesnt work the way i wrote the implementation below with friend
        }
    };
};

template <typename value_type>
ostream &operator<<(ostream &o, const Vector<value_type> &v)
{
    return v.print(o);
}

template <typename value_type>
value_type *copy_array(value_type *arr, typename Vector<value_type>::size_type old_size, typename Vector<value_type>::size_type new_size)
{
    value_type *new_arr = new value_type[new_size];

    for (typename Vector<value_type>::size_type i = 0; i < old_size; ++i)
    {
        {
            new_arr[i] = arr[i];
        }
    }
    return new_arr;
}

#endif // _CUSTOM_VECTOR_
