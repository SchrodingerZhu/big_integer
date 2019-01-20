#include "big_integer.h"
#include <iostream>
#include <stack>

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
const std::unique_ptr<Transformer> BigInteger<Transformer, radix, Container, BaseType>::pTransformer = std::make_unique<Transformer>();

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
struct BigInteger<Transformer, radix, Container, BaseType>::Impl {
    bool sign{true};
    bool is_zero{false};
    Container<BaseType> digits{};
};

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger(int number) : pImpl(std::make_unique<Impl>()) {
    if (number == 0) {
        pImpl->is_zero = true;
        return;
    }
    if (number < 0) {
        number = -number;
        pImpl->sign = false;
    }
    while (number) {
        pImpl->digits.push_back(number % radix);
        number /= radix;
    }
}

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger(long long number) : pImpl(std::make_unique<Impl>()) {
    if (number == 0) {
        pImpl->is_zero = true;
        return;
    }
    if (number < 0) {
        number = -number;
        pImpl->sign = false;
    }
    while (number) {
        pImpl->digits.push_back(number % radix);
        number /= radix;
    }
}

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger(long number) : pImpl(std::make_unique<Impl>()) {
    if (number == 0) {
        pImpl->is_zero = true;
        return;
    }
    if (number < 0) {
        number = -number;
        pImpl->sign = false;
    }
    while (number) {
        pImpl->digits.push_back(number % radix);
        number /= radix;
    }
}
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

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>
BigInteger<Transformer, radix, Container, BaseType>::operator*(const BigInteger &rhs) const {
    auto resInt = BigInteger{0};
    if (pImpl->is_zero || rhs.pImpl->is_zero) return resInt;
    else resInt.pImpl->is_zero = false;
    if (pImpl->digits.size() == 1 && pImpl->digits.front() == 1) return {rhs};
    else if (rhs.pImpl->digits.size() == 1 && rhs.pImpl->digits.front() == 1) return {*this};
    resInt.pImpl->digits
            = pTransformer->process(
            pImpl->digits, rhs.pImpl->digits); //do the transformation
    while (resInt.pImpl->digits.back() == 0)
        resInt.pImpl->digits.pop_back();
    BaseType temp = 0;
    for (auto i = resInt.pImpl->digits.begin(), t = resInt.pImpl->digits.end(); i < t; ++i) {
        *i += temp;
        temp = 0;
        if (*i >= radix) {
            temp = (*i) / radix;
            *i %= radix;
        }
    }
    while (temp) {
        resInt.pImpl->digits.push_back(temp % radix);
        temp /= radix;
    }
    resInt.pImpl->sign = pImpl->sign == rhs.pImpl->sign;
    return resInt;
}

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
std::string
BigInteger<Transformer, radix, Container, BaseType>::to_string() const {
    if (pImpl->is_zero) return "0";
    //TODO : CACHE RADIX ZERO FILLER NUMBER
    size_t t = 0, r = radix;
    while (r) { t++, r /= 10; }
    --t;
    std::string buffer{};
    for (auto i = pImpl->digits.rbegin(), ti = pImpl->digits.rend(); i < ti; ++i) {
        std::string temp{};
        auto num = *i;
        while (num) {
            temp += ('0' + (num % 10));
            num /= 10;
        }
        while (!buffer.empty() && temp.size() < t) temp += "0";
        for (auto j = temp.rbegin(), u = temp.rend(); j < u; ++j) {
            buffer += *j;
        }
    }
    return pImpl->sign ? buffer : "-" + buffer;
}