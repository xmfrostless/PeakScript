#pragma once

#include "runtime/module.h"

namespace peak {

class BuiltModuleAlgorithm {
public:
    static std::shared_ptr<Module> CreateModule();
};

}