#include <cassert>
#include <iostream>
#include <random>

#include "interval_map.cpp"

struct MyKey
{
    int v;
    MyKey(int x = 0) : v(x) {}
    friend bool operator<(MyKey const &a, MyKey const &b)
    {
        return a.v < b.v;
    }
};

// Value: only copy/move + operator==
struct MyValue
{
    char v;
    MyValue(char c = 0) : v(c) {}
    friend bool operator==(MyValue const &a, MyValue const &b)
    {
        return a.v == b.v;
    }
};

void IntervalMapTest()
{
    auto checkMinimal = [](const auto &m)
    {
        if (!m.m_map.empty())
        {
            assert(m.m_map.begin()->second != m.m_valBegin);
        }
        auto it = m.m_map.begin();
        if (it == m.m_map.end())
        {
            return;
        }
        auto prev = it++;
        for (; it != m.m_map.end(); ++it, ++prev)
        {
            assert(prev->second != it->second);
        }
    };

    {
        interval_map<MyKey, MyValue> m(MyValue{'A'});
        m.assign(MyKey{1}, MyKey{3}, MyValue{'B'});

        assert(m[MyKey{0}].v == 'A');
        assert(m[MyKey{1}].v == 'B');
        assert(m[MyKey{2}].v == 'B');
        assert(m[MyKey{3}].v == 'A');
        // check_invariants(m);
    }

    /* --------------------------------------------------------
       Original task-provided tests (UNCHANGED)
       -------------------------------------------------------- */
    {
        interval_map<int, char> intvlmap('A');

        intvlmap.assign(1, 3, 'B');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'B');
        assert(intvlmap[3] == 'A');
        assert(intvlmap[4] == 'A');

        intvlmap.assign(2, 4, 'C');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'C');
        assert(intvlmap[3] == 'C');
        assert(intvlmap[4] == 'A');

        intvlmap.assign(3, 1, 'D');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'C');
        assert(intvlmap[3] == 'C');
        assert(intvlmap[4] == 'A');

        intvlmap.assign(1, 4, 'A');
        assert(intvlmap.m_map.size() == 0);
    }

    /* --------------------------------------------------------
       Additional deterministic edge cases
       -------------------------------------------------------- */
    {
        interval_map<int, char> m('A');
        m.assign(1, 5, 'B');
        m.assign(2, 4, 'A');

        assert(m[1] == 'B');
        assert(m[2] == 'A');
        assert(m[3] == 'A');
        assert(m[4] == 'B');
        assert(m[5] == 'A');
        // check_invariants(m);
    }

    {
        interval_map<int, char> m('A');
        m.assign(1, 2, 'B');
        m.assign(2, 3, 'B'); // must merge

        assert(m.m_map.size() == 2);
        assert(m[1] == 'B');
        assert(m[3] == 'A');
        // check_invariants(m);
    }

    {
        interval_map<int, char> m('A');
        m.assign(5, 5, 'B'); // empty

        assert(m.m_map.empty());
        assert(m[0] == 'A');
        // check_invariants(m);
    }

    {
        // some basic test cases for your inspiration
        // we encourage you to add more tests below

        // initially, entire range of int -> 'A'
        interval_map<int, char> intvlmap('A');

        // change [1,3) -> 'B'
        intvlmap.assign(2, 2, 'B');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'A');
        assert(intvlmap[3] == 'A');
        assert(intvlmap[4] == 'A');

        // change [2,4) -> 'C'
        intvlmap.assign(2, 2, 'C');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'A');
        assert(intvlmap[2] == 'C');
        assert(intvlmap[3] == 'A');
        assert(intvlmap[4] == 'A');

        // empty interval -> do nothing
        intvlmap.assign(3, 1, 'D');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'A');
        assert(intvlmap[2] == 'C');
        assert(intvlmap[3] == 'A');
        assert(intvlmap[4] == 'A');

        // representation inside m_map must be minimal
        intvlmap.assign(1, 4, 'A');
        // assert(intvlmap.m_map.size() == 0);
    }
    {
        // some basic test cases
        interval_map<int, char> intvlmap('A');

        intvlmap.assign(1, 3, 'B');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'B');
        assert(intvlmap[3] == 'A');
        assert(intvlmap[4] == 'A');
        checkMinimal(intvlmap);

        intvlmap.assign(2, 4, 'C');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'C');
        assert(intvlmap[3] == 'C');
        assert(intvlmap[4] == 'A');
        checkMinimal(intvlmap);

        intvlmap.assign(3, 1, 'D');
        assert(intvlmap[0] == 'A');
        assert(intvlmap[1] == 'B');
        assert(intvlmap[2] == 'C');
        assert(intvlmap[3] == 'C');
        assert(intvlmap[4] == 'A');
        checkMinimal(intvlmap);

        intvlmap.assign(1, 4, 'A');
        assert(intvlmap.m_map.size() == 0);
        checkMinimal(intvlmap);
    }
    {
        // no-op if assigning same value as existing
        interval_map<int, char> intvlmap('X');
        intvlmap.assign(0, 5, 'X');
        assert(intvlmap.m_map.empty());
        checkMinimal(intvlmap);
    }
    for (int round = 0; round < 1000; ++round)
    {
        // Randomized test: keys from -100 to 100, 5 possible values
        const char values[] = {'A', 'B', 'C', 'D', 'E'};
        const int MIN_KEY = -100;
        const int MAX_KEY = 100;
        const int RANGE = MAX_KEY - MIN_KEY + 1; // 201 keys

        interval_map<int, char> intvlmap('A');
        char model[RANGE];
        for (int i = 0; i < RANGE; ++i)
            model[i] = 'A';

        std::mt19937 rng(round); // Different seed each round
        std::uniform_int_distribution<int> keyDist(MIN_KEY, MAX_KEY);
        std::uniform_int_distribution<int> valDist(0, 4);

        for (int iter = 0; iter < 1000; ++iter)
        {
            // Generate random interval and value
            int a = keyDist(rng);
            int b = keyDist(rng);
            if (b < a)
                std::swap(a, b);
            char v = values[valDist(rng)];

            // Apply assign
            intvlmap.assign(a, b, v);

            // Update model
            for (int k = a; k < b; ++k)
                model[k - MIN_KEY] = v;

            // Check against full model
            for (int k = MIN_KEY; k <= MAX_KEY; ++k)
                assert(intvlmap[k] == model[k - MIN_KEY]);

            checkMinimal(intvlmap);
        }
    }

    std::cout << "All tests succeeded." << std::endl;
}

int main()
{
    std::cout << "Running tests..." << std::endl;
    IntervalMapTest();
    std::cout << "Tests completed." << std::endl;
    return 0;
}
