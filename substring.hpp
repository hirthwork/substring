/*
 * substring.hpp            -- char* + length wrapper
 *
 * Copyright (C) 2012 Dmitry Potapov <potapov.d@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SUBSTRING_HPP_2012_04_23__
#define __SUBSTRING_HPP_2012_04_23__

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>

#include <assert/emptyassert.hpp>
#include <assert/noexcept.hpp>
#include <assert/nullarypredicates.hpp>

namespace substring {
    template <class charT, class traits = std::char_traits<charT>,
        class Assert = assert::empty_assert>
    class basic_substring {
    public:
        typedef traits traits_type;
        typedef typename traits::char_type value_type;
        typedef std::size_t size_type;
        typedef Assert assert_type;
        typedef const value_type& const_reference;
        typedef const value_type* const_pointer;
        typedef const value_type* const_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        static const size_type npos = -1;

        basic_substring() throw()
            : start()
            , length()
        {
        }

        basic_substring(const basic_substring& substring) throw()
            : start(substring.start)
            , length(substring.length)
        {
        }

        basic_substring(const basic_substring& substring, size_type pos,
            size_type n = npos)
        {
            const size_type size = substring.size();
            assert_type::out_of_range_assert(assert::bind(std::bind1st(
                std::less_equal<size_type>(), pos), assert::bind(size)),
                "pos > size");
            start = substring.begin() + pos;
            length = std::min(n, size - pos);
        }

        basic_substring(const charT* s, size_type n) throw()
            : start(s)
            , length(n)
        {
        }

        explicit basic_substring(const charT* s) throw()
            : start(s)
            , length(traits_type::length(s))
        {
        }

        template <class Allocator>
        explicit basic_substring(
            const std::basic_string<charT, traits, Allocator>& s) throw()
            : start(s.data())
            , length(s.size())
        {
        }

        const_iterator begin() const throw() {
            return start;
        }

        const_iterator end() const throw() {
            return start + length;
        }

        const_reverse_iterator rbegin() const throw() {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator rend() const throw() {
            return const_reverse_iterator(begin());
        }

        const_iterator cbegin() const throw() {
            return begin();
        }

        const_iterator cend() const throw() {
            return end();
        }

        const_reverse_iterator crbegin() const throw() {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crend() const throw() {
            return const_reverse_iterator(begin());
        }

        size_type size() const throw() {
            return length;
        }

        void clear() throw() {
            length = 0;
        }

        bool empty() const throw() {
            return length;
        }

        const_reference operator [](size_type pos) const throw() {
            return start[pos];
        }

        const_reference at(size_type pos) const {
            assert_type::out_of_range_assert(assert::bind(
                std::bind1st(std::less<size_type>(), pos),
                assert::bind(this, &basic_substring::size)),
                "pos >= size");
            return start[pos];
        }

        const_reference front() const throw() {
            return *start;
        }

        const_reference back() const throw() {
            return *(start + length - 1);
        }

        void pop_front() throw() {
            ++start;
            --length;
        }

        void pop_back() throw() {
            --length;
        }

        void swap(basic_substring& rhs) throw() {
            std::swap(start, rhs.start);
            std::swap(length, rhs.length);
        }

        const charT* data() throw() {
            return start;
        }

        basic_substring substr(size_type pos, size_type n = npos) const
        {
            return basic_substring(*this, pos, n);
        }

        template <class Allocator>
        std::basic_string<charT, traits, Allocator> to_string(
            const Allocator& allocator = Allocator()) const {
            return std::basic_string<charT, traits, Allocator>(start, length,
                allocator);
        }

        std::basic_string<charT, traits> to_string() const {
            return std::basic_string<charT, traits>(start, length);
        }

        bool operator ==(const basic_substring& rhs) throw () {
            return length == rhs.length()
                && traits_type::compare(start, rhs.start, length) == 0;
        }

        bool operator !=(const basic_substring& rhs) throw () {
            return !(*this == rhs);
        }

    private:
        const_pointer start;
        size_type length;
    };

    template <class charT, class traits>
    static void swap(basic_substring<charT, traits>& lhs,
        basic_substring<charT, traits>& rhs)
    {
        lhs.swap(rhs);
    }

    typedef basic_substring<char> substring;
    typedef basic_substring<wchar_t> wsubstring;

    template <class charT, class traits, class Assert, std::size_t N>
    basic_substring<charT, traits, Assert> make_substring(
        const charT (&str)[N])
    {
        return basic_substring<charT, traits, Assert>(str, N - 1);
    }

    template <class charT, class traits, std::size_t N>
    basic_substring<charT, traits> make_substring(const charT (&str)[N])
    {
        return basic_substring<charT, traits>(str, N - 1);
    }

    template <class charT, std::size_t N>
    basic_substring<charT> make_substring(const charT (&str)[N])
    {
        return basic_substring<charT>(str, N - 1);
    }

    template <class charT, class traits, class assert>
    std::basic_ostream<charT, traits>& operator <<(
        std::basic_ostream<charT, traits>& stream,
        basic_substring<charT, traits, assert> substring) {
        return stream.write(substring.data(), substring.size());
    }
}

#endif

