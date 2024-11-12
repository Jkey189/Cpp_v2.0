#include "../headers/trie.h"


bool Trie::find(std::string& str) const {
  Word* v = root;

  for (auto ch : str) {
    if (!v->to[ch]) {
      return false;
    }
    v = v->to[ch];
  }

  return true;
}

void Trie::insert(std::string& str) const {
  Word* v = root;

  ++v->termCount;
  for (auto ch : str) {
    if (!v->to[ch]) {
      v->to[ch] = new Word();
    }
    v = v->to[ch];
    ++v->termCount;
  }
  v->isTerm = true;
}
