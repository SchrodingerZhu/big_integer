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
    Container<BaseType> digits{};
};

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
BigInteger<Transformer, radix, Container, BaseType>::BigInteger(int number) : pImpl(std::make_unique<Impl>()) {
    std::stack<BaseType> temp;
    while (number) {
        temp.push(number % radix);
        number /= radix;
    }
    while (!temp.empty()) {
        pImpl->digits.push_back(temp.top());
        temp.pop();
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
    auto resInt = BigInteger{};
    resInt.pImpl->digits
            = pTransformer->process(
            pImpl->digits, rhs.pImpl->digits); //do the transformation
    BaseType temp = 0;
    for (auto i = resInt.pImpl->digits.rbegin(), t = resInt.pImpl->digits.rend(); i < t; ++i) {
        *i += temp;
        temp = 0;
        if (*i >= radix) {
            temp = (*i) / radix;
            *i %= radix;
        }
    }
    size_t t = 0;
    auto m = temp;
    while (m) {
        t++;
        m /= radix;
    } //handle radix

    if (t) {
        resInt.pImpl->digits.resize(resInt.pImpl->digits.size() + t);
        std::copy(resInt.pImpl->digits.begin(), resInt.pImpl->digits.end(), resInt.pImpl->digits.begin() + t);
        while (temp) {
            resInt.pImpl->digits[--t] = temp % radix;
            temp /= radix;
        }
    } //handle exceeded position
    while (resInt.pImpl->digits.back() > pImpl->digits.back() + rhs.pImpl->digits.back())
        resInt.pImpl->digits.pop_back();
    return resInt;
}

template<typename Transformer, std::size_t radix,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, typename BaseType>
std::string
BigInteger<Transformer, radix, Container, BaseType>::to_string() const {
    //TODO : CACHE RADIX ZERO FILLER NUMBER
    size_t t = 0, r = radix;
    while (r) { t++, r /= 10; }
    --t;
    std::string buffer{};
    for (auto i = pImpl->digits.begin(), ti = pImpl->digits.end(); i < ti; ++i) {
        std::string temp{};
        auto num = *i;
        while (num) {
            temp += '0' + (num % 10);
            num /= 10;
        }
        while (buffer.size() && temp.size() < t) temp += '0';
        for (auto j = temp.rbegin(), u = temp.rend(); j < u; ++j) {
            buffer += *j;
        }
    }
    return buffer;
}