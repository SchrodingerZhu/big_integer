#ifndef BIG_INTEGER_LIBRARY_H
#define BIG_INTEGER_LIBRARY_H

#include <cstddef>
#include <memory>
#include <vector>
#include "transformer.cpp"

template<typename Transformer = Transformer<>,
        std::size_t radix = 10,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container = std::vector,
        typename BaseType = int>
class BigInteger {
    struct Impl;
    std::unique_ptr<struct Impl> pImpl;
    static const std::unique_ptr<Transformer> pTransformer;
public:
    BigInteger();

    BigInteger(const long long &number);

    BigInteger(int number);

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

    bool is_negative() const noexcept;
    // BigInteger approx_sqrt() const noexcept;

    std::string to_string() const;

};

#endif