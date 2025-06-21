#include "builtin_module_peak.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"
#include "runtime/system.h"
#include "script.h"

using namespace peak;

std::shared_ptr<Module> BuiltModulePeak::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    _InsertFunction(space, "exp", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsString(args[0].get())) {
            return nullptr;
        }
        const auto& text = std::static_pointer_cast<ValueString>(args[0])->GetValue();
        static auto script = Script::Load("");
        return script->ExecuteExpression(text);
    });

    _InsertFunction(space, "execute", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (!ValueTool::IsString(args[0].get())) {
            return nullptr;
        }
        const auto& text = std::static_pointer_cast<ValueString>(args[0])->GetValue();
        auto script = Script::Load(text);
        if (!script) {
            return nullptr;
        }
        if (!script->Execute()) {
            return nullptr;
        }
        return ValueNull::DEFAULT_VALUE;
    });

    return std::make_shared<Module>(space);
}
