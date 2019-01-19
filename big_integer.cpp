#include "big_integer.h"

#include <iostream>

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
struct BigInteger<Transformer, radix, Container, BaseType>::Impl {
    bool sign{true};
    Container<BaseType> digits{};
    static Transformer transformer{};
};

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger() : pImpl(std::make_unique<Impl>()) {}

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::~BigInteger() = default;

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger(BigInteger &&rhs) noexcept = default;

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType> &
BigInteger<Transformer, radix, Container, BaseType>::operator=(BigInteger &&rhs) noexcept = default;

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger(const BigInteger &rhs) : pImpl(
        std::make_unique<Impl>(*rhs.pImpl)) {}

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType> &
BigInteger<Transformer, radix, Container, BaseType>::operator=(const BigInteger &rhs) {
    *pImpl = *rhs.pImpl;
    return *this;
}