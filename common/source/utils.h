#pragma once


#include <unordered_map>
#include <string>


std::string Replace(const std::string& source, const std::string& from, const std::string& to);


template <class Key, class Val>
const Val* GetVal(const std::unordered_map<Key, Val>& map, const Key& key);


// Implementations

template <class Key, class Val>
const Val* GetVal(const std::unordered_map<Key, Val>& map, const Key& key) {
    auto it = map.find(key);
    return it == map.end() ? nullptr : &it->second;
}
