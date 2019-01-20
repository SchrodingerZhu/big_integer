//
// Created by Jimmy on 2019/1/19.
//

#include <iostream>
#include <vector>
#include <deque>
#include "big_integer.h"
#include "big_integer.cpp"
//inline void multiply(std::deque<int>& a1, std::deque<int>& a2, std::deque<int>& res) {
//    static auto fft = NTTTransformer<long long, std::deque, 998244353>();
//    size_t n1 = a1.size();
//    size_t n2 = a2.size();
//    size_t n = 1;
//    while (n < n1 + n2) n <<= 1;
//    res.clear();
//    res.resize(n1 + n2 - 1);
//    static std::deque<long long> c1(1000), c2(1000);
//    for (int i = 0; i < n1; i++) c1[i] = a1[i];
//    for (int i = 0; i < n2; i++) c2[i] = a2[i];
//    fft.initialize_omegas(n);
//    fft.transform(c1, n, decltype(fft)::TRANSFORM);
//    fft.transform(c2, n, decltype(fft)::TRANSFORM);
//    for (int i = 0; i < n; i++) (c1[i] *= c2[i]) %= 998244353;
//    fft.transform(c1, n, decltype(fft)::INVERSE);
//    for (int i = 0; i < n1 + n2 - 1; i++) res[i] = (int) c1[i];
//    while(res.back() == 0) res.pop_back();
//    int temp = 0;
//    for (auto i = res.rbegin(); i < res.rend(); ++i) {
//        *i += temp;
//        temp = (*i) / 100;
//        (*i) %= 100;
//    }
//    while(temp) {
//        res.push_front(temp % 100);
//        temp /= 100;
//    }
//}
int main(){
    auto test = BigInteger<FFTTransformer<>, 10, std::vector, long long>(711);
    auto test2 = BigInteger<FFTTransformer<>, 1000>(121212);
    auto res1 = test * test;
    auto res2 = res1 * test;

    auto wtest = BigInteger<Transformer<>>(711);
    auto wtest2 = BigInteger<Transformer<>>(-3525412421300);
    auto wres = wtest * wtest * wtest * wtest * wtest;

    auto etest = BigInteger<NTTTransformer<>, 10, std::vector, long long>(711);
    auto etest2 = BigInteger<NTTTransformer<>, 1000>(121);
    auto eres1 = etest * etest;
    auto eres2 = eres1 * etest;

    std::cout << eres2.to_string() << std::endl;
    std::cout << res2.to_string() << std::endl;
}
