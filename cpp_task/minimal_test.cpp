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

template<typename IM>
void checkMinimal(const IM &m)
{
    if (!m.m_map.empty())
    {
        if (m.m_map.begin()->second == m.m_valBegin) {
            std::cout << "FAIL: first entry equals m_valBegin" << std::endl;
            assert(false);
        }
    }
    auto it = m.m_map.begin();
    if (it == m.m_map.end()) return;
    auto prev = it++;
    for (; it != m.m_map.end(); ++it, ++prev)
    {
        if (prev->second == it->second) {
            std::cout << "FAIL: consecutive entries have same value" << std::endl;
            assert(false);
        }
    }
}

void IntervalMapTest()
{
    for (int seed = 0; seed < 10000; ++seed)
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

            checkMinimal(im);

            for (int k = -25; k <= 25; ++k)
            {
                if (im[MyKey{k}].v != rm[MyKey{k}].v)
                {
                    std::cout << "FAIL seed=" << seed << " iter=" << i << std::endl;
                    assert(false);
                }
            }
        }
    }
    std::cout << "All 10000 seeds passed with minimal check!" << std::endl;
}

int main()
{
    IntervalMapTest();
    return 0;
}
