#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

template<typename E, typename T>
struct TrieNode {
	std::unordered_map<E, std::unique_ptr<TrieNode<E, T>>> children;
	std::vector<T> terminals;
};
