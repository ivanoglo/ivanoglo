#include <map>
#include <type_traits>
#include <utility>

template <typename K, typename V>
class interval_map
{
    friend void IntervalMapTest();
    V m_valBegin;
    std::map<K, V> m_map;

public:
    // constructor associates whole range of K with val
    template <typename V_forward>
    interval_map(V_forward &&val)
        : m_valBegin(std::forward<V_forward>(val))
    {
    }

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !(keyBegin < keyEnd), this designates an empty interval,
    // and assign must do nothing.
    template <typename V_forward>
    void assign(K const &keyBegin,
                K const &keyEnd,
                V_forward &&val)
        requires(std::is_same<
                 std::remove_cvref_t<V_forward>, V>::value)
    {
        if (!(keyBegin < keyEnd))
            return;

        // Single O(log N) operation
        auto it = m_map.lower_bound(keyBegin);

        // Value before keyBegin
        V prevValue = (it == m_map.begin()) ? m_valBegin : std::prev(it)->second;
        V afterValue = prevValue;

        // Erase all keys in [keyBegin, keyEnd), save last value
        while (it != m_map.end() && it->first < keyEnd)
        {
            afterValue = it->second;
            it = m_map.erase(it);
        }

        // Check if keyEnd exists
        bool keyEndExists = (it != m_map.end() && !(keyEnd < it->first));
        if (keyEndExists)
        {
            afterValue = it->second;
            if (afterValue == val)
                it = m_map.erase(it);
        }

        // Insert at keyBegin if needed (skip if would be first entry equal to m_valBegin)
        if (!(prevValue == val))
            it = m_map.emplace_hint(it, keyBegin, std::forward<V_forward>(val));

        // Insert at keyEnd if needed
        if (!(afterValue == val) && !keyEndExists)
            m_map.emplace_hint(it, keyEnd, std::move(afterValue));
    }

    // look-up of the value associated with key
    V const &operator[](K const &key) const
    {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin())
        {
            return m_valBegin;
        }
        else
        {
            return (--it)->second;
        }
    }
};
