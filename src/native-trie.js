const trie_native = require('bindings')('native-trie')

function Trie(trieType) {
	function TrieConstructor() {
		const trie = new trieType()
		
		this.insert = function(value, node) {
			trie.insert(value.split(''), node)
		}

		this.match = function(value) {
			return trie.match(value.split(''))
		}

		return this
	}
	return TrieConstructor
}

module.exports.ObjectTrie = Trie(trie_native.StringObjectTrie)
module.exports.StringTrie = Trie(trie_native.StringStringTrie)
