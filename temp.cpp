//
// Created by Jimmy on 2019/1/19.
//

#include <iostream>
#include <vector>
#include <deque>
#include "transformer.cpp"
inline void multiply(std::deque<int>& a1, std::deque<int>& a2, std::deque<int>& res) {
    static auto fft = NTTTransformer<long long, std::deque, 998244353>();
    size_t n1 = a1.size();
    size_t n2 = a2.size();
    size_t n = 1;
    while (n < n1 + n2) n <<= 1;
    res.clear();
    res.resize(n1 + n2 - 1);
    static std::deque<long long> c1(1000), c2(1000);
    for (int i = 0; i < n1; i++) c1[i] = a1[i];
    for (int i = 0; i < n2; i++) c2[i] = a2[i];
    fft.initialize_omegas(n);
    fft.transform(c1, n, decltype(fft)::TRANSFORM);
    fft.transform(c2, n, decltype(fft)::TRANSFORM);
    for (int i = 0; i < n; i++) (c1[i] *= c2[i]) %= 998244353;
    fft.transform(c1, n, decltype(fft)::INVERSE);
    for (int i = 0; i < n1 + n2 - 1; i++) res[i] = (int) c1[i];
    while(res.back() == 0) res.pop_back();
    int temp = 0;
    for (auto i = res.rbegin(); i < res.rend(); ++i) {
        *i += temp;
        temp = (*i) / 100;
        (*i) %= 100;
    }
    while(temp) {
        res.push_front(temp % 100);
        temp /= 100;
    }
}
int main(){
    std::deque<int> a1{61, 55, 55};
    std::deque<int> a2{17, 55};
    std::deque<int> res{ };
    multiply(a1, a2, res);
    for(auto i : res) {
        std::cout << i;
    }
    return 0;
}
