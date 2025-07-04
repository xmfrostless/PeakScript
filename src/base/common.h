/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <array>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <filesystem>
#include <typeindex>
#include <charconv>
#include <cmath>
#include <float.h>
#include <regex>
#include <sstream>

namespace peak {

#define MAKE_NON_COPYABLE(__CLS__) \
    __CLS__(const __CLS__&) = delete; \
    __CLS__& operator=(const __CLS__&) = delete

struct HashFunction {
    static std::size_t String(const std::string& str);
};

}