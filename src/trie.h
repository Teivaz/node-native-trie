#pragma once

#include "trienode.h"

#include <vector>
#include <utility>
#include <algorithm>

// E - element type is POD
// C - container type

struct TrieIntCore {
	using Element = int;
	using Container = std::vector<Element>;
	using TerminalNode = unsigned long long int;
};

template<typename T = TrieIntCore>
class Trie {
public:

	void insert(typename T::Container const& sequence, typename T::TerminalNode const& node) {
		auto matched = matchExisting(sequence);
		auto& trieNode = insert(matched.second, std::end(sequence), matched.first);
		trieNode.terminals.push_back(node);
	}

	void insert(typename T::Container const& sequence, typename T::TerminalNode&& node) {
		auto matched = matchExisting(sequence);
		auto& trieNode = insert(matched.second, std::end(sequence), matched.first);
		trieNode.terminals.push_back(std::move(node));
	}

	std::vector<typename T::TerminalNode> match(typename T::Container const& sequence) const {
		auto foundNode = matchNode(sequence);
		if (!foundNode) {
			return {};
		}
		return foundNode->terminals;
	}

	std::vector<typename T::TerminalNode const*> matchRef(typename T::Container const& sequence) const {
		auto foundNode = matchNode(sequence);
		if (!foundNode) {
			return {};
		}
		std::vector<typename T::TerminalNode const*> result;
		result.reserve(foundNode->terminals.size());
		std::transform(std::begin(foundNode->terminals), std::end(foundNode->terminals), std::back_inserter(result), [](typename T::TerminalNode const& a){return &a;});
		return result;
	}

	struct NoStopMatchResult {
		std::vector<typename T::TerminalNode> nodes;
		std::size_t depth;
	};

/*
	std::vector<NoStopMatchResult> matchNoStop(typename T::Container const& sequence, const std::size_t limit = 0) const {
		std::vector<NoStopMatchResult> result;
		
		auto foundNode = matchNode(sequence);
		if (!foundNode) {
			return result;
		}
		result.emplace_back({foundNode->terminals, 0});

		std::list<Node*> a;
		a.

		foundNode

		auto currentIterator = std::begin(sequence);
		auto currentNode = &m_root;

		while (currentIterator != std::end(sequence)) {
			auto found = currentNode->children.find(*currentIterator);
			if (found == std::end(currentNode->children)) {
				return {};
			}
			currentNode = &found->second;
			++currentIterator;
		}
		return currentNode->terminals;
	}
*/

private:
	using Node = TrieNode<typename T::Element, typename T::TerminalNode>;

	Node const* matchNode(typename T::Container const& sequence) const {
		auto currentIterator = std::begin(sequence);
		auto currentNode = &m_root;

		while (currentIterator != std::end(sequence)) {
			auto found = currentNode->children.find(*currentIterator);
			if (found == std::end(currentNode->children)) {
				return nullptr;
			}
			currentNode = &found->second;
			++currentIterator;
		}
		return currentNode;
	}

	std::pair<Node&, typename T::Container::const_iterator> matchExisting(typename T::Container const& sequence) {
		Node* currentNode = &m_root;
		auto currentIterator = std::begin(sequence);
		const auto end = std::end(sequence);
		while (currentIterator != end) {
			auto found = currentNode->children.find(*currentIterator);
			if (found == std::end(currentNode->children)) {
				break;
			}
			++currentIterator;
			currentNode = &found->second;
		}
		return {*currentNode, currentIterator};
	}

	Node& insert(typename T::Container::const_iterator from, typename T::Container::const_iterator to, Node& at) const {
		auto dest = &at;
		while (from != to) {
			auto const nodeValue = *from++;
			auto inserted = dest->children.insert(std::make_pair(nodeValue, Node())).first;
			dest = &inserted->second;
		}
		return *dest;
	}

private:
	Node m_root;
};
