#pragma once

#include <vector>
#include <unordered_map>

template<typename E, typename T>
struct TrieNode {
	std::unordered_map<E, TrieNode<E, T>> children;
	std::vector<T> terminals;
};
