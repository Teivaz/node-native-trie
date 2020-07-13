#include "jstrie.h"
#include <string>
#include <vector>

struct TrieStringObjectCore {
	using Element = std::string;
	using Container = std::vector<std::string>;
	using TerminalNode = Napi::Reference<Napi::Value>;
};

struct TrieStringStringCore {
	using Element = std::string;
	using Container = std::vector<std::string>;
	using TerminalNode = std::string;
};

template<typename T>
Napi::FunctionReference WrappedTrie<T>::constructor;

Napi::Object TrieInit(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
	WrappedTrie<TrieStringObjectCore>::Init(env, "StringObjectTrie", exports);
	WrappedTrie<TrieStringStringCore>::Init(env, "StringStringTrie", exports);
  return exports;
}

template<typename T>
Napi::Object WrappedTrie<T>::Init(Napi::Env env, char const* name, Napi::Object exports) {
  Napi::HandleScope scope(env);
  Napi::Function func = WrappedTrie<T>::DefineClass(env, name, {
		WrappedTrie<T>::InstanceMethod("insert", &WrappedTrie<T>::Insert),
		WrappedTrie<T>::InstanceMethod("match", &WrappedTrie<T>::Match),
		WrappedTrie<T>::InstanceMethod("matchPartial", &WrappedTrie<T>::MatchPatrial)
	});

  WrappedTrie<T>::constructor = Napi::Persistent(func);
  WrappedTrie<T>::constructor.SuppressDestruct();

  exports.Set(name, func);
	return exports;
}

std::vector<std::string> stringVectorFromArray(Napi::Array const& input) {
	uint32_t const length = input.Length();
	std::vector<std::string> result;
	result.reserve(length);
	for (uint32_t i = 0; i < length; i++) {
		result.push_back(input.Get(i).As<Napi::String>());
	}
	return result;
}

template<typename T>
Napi::Array terminalNodesToArray(Napi::Env env, std::vector<Napi::Reference<T> const*> const& nodes) {
	auto result = Napi::Array::New(env, nodes.size());
	for (uint32_t i = 0; i < nodes.size(); i++) {
		result.Set(i, nodes[i]->Value());
	}
	return result;
}

template<typename T>
Napi::Array terminalNodesToArray(Napi::Env env, std::vector<T const*> const& nodes) {
	auto result = Napi::Array::New(env, nodes.size());
	for (uint32_t i = 0; i < nodes.size(); i++) {
		result.Set(i, *nodes[i]);
	}
	return result;
}

template<>
Napi::Value WrappedTrie<TrieStringObjectCore>::Insert(const Napi::CallbackInfo& info) {
	auto const sequence = stringVectorFromArray(info[0].As<Napi::Array>());
	if (info[1].IsObject()) {
		m_trie.insert(sequence, Napi::Persistent(info[1]));
	}
	else {
		//insert(sequence, Napi::Persistent(Napi::Object(info.Env(), info[1])));
	}
	return info.Env().Undefined();
}

template<>
Napi::Value WrappedTrie<TrieStringStringCore>::Insert(const Napi::CallbackInfo& info) {
	auto const sequence = stringVectorFromArray(info[0].As<Napi::Array>());
	m_trie.insert(sequence, info[1].As<Napi::String>());
	return info.Env().Undefined();
}

template<typename T>
Napi::Value WrappedTrie<T>::Match(const Napi::CallbackInfo& info) {
	auto const sequence = stringVectorFromArray(info[0].As<Napi::Array>());
	const auto matchedNodes = m_trie.match(sequence);
	return terminalNodesToArray(info.Env(), matchedNodes);
}

template<>
Napi::Value WrappedTrie<TrieStringStringCore>::MatchPatrial(const Napi::CallbackInfo& info) {
	auto const sequence = stringVectorFromArray(info[0].As<Napi::Array>());
	TrieMatchParams params;
	if (info.Length() > 1 && info[1].IsObject()) {
		auto const paramsObject = info[1].As<Napi::Object>();
		if (paramsObject.Has("includeExactMatch")) {
			params.includeExactMatch = paramsObject.Get("includeExactMatch").ToBoolean();
		}
		if (paramsObject.Has("maxDepth")) {
			params.maxDepth = (std::size_t)paramsObject.Get("maxDepth").ToNumber().Int32Value();
		}
		if (paramsObject.Has("maxElements")) {
			params.maxElements = (std::size_t)paramsObject.Get("maxElements").ToNumber().Int32Value();
		}
	}
	auto const matchedNodes = m_trie.match(sequence, params);

	auto result = Napi::Object::New(info.Env());
	for (auto pair : matchedNodes) {
		result.Set((uint32_t)pair.first, terminalNodesToArray(info.Env(), pair.second));
	}

	return result;
}
