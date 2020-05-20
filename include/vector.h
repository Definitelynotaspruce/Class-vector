#include <memory>
#include <vector>
#include <iostream>

template <class T>
class Vector
{
public:
    typedef T *iterator;
    typedef const T *const_iterator;
    typedef std::size_t size_type;
    typedef T value_type;

    // MEMBER FUNCTIONS (constructor, destructor,copy constructor, =, asign, get_allocator)
    Vector() { create(); }
    explicit Vector(size_t n, const T &val = T{}) { create(n, val); }
    //Vector(typename vector<T>::iterator first, typename vector<T>::iterator last);
    Vector(std::initializer_list<T> lst) { create(lst); }
    Vector(const Vector &v) { create(v.begin(), v.end()); }
    Vector &operator=(const Vector &);
    ~Vector() { uncreate(); }
    //assign
    std::allocator<T> get_allocator() const { return alloc; }

    // ELEMENT ACCESS (at, [], front, back, first )
    T &at(size_t pos) { return first[pos]; }
    const T &at(size_t pos) const { return first[pos]; }
    T &operator[](size_t i) { return first[i]; }
    const T &operator[](size_t i) const { return first[i]; }
    T &front() { return *first; }
    const T &front() const { return *first; }
    T &back() { return *(avail - 1); }
    const T &back() const { return *(avail - 1); }
    iterator data() { return first; }             //Pointer to the underlying element storage.
    const iterator data() const { return first; } //Pointer to the underlying element storage.

    //ITERATORS (begin, end, rbegin, rend)
    iterator begin() { return first; }
    const_iterator cbegin() const { return first; }
    iterator end() { return avail; }
    const_iterator cend() const { return avail; }
    //biski su sitais nijasna
    iterator rbegin() { return avail - 1; }
    const_iterator crbegin() const { return avail - 1; }
    iterator rend() { return first; }
    const_iterator crend() const { return first; }

    //CAPACITY (empty, size, max_size, reserve, capacity, shrink_to_fit)
    bool empty() const { return (first == NULL); }
    size_t size() const { return avail - first; }
    size_t max_size() const;
    void reserve(size_t new_cap);
    size_t capacity() const { return limit - first; } //pas mane jis nedideja deja
    void shrink_to_fit();

    //MODIFIERS (clear,insert, emplace, erase, push_back, emplace_back, pop_back, resize)
    void clear();
    iterator insert(const_iterator pos, const T &value);
    //emplace
    void emplace_back(T args) { push_back(args); }
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T &t);
    void pop_back();
    void resize(size_t count, T value = T());
    void swap(Vector<T> &rhs);

    //NON-MEMBER FUNCTIONS (==, std::swap, erase, erase_if)
    bool operator==(const Vector &);
    //std::swap
    //erase
    //erase_if

private:
    iterator first; //pirmasis elementas
    iterator avail; // pirmasis po paskutinio elemento
    iterator limit; // capacity

    std::allocator<T> alloc; // objektas atminties valdymui

    void create();
    void create(size_t, const T &);
    void create(const_iterator, const_iterator);
    void create(std::initializer_list<T>);
    void uncreate();
    void grow();
    void unchecked_append(const T &);
};
//-----------------------------------------------------------------------------------------
//MEMBER FUNCTIONS

template <class T>
Vector<T> &Vector<T>::operator=(const Vector &rhs)
{
    if (&rhs != this)
    {
        uncreate();
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

//CAPACITY
template <class T>
size_t Vector<T>::max_size() const
{
    return alloc.max_size();
}

template <class T>
void Vector<T>::reserve(size_t new_cap)
{
    if (new_cap > capacity())
    {
        iterator new_data = alloc.allocate(new_cap);
        iterator new_avail = std::uninitialized_copy(first, avail, new_data);
        uncreate();
        first = new_data;
        avail = new_avail;
        limit = first + new_cap;
    }
}

template <class T>
void Vector<T>::shrink_to_fit()
{
    limit = avail;
    Vector<T>::size_type tempSize = size();

    iterator tempData = alloc.allocate(limit - first);
    std::uninitialized_copy(first, first + size(), tempData);
    alloc.deallocate(first, capacity());

    first = tempData;
    limit = avail = first + tempSize;
}

//MODIFIERS

template <class T>
void Vector<T>::clear()
{
    iterator new_first = alloc.allocate(capacity());
    size_t cap = capacity();
    uncreate();
    limit = new_data + cap;
    first = avail = new_data;
}
template <class T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator pos)
{
    iterator it = &first[pos - first];
    alloc.destroy(it);
    std::uninitialized_move(it + 1, avail--, it);    

    return it;
}

template <class T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator start, const_iterator last)
{
    iterator it_start = &first[start - first];
    iterator it_end = &first[last - first];

    for (auto it = it_start; it != it_end; ++it)
        alloc.destroy(it);

    std::uninitialized_move(it_end, avail, it_start);
    return it_end;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T &val)
{
    auto new_pos = pos - first;
    if (avail == limit)
        grow();

    iterator it = &first[new_pos];
    std::uninitialized_move(it, avail++, it + 1);
    *(it) = val;

    return it;
}

