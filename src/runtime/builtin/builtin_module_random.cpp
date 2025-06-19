#include "builtin_module_random.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"
#include "runtime/system.h"

using namespace peak;

std::shared_ptr<Module> BuiltModuleRandom::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    srand(static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()));

    _InsertFunction(space, "rand_int", 0, [](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        return std::make_shared<ValueNumber>(rand());
    });

    _InsertFunction(space, "range_int", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        if (ValueTool::IsInteger(args[0].get()) && ValueTool::IsInteger(args[1].get())) {
            auto left = static_cast<long long>(std::static_pointer_cast<ValueNumber>(args[0])->GetValue());
            auto right = static_cast<long long>(std::static_pointer_cast<ValueNumber>(args[1])->GetValue());
            if (left >= right) {
                return nullptr;
            }
            return std::make_shared<ValueNumber>(static_cast<double>(left + rand() % (right - left)));
        }
        return nullptr;
    });

    return std::make_shared<Module>(space);
}
