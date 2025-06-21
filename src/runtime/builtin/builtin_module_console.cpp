#include "builtin_module_console.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"
#include "runtime/system.h"

using namespace peak;

std::shared_ptr<Module> BuiltModuleConsole::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    _InsertFunction(space, "clear", 0, [](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
#if defined(_WIN32)
        system("cls");
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
        system("clear");
#pragma GCC diagnostic pop
#endif
        return ValueNull::DEFAULT_VALUE;
    });
    _InsertFunction(space, "input", 0, [](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        std::string ret;
        std::getline(std::cin, ret);
        return std::make_shared<ValueString>(ret);
    });
    _InsertFunction(space, "log", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        System::Echo(args[0]->ToString());
        return ValueNull::DEFAULT_VALUE;
    });

    return std::make_shared<Module>(space);
}
