import bindings from 'bindings'

export interface MatchParameters {
	/**
	 * @brief If the result should include the exact match
	 * 
	 * @default true
	 */
	includeExactMatch:boolean

	/**
	 * @brief Maximal depth after last node in the value being matched
	 * 
	 * @default -1 (all)
	 */
	maxDepth:number

	/**
	 * @brief Maximal number of terminal nodes to be returned
	 * 
	 * @default -1 (all)
	 */
	maxElements:number
}

interface StringObjectTrie {
	new():StringObjectTrie
	insert(trieNodes:string[], terminalNode:object):void
	match(trieNodes:string[]):object[]
	matchPartial(trieNodes:string[], params:MatchParameters):object[][]
}

interface StringStringTrie {
	new():StringStringTrie
	insert(trieNodes:string[], terminalNode:string):void
	match(trieNodes:string[]):string[]
	matchPartial(trieNodes:string[], params:MatchParameters):string[][]
}

const trieNative:{
	StringObjectTrie:StringObjectTrie,
	StringStringTrie:StringStringTrie,
} = bindings('native-trie')

interface Trie <Value, Node> {
	insert(value:Value, node:Node):void
	match(value:Value):Node[]
	matchPartial(value:Value, params?:MatchParameters):Node[][]
}

export class ObjectTrie implements Trie<string, object> {
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

	matchPartial(value:string, params?:MatchParameters):object[][] {
		if (params === undefined) {
			params = {includeExactMatch: false, maxDepth: -1, maxElements: -1}
		}
		return this._impl.matchPartial(value.split(''), params)
	}
}

export class StringTrie implements Trie<string, string> {
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

	matchPartial(value:string, params?:MatchParameters):string[][] {
		if (params === undefined) {
			params = {includeExactMatch: false, maxDepth: -1, maxElements: -1}
		}
		return this._impl.matchPartial(value.split(''), params)
	}
}
