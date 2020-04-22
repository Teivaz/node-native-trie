import bindings from 'bindings'

interface StringObjectTrie {
	new(): StringObjectTrie
	insert: (trieNodes:string[], terminalNode:object)=>void
	match: (trieNodes:string[])=>object[]
}

interface StringStringTrie {
	new(): StringStringTrie
	insert: (trieNodes:string[], terminalNode:string)=>void
	match: (trieNodes:string[])=>string[]
}

const trieNative:{
	StringObjectTrie:StringObjectTrie,
	StringStringTrie:StringStringTrie,
} = bindings('native-trie')

export class ObjectTrie {
	private _impl:StringObjectTrie

	constructor() {
		this._impl = new trieNative.StringObjectTrie()
	}

	insert(value:string, node:object):void {
		return this._impl.insert(value.split(''), node)
	}

	match(value:string):object[] {
		return this._impl.match(value.split(''))
	}
}

export class StringTrie {
	private _impl:StringStringTrie

	constructor() {
		this._impl = new trieNative.StringStringTrie()
	}

	insert(value:string, node:string):void {
		return this._impl.insert(value.split(''), node)
	}

	match(value:string):string[] {
		return this._impl.match(value.split(''))
	}
}
