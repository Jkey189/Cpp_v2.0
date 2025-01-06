#ifndef TRIE_H
#define TRIE_H


#include "../../includes/libraries.h"


class Trie {
  struct Word {
    std::map<char, Word*> to;
    int64_t termCount = 0;
    bool isTerm = false;
  };

public:
  bool find(std::string&) const;
  void insert(std::string&) const;

private:
  Word* root = new Word();
};


#endif //TRIE_H
