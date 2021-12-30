#pragma once


#include <unordered_map>


template <class Key, class Val>
const Val* GetVal(const std::unordered_map<Key, Val>& map, const Key& key) {
    auto it = map.find(key);
    return it == map.end() ? nullptr : &it->second;
}
