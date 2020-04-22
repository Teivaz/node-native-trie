"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const bindings_1 = __importDefault(require("bindings"));
const trieNative = bindings_1.default('native-trie');
class ObjectTrie {
    constructor() {
        this._impl = new trieNative.StringObjectTrie();
    }
    insert(value, node) {
        return this._impl.insert(value.split(''), node);
    }
    match(value) {
        return this._impl.match(value.split(''));
    }
}
exports.ObjectTrie = ObjectTrie;
class StringTrie {
    constructor() {
        this._impl = new trieNative.StringStringTrie();
    }
    insert(value, node) {
        return this._impl.insert(value.split(''), node);
    }
    match(value) {
        return this._impl.match(value.split(''));
    }
}
exports.StringTrie = StringTrie;
