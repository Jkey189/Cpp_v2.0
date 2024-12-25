cmake_minimum_required(VERSION 3.29)

project(Language)

set(CMAKE_CXX_STANDARD 20)

add_executable(Language
        main.cpp


        includes/libraries.h


        lexical-analyzer/headers/trie.h
        lexical-analyzer/headers/tokens.h
        lexical-analyzer/headers/lexer.h

        lexical-analyzer/sources/trie.cpp
        lexical-analyzer/sources/tokens.cpp
        lexical-analyzer/sources/lexer.cpp


        syntax-analyzer/headers/parser.h
        syntax-analyzer/sources/parser.cpp


        semantic-analyzer/headers/semantic.h
        semantic-analyzer/sources/semantic.cpp
        semantic-analyzer/TID/tid.cpp
        semantic-analyzer/TID/tid.h
)
