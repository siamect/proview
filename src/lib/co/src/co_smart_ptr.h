#ifndef CO_UNIQUE_PTR
#define CO_UNIQUE_PTR

#include "co_debug.h"

// std::remove_reference
template<typename T> struct remove_reference      { typedef T type; };
template<typename T> struct remove_reference<T&>  { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };

// std::forward
template<typename T>
T&& forward(typename remove_reference<T>::type& x) {
    return static_cast<T&&>(x);
}

template<typename T>
T&& forward(typename remove_reference<T>::type&& x) {
    return static_cast<T&&>(x);
}

// std::move
template<typename T>
typename remove_reference<T>::type&& move(T&& x) {
    return ((typename remove_reference<T>::type&&)x);
}

template<typename T>
class default_delete {
public:
    void operator()(T* ptr) const {
        delete ptr;
    }
};

template<typename T>
class default_delete<T[]> {
public:
    void operator()(T* ptr) const {
        delete[] ptr;
    }
};



// std::unique_ptr for single objects -> free memory with 'delete'
template<typename T, typename Deleter = default_delete<T>>
class unique_ptr {
public:
    typedef unique_ptr<T, Deleter> this_type;
    unique_ptr(T* p) : value(p) {
    }
    unique_ptr(T* p, Deleter d) : value(p), del(d) {
    }
    unique_ptr(this_type&& x) : value(x.release()), del(forward<Deleter>(x.get_deleter())) {
    }

    this_type& operator=(this_type&& x) {
        reset(x.release());
        del = move(forward<Deleter>(x.get_deleter()));
        return *this;
    }

    ~unique_ptr() {
        reset();
    }

    void reset(T* p = NULL) {
        if (p != value) {
            get_deleter()(value);
            value = p;
        }
    }

    T* release() {
        T* const tmp = value;
        value = NULL;
        return tmp;
    }

    T& operator*() const {
        return *value;
    }

    T* operator->() const {
        return value;
    }

    T* get() const {
        return value;
    }

    Deleter& get_deleter() {
        return del;
    }

    const Deleter& get_deleter() const {
        return del;
    }
protected:
    T* value;
    Deleter del;

    unique_ptr(const this_type&);
    unique_ptr& operator=(const this_type&);
    unique_ptr& operator=(T* p);
};

// std::unique_ptr for arrays -> free memory with 'delete[]'
template<typename T, typename Deleter>
class unique_ptr<T[], Deleter> {
public:
    typedef unique_ptr<T[], Deleter> this_type;

    unique_ptr(T* p, Deleter d = default_delete<T[]>()) : value(p), del(d) {
    }
    unique_ptr(this_type&& x) : value(x.release()), del(forward<Deleter>(x.get_deleter())) {
    }

    this_type& operator=(this_type&& x) {
        reset(x.release());
        del = move(forward<Deleter>(x.get_deleter()));
        return *this;
    }

    ~unique_ptr() {
        reset();
    }

    void reset(T* p = NULL) {
        if (p != value) {
            get_deleter()(value);
            value = p;
        }
    }

    T* release() {
        T* const tmp = value;
        value = NULL;
        return tmp;
    }

    T& operator[](int i) const {
        return value[i];
    }

    T* get() const {
        return value;
    }

    Deleter& get_deleter() {
        return del;
    }

    const Deleter& get_deleter() const {
        return del;
    }
protected:
    T* value;
    Deleter del;

    unique_ptr(const this_type&);
    unique_ptr& operator=(const this_type&);
    unique_ptr& operator=(T* p);
};

#endif