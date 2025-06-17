#include "builtin_module_arrays.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"

using namespace peak;

std::shared_ptr<Module> BuiltModuleArrays::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    _InsertFunction(space, "len", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get())) {
            return std::make_shared<ValueNumber>(static_cast<double>(std::static_pointer_cast<ValueArray>(args[0])->GetArray().size()));
        }
        return nullptr;
    });
    _InsertFunction(space, "create", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsNumber(args[0].get())) {
            auto arr = std::make_shared<ValueArray>();
            auto size = static_cast<std::size_t>(std::static_pointer_cast<ValueNumber>(args[0])->GetValue());
            arr->Reserve(size);
            for (auto i = 0u; i < size; ++i) {
                arr->EmplaceBack(args[1]);
            }
            return arr;
        }
        return nullptr;
    });
    _InsertFunction(space, "append", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get())) {
            auto ret = std::static_pointer_cast<ValueArray>(args[0]);
            ret->EmplaceBack(args[1]);
            return args[0];
        }
        return nullptr;
    });
    _InsertFunction(space, "insert", 3, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get()) && ValueTool::IsInteger(args[1].get())) {
            auto& ret = std::static_pointer_cast<ValueArray>(args[0])->GetArray();
            auto argIndex = std::static_pointer_cast<ValueNumber>(args[1])->GetValue();
            if (argIndex >= 0) {
                auto index = std::min(static_cast<long long>(argIndex), static_cast<long long>(ret.size()));
                ret.insert(ret.begin() + index, std::make_shared<Variable>("", VariableAttribute::None, args[2]));
                return args[0];
            }
        }
        return nullptr;
    });
    _InsertFunction(space, "remove_from", 3, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get()) && ValueTool::IsInteger(args[1].get()) && ValueTool::IsInteger(args[2].get())) {
            auto& ret = std::static_pointer_cast<ValueArray>(args[0])->GetArray();
            auto index = static_cast<long long>(std::static_pointer_cast<ValueNumber>(args[1])->GetValue());
            if (index >= 0 && index < static_cast<long long>(ret.size())) {
                auto count = static_cast<long long>(std::static_pointer_cast<ValueNumber>(args[2])->GetValue());
                count = std::min(count, static_cast<long long>(ret.size()) - index);
                if (count >= 0) {
                    ret.erase(ret.begin() + index, ret.begin() + index + count);
                    return args[0];
                }
            }
        }
        return nullptr;
    });
    _InsertFunction(space, "remove_at", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get()) && ValueTool::IsInteger(args[1].get())) {
            auto& ret = std::static_pointer_cast<ValueArray>(args[0])->GetArray();
            auto index = static_cast<long long>(std::static_pointer_cast<ValueNumber>(args[1])->GetValue());
            if (index >= 0 && index < static_cast<long long>(ret.size())) {
                ret.erase(ret.begin() + index);
                return args[0];
            }
        }
        return nullptr;
    });
    _InsertFunction(space, "remove", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get())) {
            auto& ret = std::static_pointer_cast<ValueArray>(args[0])->GetArray();
            auto target = args[1];
            auto ite = std::find_if(ret.begin(), ret.end(), [target](std::shared_ptr<peak::Variable> item) {
                return ValueTool::Equal(target.get(), item->GetValue().get());
            });
            if (ite != ret.end()) {
                ret.erase(ite);
            }
            return args[0];
        }
        return nullptr;
    });

    return std::make_shared<Module>(space);
}
