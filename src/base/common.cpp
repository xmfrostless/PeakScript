#include "common.h"

using namespace peak;

std::size_t HashFunction::String(const std::string& str) {
    static std::hash<std::string> _HASH_CODE_STRING;
    return _HASH_CODE_STRING(str);
}
