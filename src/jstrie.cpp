#include "jstrie.h"

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
		WrappedTrie<T>::InstanceMethod("match", &WrappedTrie<T>::Match)
	});

  WrappedTrie<T>::constructor = Napi::Persistent(func);
  WrappedTrie<T>::constructor.SuppressDestruct();

  exports.Set(name, func);
	return exports;
}

template<>
Napi::Value WrappedTrie<TrieStringObjectCore>::Insert(const Napi::CallbackInfo& info) {
	auto a = info[0].As<Napi::Array>();

	uint32_t length = a.Length();
	std::vector<std::string> sequence(length);
	for (uint32_t i = 0; i < length; i++) {
		sequence.push_back(a.Get(i).As<Napi::String>());
	}

	if (info[1].IsObject()) {
		m_trie.insert(sequence, Napi::Persistent(info[1]));
	}
	else {
		//insert(sequence, Napi::Persistent(Napi::Object(info.Env(), info[1])));
	}

	return info.Env().Undefined();
}

template<>
Napi::Value WrappedTrie<TrieStringObjectCore>::Match(const Napi::CallbackInfo& info) {
	auto a = info[0].As<Napi::Array>();

	uint32_t length = a.Length();
	std::vector<std::string> sequence(length);
	for (uint32_t i = 0; i < length; i++) {
		sequence.push_back(a.Get(i).As<Napi::String>());
	}
	const auto matchedNodes = m_trie.matchRef(sequence);

	auto result = Napi::Array::New(info.Env(), matchedNodes.size());
	for (uint32_t i = 0; i < matchedNodes.size(); i++) {
		result.Set(i, matchedNodes[i]->Value());
	}
	
	return result;
}


template<>
Napi::Value WrappedTrie<TrieStringStringCore>::Insert(const Napi::CallbackInfo& info) {
	auto a = info[0].As<Napi::Array>();

	uint32_t length = a.Length();
	std::vector<std::string> sequence(length);
	for (uint32_t i = 0; i < length; i++) {
		sequence.push_back(a.Get(i).As<Napi::String>());
	}

	m_trie.insert(sequence, info[1].As<Napi::String>());

	return info.Env().Undefined();
}

template<>
Napi::Value WrappedTrie<TrieStringStringCore>::Match(const Napi::CallbackInfo& info) {
	auto a = info[0].As<Napi::Array>();

	uint32_t length = a.Length();
	std::vector<std::string> sequence(length);
	for (uint32_t i = 0; i < length; i++) {
		sequence.push_back(a.Get(i).As<Napi::String>());
	}
	const auto matchedNodes = m_trie.matchRef(sequence);

	auto result = Napi::Array::New(info.Env(), matchedNodes.size());
	for (uint32_t i = 0; i < matchedNodes.size(); i++) {
		result.Set(i, *matchedNodes[i]);
	}
	
	return result;
}
