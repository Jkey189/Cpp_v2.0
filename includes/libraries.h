#ifndef LIBRARIES_H
#define LIBRARIES_H


#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <fstream>
#include <stdexcept>
#include <map>
#include <cstdint>
#include <cstring>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>
#include <stack>


// const variables
inline int COUNT = 0;
static inline std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());


#endif //LIBRARIES_H
