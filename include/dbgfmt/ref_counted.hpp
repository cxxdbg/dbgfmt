
/// \file removable.hpp
/// Contains definition of the removable class.

#pragma once

#include <cassert>
#include <memory>
#include <boost/smart_ptr/intrusive_ptr.hpp>


namespace dbgfmt {


/// Represents interface to an object with reference counter
class ref_counted {
public:
    /// Adds reference to object
    virtual void add_ref() = 0;

    /// Removes reference to object and destroys it if there are
    /// no more references left
    virtual void release() = 0;
};


/// Simple implementation of the ref_counted interface. Can be used
/// as base class for objects that should implement the ref_counted
/// interface
class ref_counted_impl: virtual public ref_counted {
public:
    /// Virtual destructor. Checks that there are no references
    /// on object. Calls assert(false) if reference count is not zero
    virtual ~ref_counted_impl() {
        assert(ref_count_ == 0 && "Destroyed shared object with non zero ref count");
    }

    /// Adds reference to object
    void add_ref() override {
        ++ref_count_;
    }

    /// Removes reference to object
    void release() override {
        --ref_count_;
        if (ref_count_ == 0) {
            delete this;
        }
    }

private:
    size_t ref_count_ = 0;              ///< Reference counter
};


/// Implementation of functions for boost::intrusive_ptr
inline void intrusive_ptr_add_ref(ref_counted * obj) {
    obj->add_ref();
}

inline void intrusive_ptr_release(ref_counted * obj) {
    obj->release();
}


/// Base class of reference wrappers for ref_counted objects.
/// Contains instrusive_ptr to ref_counted.
template <typename T>
class ref_counted_ref_base {
    static_assert(std::is_base_of<ref_counted, T>::value,
                  "T template parameter must be derived from ref_counted"
                  "interface");

    template <typename T2>
    friend class ref_counted_ref_base;

public:
    /// Default constructor, makes invalid pointer
    ref_counted_ref_base() = default;

    /// Copy constructor
    ref_counted_ref_base(const ref_counted_ref_base & r) = default;

    /// Move constructor
    ref_counted_ref_base(ref_counted_ref_base && r) = default;

    /// Conversion constructor
    template <typename T2>
    ref_counted_ref_base(const ref_counted_ref_base<T2> & r):
        ptr_{r.ptr_} {}

    /// Destructor
    ~ref_counted_ref_base() = default;

    /// Copy assignment operator
    ref_counted_ref_base<T> & operator=(const ref_counted_ref_base<T> &) = default;

    /// Move assignment operator
    ref_counted_ref_base<T> & operator=(ref_counted_ref_base<T> &&) = default;

    /// Increases number of references and return pointer to object.
    /// Should be used when returning pointer to ref counted
    /// object from a function
    T * do_return() {
        if (ptr_) {
            ptr_->add_ref();
        }
        return ptr_.get();
    }

    /// Returns true if reference is valid (not empty)
    operator bool() const { return ptr_ ? true : false; }

    /// Returns true if reference is invalid (empty)
    bool operator!() const { return *this ? false : true; }

    /// Returns pointer to ref counted object
    T * get() const { return ptr_.get(); }

protected:
    /// Constructs referece with specified raw pointer to ref counted
    /// object. Does not change number of references.
    explicit ref_counted_ref_base(T * obj): ptr_{obj} {
        if (ptr_) {
            ptr_->release();
        }
    }

private:
    boost::intrusive_ptr<T> ptr_;
};


class empty_ref_mixin {};


/// Wrapper around intrusive_ptr that prevents incorrect use of
/// intrusive pointer to ref_counted when passing parameters and
/// return values
template <typename T, typename Mixin = empty_ref_mixin>
class ref_counted_ref:
        virtual public ref_counted_ref_base<T>,
        public Mixin {

    template <typename T2>
    friend ref_counted_ref<T2> acquire_ref_counted(T2 *);

public:
    /// Default constructor, creates empty pointer
    ref_counted_ref() = default;

    /// Copy constructor
    ref_counted_ref(const ref_counted_ref & r) = default;

    /// Move constructor
    ref_counted_ref(ref_counted_ref && r) = default;

    /// Conversion constructor
    template <typename T2>
    ref_counted_ref(const ref_counted_ref<T2> & p):
        ref_counted_ref_base<T>{p} {}

    /// Returns raw pointer to object
    T * operator->() const {
        return this->get();
    }

private:
    /// Constructs referece with specified raw pointer to ref counted
    /// object. Does not change number of references.
    ref_counted_ref(T * obj): ref_counted_ref_base<T>{obj} {}
};


/// Acquires ref counted ptr. Does not increases reference counter.
/// Reference counter will be decreased after returned object is destroyed.
/// Should be called for return values.
template <typename T>
ref_counted_ref<T> acquire_ref_counted(T * obj) {
    return ref_counted_ref<T>{obj};
}


/// Creates ref counted object
template <typename T, typename ... Args>
ref_counted_ref<T> make_ref_counted(Args && ... args) {
    auto obj = std::make_unique<T>(std::forward<Args>(args)...);
    obj->add_ref();
    return acquire_ref_counted(obj.release());
}


}


