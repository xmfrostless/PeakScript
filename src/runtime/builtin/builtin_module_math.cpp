#include "builtin_module_math.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"
#include "runtime/system.h"
#include <cmath>

using namespace peak;

std::shared_ptr<Module> BuiltModuleMath::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    _InsertFunction(space, "sqrt", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get())) {
            return nullptr;
        }
        const auto& value = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        if (value < 0) {
            return nullptr;
        }
        return std::make_shared<ValueNumber>(std::sqrt(value));
    });

    _InsertFunction(space, "min", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get()) || !ValueTool::IsNumber(args[1].get())) {
            return nullptr;
        }
        const auto& a = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        const auto& b = std::static_pointer_cast<ValueNumber>(args[1])->GetValue();
        return std::make_shared<ValueNumber>(std::min(a, b));
    });

    _InsertFunction(space, "max", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get()) || !ValueTool::IsNumber(args[1].get())) {
            return nullptr;
        }
        const auto& a = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        const auto& b = std::static_pointer_cast<ValueNumber>(args[1])->GetValue();
        return std::make_shared<ValueNumber>(std::max(a, b));
    });

    _InsertFunction(space, "clamp", 3, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get()) ||
            !ValueTool::IsNumber(args[1].get()) ||
            !ValueTool::IsNumber(args[2].get())) {
            return nullptr;
        }
        const auto& v = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        const auto& a = std::static_pointer_cast<ValueNumber>(args[1])->GetValue();
        const auto& b = std::static_pointer_cast<ValueNumber>(args[2])->GetValue();
        return std::make_shared<ValueNumber>(std::clamp(v, a, b));
    });

    _InsertFunction(space, "trunc", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get())) {
            return nullptr;
        }
        const auto& num = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        return std::make_shared<ValueNumber>(std::trunc(num));
    });

    _InsertFunction(space, "floor", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get())) {
            return nullptr;
        }
        const auto& num = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        return std::make_shared<ValueNumber>(std::floor(num));
    });

    _InsertFunction(space, "ceil", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get())) {
            return nullptr;
        }
        const auto& num = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        return std::make_shared<ValueNumber>(std::ceil(num));
    });

    _InsertFunction(space, "round", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get())) {
            return nullptr;
        }
        const auto& num = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        return std::make_shared<ValueNumber>(std::round(num));
    });
    
    _InsertFunction(space, "abs", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsNumber(args[0].get())) {
            return nullptr;
        }
        const auto& num = std::static_pointer_cast<ValueNumber>(args[0])->GetValue();
        return std::make_shared<ValueNumber>(std::abs(num));
    });

    return std::make_shared<Module>(space);
}
