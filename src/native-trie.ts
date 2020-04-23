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

interface NativeTrie <Value, Node> {
	new():NativeTrie<Value, Node>
	insert(trieNodes:Value[], terminalNode:Node):void
	match(trieNodes:Value[]):Node[]
	matchPartial(trieNodes:Value[], params?:Partial<MatchParameters>):Map<number, Node[]>
}

const trieNative:{
	StringObjectTrie:NativeTrie<string, object>,
	StringStringTrie:NativeTrie<string, string>,
} = bindings('native-trie')

interface Trie <Value, Node> {
	insert(value:Value, node:Node):void
	match(value:Value):Node[]
	matchPartial(value:Value, params?:MatchParameters):Map<number, Node[]>
}

export class ObjectTrie implements Trie<string, object> {
	private _impl:NativeTrie<string, object>

	constructor() {
		this._impl = new trieNative.StringObjectTrie()
	}

	insert(value:string, node:object):void {
		return this._impl.insert(value.split(''), node)
	}

	match(value:string):object[] {
		return this._impl.match(value.split(''))
	}

	matchPartial(value:string, params?:Partial<MatchParameters>):Map<number, object[]> {
		return this._impl.matchPartial(value.split(''), params)
	}
}

export class StringTrie implements Trie<string, string> {
	private _impl:NativeTrie<string, string>

	constructor() {
		this._impl = new trieNative.StringStringTrie()
	}

	insert(value:string, node:string):void {
		return this._impl.insert(value.split(''), node)
	}

	match(value:string):string[] {
		return this._impl.match(value.split(''))
	}

	matchPartial(value:string, params?:Partial<MatchParameters>):Map<number, string[]> {
		return this._impl.matchPartial(value.split(''), params)
	}
}
