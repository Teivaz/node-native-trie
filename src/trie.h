#pragma once

#include "trienode.h"

#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <limits>

struct TrieIntCore {
	using Element = int;
	using Container = std::vector<Element>;
	using TerminalNode = unsigned long long int;
};

struct TrieMatchParams {
	TrieMatchParams(bool includeExactMatch = true, std::size_t maxDepth = std::numeric_limits<std::size_t>::max(), std::size_t maxElements = std::numeric_limits<std::size_t>::max())
	: includeExactMatch(includeExactMatch)
	, maxDepth(maxDepth)
	, maxElements(maxElements) {}

	TrieMatchParams(std::size_t maxDepth = std::numeric_limits<std::size_t>::max(), std::size_t maxElements = std::numeric_limits<std::size_t>::max())
	: maxDepth(maxDepth)
	, maxElements(maxElements) {}

	bool includeExactMatch = true;
	std::size_t maxDepth = std::numeric_limits<std::size_t>::max();
	std::size_t maxElements = std::numeric_limits<std::size_t>::max();
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
		NoStopMatchResult(std::vector<typename T::TerminalNode> const& nodes, std::size_t depth) : nodes(nodes), depth(depth) {}
		NoStopMatchResult(std::vector<typename T::TerminalNode> const& nodes, std::size_t truncate, std::size_t depth)
		: nodes(std::begin(nodes), std::begin(nodes) + truncate), depth(depth) {}
		
		std::vector<typename T::TerminalNode> nodes;
		std::size_t depth;
	};

	std::vector<NoStopMatchResult> match(typename T::Container const& sequence, TrieMatchParams params) const {
		std::vector<NoStopMatchResult> result;
		std::size_t totalElements = 0;

		auto foundNode = matchNode(sequence);
		if (!foundNode) {
			return result;
		}

		struct NodeResultBfs {
			NodeResultBfs(Node const* node, std::size_t depth) : node(node), depth(depth) {}
			
			Node const* node;
			std::size_t depth;
		};

		std::deque<NodeResultBfs> sweepBuffer;
		sweepBuffer.emplace_back(foundNode, 0);

		while (!sweepBuffer.empty()) {
			auto const current = sweepBuffer.front();

			auto const hasElements = current.node->terminals.size() > 0;
			auto const shouldSkip = (current.depth == 0) && !params.includeExactMatch;
			if (hasElements && !shouldSkip) {
				if (totalElements + current.node->children.size() >= params.maxElements) {
					const std::size_t elementsToAdd = params.maxElements - totalElements;
					result.emplace_back(current.node->terminals, elementsToAdd, current.depth);
					return result;
				}
				else {
					totalElements += current.node->children.size();
					result.emplace_back(current.node->terminals, current.depth);
				}
			}

			if (params.maxDepth >= current.depth + 1) {
				for (auto const& child : current.node->children) {
					sweepBuffer.emplace_back(&child.second, current.depth + 1);
				}
			}
			sweepBuffer.pop_front();
		}
		return result;
	}

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
