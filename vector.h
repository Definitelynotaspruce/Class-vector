#include <memory>
#include <vector>

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
    explicit Vector(size_t n, const T &t = T{}) { create(n, t); }
    Vector(const Vector &v) { create(v.begin(), v.end()); }
    Vector &operator=(const Vector &);
    ~Vector() { uncreate(); }
    //asign nera
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
    //first cia karoce kertasi su definitionu sadly
    T& data() { return this->first; }
	const T& data() const { return this->first; }

    //ITERATORS (begin, end, rbegin, rend)
    iterator begin() { return first; }
    const_iterator begin() const { return first; }
    iterator end() { return avail; }
    const_iterator end() const { return avail; }
    iterator rbegin() { return limit - 1; }
    const_iterator rbegin() const { return limit - 1; }
    iterator rend() { return first + 1; }
    const_iterator rend() const { return first + 1; }

    //CAPACITY (empty, size, max_size, reserve, capacity, shrink_to_fit)
    bool empty() const { return (first == NULL); }
    size_t size() const { return avail - first; }
    size_t max_size() const;
    void reserve(size_t new_cap);
    size_t capacity() const { return limit - first; }
    void shrink_to_fit(); 

    //MODIFIERS (clear,insert, emplace, erase, push_back, emplace_back, pop_back, resize)
    void clear();
    iterator insert( const_iterator pos, const T& value ); 
    //emplace
    void emplace_back(T args) { push_back(args); }
    iterator erase(const_iterator pos);                       
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
    iterator first;
    iterator avail;
    iterator limit;
    // atminties išskyrimo valdymui
    std::allocator<T> alloc; // objektas atminties valdymui

    void create();
    void create(size_t, const T &);
    void create(const_iterator, const_iterator);
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
//asign

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
    iterator new_data = alloc.allocate(capacity());
    size_t cap = capacity();
    uncreate();
    limit = new_data + cap;
    first = avail = new_data;
}
template <class T>
typename Vector<T>::iterator Vector<T>::erase(const T* pos) {
		T* it = &first[pos - first];
        alloc.destroy(it);
		//memmove(it, it + 1, (size() - (pos - elem) - 1) * sizeof(T));
        std::uninitialized_move_n(it + 1, (size() - (pos - first) - 1) * sizeof(T), it);
		resize((avail-first) - 1);
		return it;
	}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(Vector<T>::const_iterator pos, const T &val)
{
    auto it = &first[pos - first];
    size_type cap_size = limit - first;
    size_type vec_size = avail - first;
    auto temp_first = first, avail_size = avail - first, limit_size = limit - first;
    if(avail == limit)
    {
        auto temp_first = alloc.allocate(capacity() << 2);
        std::uninitialized_copy(first, avail, temp_first);
        alloc.deallocate(first, limit - first);
        first = temp_first;
        avail = first + avail_size;
        limit = first + limit_size;
    }
    std::uninitialized_move_n(it, ((vec_size) - (pos - first)) * sizeof(T), it);
    (*it) = val;
    resize(avail_size + 1);
    
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
    if ( rhs.size() == this->size() ) 
    {
        for(auto element : rhs)
        {
            if(element != *pos++)
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
    first = alloc.allocate(n);             // grąžina ptr į array pirmą elementą
    limit = avail = first + n;             // sustato rodykles į vietas
    std::uninitialized_fill(first, limit, val); // inicializuoja elementus val reikšme
}

template <class T>
void Vector<T>::create(const_iterator i, const_iterator j)
{
    first = alloc.allocate(j - i);                   // išskirti vietos j-i elementams
    limit = avail = std::uninitialized_copy(i, j, first); // nukopijuoja elementus iš intervalo
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
    // dvigubai daugiau vietos, nei buvo
    size_t new_size = std::max(2 * (limit - first), ptrdiff_t(1));
    // išskirti naują vietą ir perkopijuoti egzistuojančius elementus
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(first, avail, new_data);
    // atlaisvinti seną vietą
    uncreate();
    // reset'int rodykles į naujai išskirtą vietą
    first = new_data;
    avail = new_avail;
    limit = first + new_size;
}
// tariame, kad `avail` point'ina į išskirtą, bet neinicializuotą vietą
template <class T>
void Vector<T>::unchecked_append(const T &val)
{
    alloc.construct(avail++, val);
}
