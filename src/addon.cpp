#include <napi.h>
#include "jstrie.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return TrieInit(env, exports);
}

NODE_API_MODULE(trie, InitAll);
