#include "trie.h"
#include <string>
#include <iostream>

struct TrieStringCore {
	using Element = int;
	using Container = std::vector<Element>;
	using TerminalNode = std::string;
};

int main() {
	auto t = Trie<TrieStringCore>();
	t.insert({1, 2, 3, 4}, "1234a");
	t.insert({1, 2, 3, 4}, "1234b");
	t.insert({1, 2, 3, 4, 4}, "12344");
	t.insert({2, 2, 2}, "222");
	std::vector<std::string> result;

	result = t.match({1});
	std::cout << "matching 'a':\n";
	for (const auto a : result) {
		std::cout << a << ", ";
	}
	std::cout << "\n";
	result.clear();


	result = t.match({1, 2, 3, 4});
	std::cout << "matching 'abcd':\n";
	for (const auto a : result) {
		std::cout << a << ", ";
	}
	std::cout << "\n";
	result.clear();

	result = t.match({1, 2, 3, 4, 4});
	std::cout << "matching 'abcde':\n";
	for (const auto a : result) {
		std::cout << a << ", ";
	}
	std::cout << "\n";
	result.clear();
}
