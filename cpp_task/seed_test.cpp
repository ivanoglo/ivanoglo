#include <cassert>
#include <iostream>
#include <map>
#include <random>

#include "interval_map.cpp"

struct MyKey
{
    int v;
    MyKey(int x = 0) : v(x) {}
    friend bool operator<(MyKey const &a, MyKey const &b) { return a.v < b.v; }
};

struct MyValue
{
    char v;
    MyValue(char c = 0) : v(c) {}
    friend bool operator==(MyValue const &a, MyValue const &b) { return a.v == b.v; }
};

template <typename K, typename V>
struct ReferenceMap
{
    std::map<K, V> m;
    V defaultValue;
    ReferenceMap(V v) : defaultValue(v) {}
    void assign(K const &b, K const &e, V const &v)
    {
        if (!(b < e)) return;
        const V after = (*this)[e];
        m.erase(m.lower_bound(b), m.lower_bound(e));
        m[b] = v;
        m[e] = after;
    }
    V operator[](K const &k) const
    {
        auto it = m.upper_bound(k);
        if (it == m.begin()) return defaultValue;
        return std::prev(it)->second;
    }
};

bool testWithSeed(int seed)
{
    interval_map<MyKey, MyValue> im(MyValue{'A'});
    ReferenceMap<MyKey, MyValue> rm(MyValue{'A'});

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> keyDist(-20, 20);
    std::uniform_int_distribution<int> valDist(0, 2);

    for (int i = 0; i < 500; ++i)
    {
        int a = keyDist(rng);
        int b = keyDist(rng);
        if (b < a) std::swap(a, b);
        char v = "ABC"[valDist(rng)];

        im.assign(MyKey{a}, MyKey{b}, MyValue{v});
        rm.assign(MyKey{a}, MyKey{b}, MyValue{v});

        for (int k = -25; k <= 25; ++k)
        {
            if (im[MyKey{k}].v != rm[MyKey{k}].v)
            {
                std::cout << "FAIL seed=" << seed << " iter=" << i << std::endl;
                std::cout << "  assign(" << a << ", " << b << ", '" << v << "')" << std::endl;
                std::cout << "  key " << k << ": im=" << im[MyKey{k}].v << " rm=" << rm[MyKey{k}].v << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main()
{
    for (int seed = 0; seed < 10000; ++seed)
    {
        if (!testWithSeed(seed)) return 1;
    }
    std::cout << "All 10000 seeds passed!" << std::endl;
    return 0;
}
