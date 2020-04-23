export interface MatchParameters {
    /**
     * @brief If the result should include the exact match
     *
     * @default true
     */
    includeExactMatch: boolean;
    /**
     * @brief Maximal depth after last node in the value being matched
     *
     * @default -1 (all)
     */
    maxDepth: number;
    /**
     * @brief Maximal number of terminal nodes to be returned
     *
     * @default -1 (all)
     */
    maxElements: number;
}
interface Trie<Value, Node> {
    insert(value: Value, node: Node): void;
    match(value: Value): Node[];
    matchPartial(value: Value, params?: MatchParameters): Map<number, Node[]>;
}
export declare class ObjectTrie implements Trie<string, object> {
    private _impl;
    constructor();
    insert(value: string, node: object): void;
    match(value: string): object[];
    matchPartial(value: string, params?: Partial<MatchParameters>): Map<number, object[]>;
}
export declare class StringTrie implements Trie<string, string> {
    private _impl;
    constructor();
    insert(value: string, node: string): void;
    match(value: string): string[];
    matchPartial(value: string, params?: Partial<MatchParameters>): Map<number, string[]>;
}
export {};
