#pragma once

#include "trie.h"
#include <napi.h>
#include <string>

Napi::Object TrieInit(Napi::Env env, Napi::Object exports);

template<typename T>
class WrappedTrie : public Napi::ObjectWrap<WrappedTrie<T>> {
public:
	static Napi::Object Init(Napi::Env env, char const* name, Napi::Object exports);
	
	WrappedTrie(const Napi::CallbackInfo& info) 
	: Napi::ObjectWrap<WrappedTrie>(info) {}
	
private:
	static Napi::FunctionReference constructor;

	Napi::Value Insert(const Napi::CallbackInfo& info) {
		return info.Env().Undefined();
	}

	Napi::Value Match(const Napi::CallbackInfo& info);

private:
	Trie<T> m_trie;
};
