#pragma once

#include "runtime/module.h"

namespace peak {

class BuiltModulePeak {
public:
    static std::shared_ptr<Module> CreateModule();
};

}