//
// Created by Jimmy on 2019/1/19.
//

#ifndef BIG_INTEGER_TRANSFORMER_H
#define BIG_INTEGER_TRANSFORMER_H

#include <memory>
#include <vector>
#include <complex>

/*
 * This is the abstract data structure of transformer
 * Notice that FFT's Container must be the same as the outer wrapper
 */

template<typename DataType,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container = std::vector>
class Transformer {
    Container<DataType> omegas[2]{{},
                                  {}};
public:
    enum Operation {
        TRANSFORM = 0, INVERSE = 1
    };

private:
    virtual void initialize_omegas(const size_t &n);

    virtual void transform(Container<DataType> &a, const size_t &n, const Operation &type);

public:
    Container<DataType> &access(const size_t &i);

    virtual Container<DataType> process(const Container<DataType> &a1, const Container<DataType> &a2);
};

template<typename DataType = std::complex<long double>,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container = std::vector>
class FFTTransformer : public Transformer<DataType, Container> {
    const __float128 PI{acos(-1)};
    using Operation = typename Transformer<DataType, Container>::Operation;

    void initialize_omegas(const size_t &n) override;

    void transform(Container<DataType> &a, const size_t &n, const Operation &type) override;

public:
    template<class BaseType>
    Container<BaseType> process(const Container<BaseType> &a1, const Container<BaseType> &a2);
};

template<typename DataType = long long,
        template<typename _Tp, typename _Alloc = std::allocator<_Tp>> typename Container = std::vector,
        DataType MOD = 998244353>
class NTTTransformer : public Transformer<DataType, Container> {
    using Operation = typename Transformer<DataType, Container>::Operation;

    static constexpr DataType
    __multiply(const DataType &a, const DataType &b, const DataType &c) noexcept {
        return static_cast<DataType>(((a * b - (long long) ((long double) a / c * b + 1e-8) * c) % c + c) % c);
    }

    static constexpr DataType __pow(DataType a, DataType n, const DataType &mod) noexcept;

    static constexpr DataType __root(const DataType &p);

    static void __exgcd(const DataType &a, const DataType &b, DataType &g, DataType &x, DataType &y) noexcept;

    static constexpr DataType __inv(const DataType &a, const DataType &p) noexcept;

    void initialize_omegas(const size_t &n) override;

    void transform(Container<DataType> &a, const size_t &n, const Operation &type) override;

public:
    template<class BaseType>
    Container<DataType> process(const Container<BaseType> &a1, const Container<BaseType> &a2);

};

#endif //BIG_INTEGER_TRANSFORMER_H
