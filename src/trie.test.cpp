#include "trie.h"
#include <string>
#include <sstream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

struct TrieStringCore {
	using Element = char;
	using Container = std::string;
	using TerminalNode = std::string;
};

using StringTrie = Trie<TrieStringCore>;
using Match = StringTrie::PartialMatchResult;

std::string operator+(Match m) {
	std::stringstream s;
	s << "{";
	bool firstKey = true;
	for (auto i : m) {
		if (firstKey) {
			firstKey = false;
		}
		else {
			s << ", ";
		}
		s << i.first << ":[";
		bool firstValue = true;
		for (std::string const* value : i.second) {
			if (firstValue) {
				firstValue = false;
			}
			else {
				s << ",";
			}
			s << "'" << *value << "'";
		}
		s << "]";
	}
	s << "}";
	return s.str();
}

TEST_CASE("matchPartial | empty", "[partial]") {
	auto t = StringTrie();
	REQUIRE(+t.match("string", {}) == "{}");
}

TEST_CASE("matchPartial | no match", "[partial]") {
	auto t = StringTrie();
	t.insert("string", "value");
	REQUIRE(+t.match("string2", {}) == "{}");
}

TEST_CASE("matchPartial | match - incomplete", "[partial]") {
	auto t = StringTrie();
	t.insert("string", "value");
	REQUIRE(+t.match("str", {}) == "{3:['value']}");
}

TEST_CASE("matchPartial | no match - different", "[partial]") {
	auto t = StringTrie();
	t.insert("string", "value");
	REQUIRE(+t.match("grtsadda", {}) == "{}");
}

TEST_CASE("matchPartial | multiple partial match", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "value 1");
	t.insert("abcd", "value 2");
	REQUIRE(+t.match("a", {}) == "{3:['value 1','value 2']}");
}

TEST_CASE("matchPartial | multiple level partial match", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "value 1");
	t.insert("abcde", "value 2");
	REQUIRE(+t.match("a", {}) == "{3:['value 1'], 4:['value 2']}");
}

TEST_CASE("matchPartial | complex match", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "1.1");
	t.insert("abcd", "1.2");
	t.insert("abcde", "2.1");
	t.insert("abcde", "2.2");
	t.insert("abcdefg", "4.1");
	t.insert("abcdefg", "4.2");
	t.insert("bcdef", "0");
	REQUIRE(+t.match("abcd", {}) == "{0:['1.1','1.2'], 1:['2.1','2.2'], 3:['4.1','4.2']}");
}

TEST_CASE("matchPartial | constrained partial match: no exact match", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "1.1");
	t.insert("abcd", "1.2");
	t.insert("abcde", "2.1");
	t.insert("abcde", "2.2");
	t.insert("abcdefg", "4.1");
	t.insert("abcdefg", "4.2");
	t.insert("bcdef", "0");
	REQUIRE(+t.match("abcd", TrieMatchParams{false}) == "{1:['2.1','2.2'], 3:['4.1','4.2']}");
}

TEST_CASE("matchPartial | constrained | limited depth", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "1.1");
	t.insert("abcd", "1.2");
	t.insert("abcde", "2.1");
	t.insert("abcde", "2.2");
	t.insert("abcdefg", "4.1");
	t.insert("abcdefg", "4.2");
	t.insert("bcdef", "0");
	REQUIRE(+t.match("abcd", TrieMatchParams{true, 2}) == "{0:['1.1','1.2'], 1:['2.1','2.2']}");
}

TEST_CASE("matchPartial | constrained partial match | limited count without exact", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "1.1");
	t.insert("abcd", "1.2");
	t.insert("abcde", "2.1");
	t.insert("abcde", "2.2");
	t.insert("abcdefg", "4.1");
	t.insert("abcdefg", "4.2");
	t.insert("bcdef", "0");
	REQUIRE(+t.match("abcd", TrieMatchParams{false, (std::size_t)-1, 1}) == "{1:['2.1']}");
}

TEST_CASE("matchPartial | constrained partial match | limited number", "[partial]") {
	auto t = StringTrie();
	t.insert("abcd", "1.1");
	t.insert("abcd", "1.2");
	t.insert("abcde", "2.1");
	t.insert("abcde", "2.2");
	t.insert("abcdefg", "4.1");
	t.insert("abcdefg", "4.2");
	t.insert("bcdef", "0");
	REQUIRE(+t.match("abcd", TrieMatchParams{true, (std::size_t)-1, 3}) == "{0:['1.1','1.2'], 1:['2.1']}");
}
