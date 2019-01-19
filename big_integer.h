#ifndef BIG_INTEGER_LIBRARY_H
#define BIG_INTEGER_LIBRARY_H

#include <cstddef>
#include <memory>
#include <vector>
#include "transformer.h"

template<typename Transformer = NTTTransformer<>,
        std::size_t radix = 4,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container = std::vector,
        typename BaseType = int>
class BigInteger {
    struct Impl;
    std::unique_ptr<struct Impl> pImpl;
public:
    BigInteger();

    BigInteger(const long long &number);

    BigInteger(const int &number);

    BigInteger(const short &number);

    BigInteger(const char &number);

    BigInteger(const unsigned long long &number);

    BigInteger(const unsigned int &number);

    BigInteger(const unsigned short &number);

    BigInteger(const unsigned char &number);

    BigInteger(const bool &number);

    explicit BigInteger(const std::string &str);

    BigInteger(const BigInteger &rhs);

    BigInteger(BigInteger &&rhs) noexcept;

    ~BigInteger();

    BigInteger &operator=(const BigInteger &rhs);

    BigInteger &operator=(BigInteger &&rhs) noexcept;

    BigInteger operator+(const BigInteger &rhs) const;

    BigInteger operator-(const BigInteger &rhs) const;

    BigInteger operator*(const BigInteger &rhs) const;

    BigInteger operator/(const BigInteger &rhs) const;

    BigInteger operator^(const BigInteger &rhs) const;

    BigInteger operator&(const BigInteger &rhs) const;

    BigInteger &operator+=(const BigInteger &rhs);

    BigInteger &operator-=(const BigInteger &rhs);

    BigInteger &operator*=(const BigInteger &rhs);

    BigInteger &operator/=(const BigInteger &rhs);

    BigInteger &operator^=(const BigInteger &rhs);

    BigInteger &operator&=(const BigInteger &rhs);

    BigInteger operator!() const;

    BigInteger &operator++();

    const BigInteger operator++(int);

    BigInteger &operator--();

    const BigInteger operator--(int);

    BigInteger operator~() const;

    BigInteger slow_multiply(const BigInteger &rhs) const;

    bool is_negative() const noexcept;
    // BigInteger approx_sqrt() const noexcept;

};

#endif