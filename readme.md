# Native NodeJS trie module

```
npm i native-trie
```

It is designed to be fast and handle large tries.

Currently stores whole trie in memory.
Currently has only two methods: `insert` and `match`.

The nodes of the trie graph are unicode code points. And in the memory are stored as short UTF-8 strings but can be easily expanded to 32 bit integers which can fit any unicode code point.

*Note* the combining characters and diacritical marks are treated as a separate nodes in the graph so symbols like 'é' ('\u00E9') and 'é' ('\u0065\u0301') will be represented as different entities. If you need these cases to be identical consider using text normalization before calling `insert` and `match`.

### The `insert` function

The `insert` function creates all required nodes in the trie and stores a terminal value at it's last node.

### The `match` function

The `match` function searches the trie and returns all terminal values that have exact match.

### The `matchPartial` function

The `matchPartial` function searches for the given sequence in the trie but does not stop at the exact match. Instead it searches in breadth the trie for all possible continuations of it. The result is an object where keys are depth of a node after last matched element and the keys are the nodes for corresponding depths. The search can be limited by depth and by number of elements.
