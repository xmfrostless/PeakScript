#include "builtin_module_time.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"

using namespace peak;

std::shared_ptr<Module> BuiltModuleTime::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    _InsertFunction(space, "get_tick", 0, [](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto tick = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        return std::make_shared<ValueNumber>(static_cast<double>(tick));
    });
    _InsertFunction(space, "get_utc_time", 0, [](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm;

#if defined(_MSC_VER) || defined(_WIN32)
        if (gmtime_s(&now_tm, &now_time) != 0) {
            return nullptr;
        }
#else
        if (!gmtime_r(&now_time, &now_tm)) {
            return nullptr;
        }
#endif
        auto year = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_year + 1900));
        auto month = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_mon + 1));
        auto day = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_mday));
        auto hour = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_hour));
        auto minute = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_min));
        auto second = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_sec));

        auto ret = std::make_shared<ValueObject>();
        auto retSpace = ret->GetSpace();
        retSpace->AddVariable(std::make_shared<Variable>("year", VariableAttribute::None, year));
        retSpace->AddVariable(std::make_shared<Variable>("month", VariableAttribute::None, month));
        retSpace->AddVariable(std::make_shared<Variable>("day", VariableAttribute::None, day));

        retSpace->AddVariable(std::make_shared<Variable>("hour", VariableAttribute::None, hour));
        retSpace->AddVariable(std::make_shared<Variable>("minute", VariableAttribute::None, minute));
        retSpace->AddVariable(std::make_shared<Variable>("second", VariableAttribute::None, second));
        return ret;
    });
    _InsertFunction(space, "get_local_time", 0, [](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm;

#if defined(_MSC_VER) || defined(_WIN32)
        if (localtime_s(&now_tm, &now_time) != 0) {
            return nullptr;
        }
#else
        if (!localtime_r(&now_time, &now_tm)) {
            return nullptr;
        }
#endif
        auto year = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_year + 1900));
        auto month = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_mon + 1));
        auto day = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_mday));
        auto hour = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_hour));
        auto minute = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_min));
        auto second = std::make_shared<ValueNumber>(static_cast<double>(now_tm.tm_sec));

        auto ret = std::make_shared<ValueObject>();
        auto retSpace = ret->GetSpace();
        retSpace->AddVariable(std::make_shared<Variable>("year", VariableAttribute::None, year));
        retSpace->AddVariable(std::make_shared<Variable>("month", VariableAttribute::None, month));
        retSpace->AddVariable(std::make_shared<Variable>("day", VariableAttribute::None, day));

        retSpace->AddVariable(std::make_shared<Variable>("hour", VariableAttribute::None, hour));
        retSpace->AddVariable(std::make_shared<Variable>("minute", VariableAttribute::None, minute));
        retSpace->AddVariable(std::make_shared<Variable>("second", VariableAttribute::None, second));
        return ret;
    });

    return std::make_shared<Module>(space);
}
