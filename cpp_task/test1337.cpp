#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <vector>

#include "interval_map.cpp"

int main()
{
    interval_map<int, char> intvlmap('A');
    std::vector<char> model(11, 'A');

    std::mt19937 rng(1337);
    std::uniform_int_distribution<int> keyDist(-5, 5);
    std::uniform_int_distribution<int> valDist(0, 2);
    const char values[] = {'A', 'B', 'C'};

    for (int i = 0; i < 200; ++i)
    {
        int a = keyDist(rng);
        int b = keyDist(rng);
        if (b < a) std::swap(a, b);
        char v = values[valDist(rng)];
        
        intvlmap.assign(a, b, v);
        for (int k = a; k < b; ++k)
            model[k + 5] = v;
        
        for (int k = -5; k <= 5; ++k)
        {
            if (intvlmap[k] != model[k + 5])
            {
                std::cout << "FAIL iter=" << i << " assign(" << a << "," << b << ",'" << v << "')" << std::endl;
                std::cout << "  key " << k << ": got '" << intvlmap[k] << "' expected '" << model[k + 5] << "'" << std::endl;
                return 1;
            }
        }
    }
    std::cout << "Test passed!" << std::endl;
    return 0;
}
