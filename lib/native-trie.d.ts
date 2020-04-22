export declare class ObjectTrie {
    private _impl;
    constructor();
    insert(value: string, node: object): void;
    match(value: string): object[];
}
export declare class StringTrie {
    private _impl;
    constructor();
    insert(value: string, node: string): void;
    match(value: string): string[];
}
