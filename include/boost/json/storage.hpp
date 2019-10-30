//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/json
//

#ifndef BOOST_JSON_STORAGE_HPP
#define BOOST_JSON_STORAGE_HPP

#include <boost/json/detail/config.hpp>
#include <atomic>
#include <cstddef>

namespace boost {
namespace json {

/** Abstract interface to a memory resource used with JSON.
*/
class storage
{
    std::atomic<
        unsigned long long> refs_{ 1 };
    unsigned long long const id_ = 0;
    bool const need_free_ = true;
    bool counted_ = true;

    friend class storage_ptr;

    template<class T>
    friend class scoped_storage;

public:
    virtual
    ~storage() = default;

    inline
    bool
    need_free() const noexcept
    {
        return need_free_;
    }

    bool
    is_equal(
        storage const& other) const noexcept
    {
        return (this == &other) || (
            this->id_ != 0 &&
            this->id_ == other.id_);
    }
    void*
    allocate(
        std::size_t n,
        std::size_t align)
    {
        return do_allocate(n, align);
    }

    void
    deallocate(
        void* p,
        std::size_t n,
        std::size_t align) noexcept
    {
        return do_deallocate(p, n, align);
    }

    friend
    bool
    operator==(
        storage const& lhs,
        storage const& rhs) noexcept
    {
        return lhs.is_equal(rhs);
    }

    friend
    bool
    operator!=(
        storage const& lhs,
        storage const& rhs) noexcept
    {
        return ! lhs.is_equal(rhs);
    }

protected:
    // Choose a unique 64-bit random number from here:
    // https://www.random.org/cgi-bin/randbyte?nbytes=8&format=h
    storage(
        bool need_free,
        unsigned long long id = 0) noexcept
        : id_(id)
        , need_free_(need_free)
    {
    }

    virtual
    void*
    do_allocate(
        std::size_t n,
        std::size_t align) = 0;

    virtual
    void
    do_deallocate(
        void* p,
        std::size_t n,
        std::size_t align) noexcept = 0;
};

} // json
} // boost

#endif