template <class T>
void Vector<T>::push_back(const T &t)
{
    if (avail == limit)
        grow();
    unchecked_append(t);
}

template <class T>
void Vector<T>::pop_back()
{
    alloc.destroy(avail--);
}

template <class T>
void Vector<T>::resize(size_t count, T value)
{
    if (count < size())
    {
        iterator new_data = alloc.allocate(count);
        iterator new_avail = std::uninitialized_copy(first, first + count, new_data);
        iterator new_limit = new_data + capacity();

        uncreate();

        first = new_data;
        avail = new_avail;
        limit = new_limit;
    }
    else if (count > size())
    {
        iterator new_data = alloc.allocate(count);
        iterator pos = std::uninitialized_copy(first, avail, new_data);
        std::uninitialized_fill(pos, new_data + count - 1, value);
        iterator new_avail = new_data + count;
        iterator new_limit;
        if (capacity() < new_avail - new_data)
        {
            new_limit = new_avail;
        }
        else
        {
            new_limit = new_data + capacity();
        }
        uncreate();

        first = new_data;
        avail = new_avail;
        limit = new_limit;
    }
}

template <typename T>
void Vector<T>::swap(Vector<T> &rhs)
{
    size_t temp_size = size(),
           temp_capacity = capacity();
    T *temp_first = first;

    first = rhs.first;
    limit = rhs.limit;
    avail = rhs.avail;

    rhs.first = temp_first;
    rhs.limit = temp_first + temp_capacity;
    rhs.avail = temp_first + temp_size;
}

//NON-MEMBER FUNCTIONS (==, std::swap, erase, erase_if)
template <class T>
bool Vector<T>::operator==(const Vector &rhs)
{
    size_t index = 0;
    auto pos = this->begin();
    if (rhs.size() == this->size())
    {
        for (auto element : rhs)
        {
            if (element != *pos++)
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------

template <class T>
void Vector<T>::create()
{
    first = avail = limit = nullptr;
}

template <class T>
void Vector<T>::create(size_t n, const T &val)
{
    first = alloc.allocate(n);                  // grąžina ptr į array pirmą elementą
    limit = avail = first + n;                  // sustato rodykles į vietas
    std::uninitialized_fill(first, limit, val); // inicializuoja elementus val reikšme
}

template <class T>
void Vector<T>::create(const_iterator i, const_iterator j)
{
    first = alloc.allocate(j - i);                        // išskirti vietos j-i elementams
    limit = avail = std::uninitialized_copy(i, j, first); // nukopijuoja elementus iš intervalo
}

template <typename T>
void Vector<T>::create(std::initializer_list<T> list)
{
    first = avail = alloc.allocate(list.size());
    limit = first + list.size();
    for (auto &val : list)
        unchecked_append(val);
}

template <class T>
void Vector<T>::uncreate()
{
    if (first)
    {
        // sunaikinti (atbuline tvarka) sukonstruotus elementus
        iterator pos = avail;
        while (pos != first)
            alloc.destroy(--pos);
        // atlaisvinti išskirtą atmintį. Turi būti first != nullptr
        alloc.deallocate(first, limit - first);
    }
    // reset'inam pointer'iuss - Vector'ius tuščias
    first = limit = avail = nullptr;
}

template <class T>
void Vector<T>::grow()
{
    size_t new_size = std::max(2 * (limit - first), ptrdiff_t(1));
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(first, avail, new_data);

    uncreate();

    first = new_data;
    avail = new_avail;
    limit = first + new_size;
}

template <class T>
void Vector<T>::unchecked_append(const T &val)
{
    alloc.construct(avail++, val);
}
