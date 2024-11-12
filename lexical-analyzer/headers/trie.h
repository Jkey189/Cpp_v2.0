#ifndef TRIE_H
#define TRIE_H


#include "../../includes/libraries.h"


class Trie {
  struct Word {
    std::map<char, Word*> to;
    __int64 termCount = 0;
    bool isTerm = false;
  };

public:
  bool find(std::string&) const;
  void insert(std::string&) const;

private:
  __int64 cnt = 0;
  Word* root = new Word();
};


#endif //TRIE_H
