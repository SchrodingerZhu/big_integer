//
// Created by Jimmy on 2019/1/19.
//
#include "transformer.h"
#include <algorithm>

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
void Transformer<DataType, Container>::transform
        (Container<DataType> &a, const size_t &n, const Transformer::Operation &type) {
    throw std::runtime_error("virtual transformer is called");
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
void Transformer<DataType, Container>::initialize_omegas(const size_t &n) {
    throw std::runtime_error("virtual transformer is called");
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
Container<DataType> Transformer<DataType, Container>::process
        (const Container<DataType> &a1, const Container<DataType> &a2) {
    size_t n1 = a1.size();
    size_t n2 = a2.size();
    size_t n = n1 + n2;
    auto res = Container<DataType>(n);
    for (size_t i = 0; i < n; ++i) {
        size_t j = 0, l = i, tj = std::min(i, n1 - 1);
        while (j <= tj && l != -1) {
            if (l < n2)
                res[i] = j == 0 ? a1[j] * a2[l] : res[i] + a1[j] * a2[l];
            j++;
            l--;
        }
    }
    return res;
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
Container<DataType> &Transformer<DataType, Container>::access(const size_t &i) {
    return omegas[i];
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
void FFTTransformer<DataType, Container>::transform
        (Container<DataType> &a, const size_t &n, const FFTTransformer::Operation &type) {
    size_t k{0};
    while ((1 << k) < n) ++k;
    for (size_t i{0}; i < n; ++i) {
        size_t t{0};
        for (size_t j = 0; j < k; ++j) if (i & (1 << j)) t |= (1 << (k - j - 1));
        if (i < t) std::swap(a[i], a[t]);
    }

    for (size_t l{2}; l <= n; l <<= 1) {
        size_t m = l >> 1;
        for (auto p{a.begin()}, pt{a.begin()}; p != pt + n; p += l) {
            for (size_t i = 0; i < m; ++i) {
                auto t = this->access(type)[n / l * i] * p[m + i];
                p[m + i] = p[i] - t;
                p[i] += t;
            }
        }
    }
    if (type == Operation::INVERSE) {
        for (auto &i : a) { i /= n; }
    }
}

template<typename DataType, template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
template<class BaseType>
Container<BaseType> FFTTransformer<DataType, Container>::
process(const Container<BaseType> &a1, const Container<BaseType> &a2) {
    size_t n1 = a1.size();
    size_t n2 = a2.size();
    size_t n = 1;
    while (n < n1 + n2) n <<= 1;
    auto res = Container<BaseType>(n);
    static Container<DataType> c1(n), c2(n); // TODO: space management
    for (int i = 0; i < n1; i++) c1[i].real(a1[i]);
    for (int i = 0; i < n2; i++) c2[i].real(a2[i]);
    initialize_omegas(n);
    transform(c1, n, Operation::TRANSFORM);
    transform(c2, n, Operation::TRANSFORM);
    for (int i = 0; i < n; i++) c1[i] *= c2[i];
    transform(c1, n, Operation::INVERSE);
    for (int i = 0; i < n1 + n2 - 1; i++) res[i] = static_cast<BaseType>(c1[i].real());
    return res;
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container>
void FFTTransformer<DataType, Container>::initialize_omegas(const size_t &n) {
    this->access(0).clear();
    this->access(1).clear();
    for (size_t i{0}; i < n; ++i) {
        this->access(0).emplace_back(DataType{cos(2 * PI / n * i), sin(2 * PI / n * i)});
        this->access(1).emplace_back(std::conj(this->access(0)[i]));
    }
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
constexpr DataType
NTTTransformer<DataType, Container, MOD>::__pow(DataType a, DataType n, const DataType &mod) noexcept {
    auto res = DataType{1};
    while (n) {
        if (n & 1) {
            res = __multiply(res, a, mod);
        }
        a = __multiply(a, a, mod);
        n >>= 1;
    }
    return res;
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
constexpr DataType NTTTransformer<DataType, Container, MOD>::__root(const DataType &p) {
    for (size_t i = 2; i <= p; ++i) {
        size_t x = p - 1;
        bool flag = false;
        for (size_t k = 2; k * k <= p - 1; ++k)
            if (x % k == 0) {
                if (__pow(i, (p - 1) / k, p) == 1) {
                    flag = true;
                    break;
                }
                while (x % k == 0) x /= k;
            }
        if (!flag && (x == 1 || __pow(i, (p - 1) / x, p) != 1)) {
            return i;
        }
    }
    throw std::runtime_error("failed to calculate the root, wrong data type?");
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
void NTTTransformer<DataType, Container, MOD>::__exgcd(const DataType &a, const DataType &b, DataType &g, DataType &x,
                                                       DataType &y) noexcept {
    // TODO: refactor recursion
    if (!b) g = a, x = 1, y = 0;
    else __exgcd(b, a % b, g, y, x), y -= x * (a / b);
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
constexpr DataType NTTTransformer<DataType, Container, MOD>::__inv(const DataType &a, const DataType &p) noexcept {
    DataType g{}, x{}, y{};
    __exgcd(a, p, g, x, y);
    return (x + p) % p;
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
void NTTTransformer<DataType, Container, MOD>::initialize_omegas(const size_t &n) {
    DataType g = __root(MOD);
    auto x = __pow(g, (MOD - 1) / n, MOD);
    for (int i = 0; i < n; ++i) {
        this->access(0).emplace_back((i == 0) ? 1 : __multiply(this->access(0).back(), x, MOD));
        this->access(1).emplace_back(std::move(__inv(this->access(0).back(), MOD)));
    }
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
void NTTTransformer<DataType, Container, MOD>::transform(Container<DataType> &a, const size_t &n,
                                                         const NTTTransformer::Operation &type) {
    size_t k{0};
    while ((1 << k) < n) ++k;
    for (size_t i{0}; i < n; ++i) {
        size_t t{0};
        for (size_t j = 0; j < k; ++j) if (i & (1 << j)) t |= (1 << (k - j - 1));
        if (i < t) std::swap(a[i], a[t]);
    }
    for (size_t l{2}; l <= n; l <<= 1) {
        size_t m = l >> 1;
        for (auto p{a.begin()}, pt{a.begin()}; p != pt + n; p += l) {
            for (size_t i = 0; i < m; ++i) {
                auto t = this->access(type)[n / l * i] * p[m + i] % MOD;
                p[m + i] = (p[i] - t + MOD) % MOD;
                (p[i] += t) %= MOD;
            }
        }
    }
    if (type == Operation::INVERSE) {
        auto x = __inv(n, MOD);
        for (auto &i : a) i = __multiply(i, x, MOD);
    }
}

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container, DataType MOD>
template<class BaseType>
Container<DataType>
NTTTransformer<DataType, Container, MOD>::process(const Container<BaseType> &a1, const Container<BaseType> &a2) {
    size_t n1 = a1.size();
    size_t n2 = a2.size();
    size_t n = 1;
    while (n < n1 + n2) n <<= 1;
    auto res = Container<BaseType>(n);
    static Container<DataType> c1(n), c2(n); // TODO: space management
    for (int i = 0; i < n1; i++) c1[i] = static_cast<DataType>(a1[i]);
    for (int i = 0; i < n2; i++) c2[i] = static_cast<DataType>(a2[i]);
    initialize_omegas(n);
    transform(c1, n, Operation::TRANSFORM);
    transform(c2, n, Operation::TRANSFORM);
    for (int i = 0; i < n; i++) (c1[i] *= c2[i]) %= MOD;
    transform(c1, n, Operation::INVERSE);
    for (int i = 0; i < n1 + n2 - 1; i++) res[i] = static_cast<BaseType>(c1[i]);
    return res;
}