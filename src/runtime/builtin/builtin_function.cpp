#include "builtin_function.h"
#include "runtime/space.h"
#include "runtime/system.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"

using namespace peak;

BuiltInFunction* BuiltInFunction::GetInstance() {
	static BuiltInFunction _ins;
	return &_ins;
}

std::shared_ptr<Variable> BuiltInFunction::FindVariable(const std::string& name) const {
	return FindVariable(HashFunction::String(name));
}

std::shared_ptr<Variable> BuiltInFunction::FindVariable(std::size_t hashCode) const {
	auto ite = _variables.find(hashCode);
	if (ite != _variables.end()) {
		return ite->second;
	}
	return nullptr;
}

BuiltInFunction::BuiltInFunction() {
	auto _Emplace = [this](const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
		auto value = std::make_shared<ValueFunction>(paramSize, func);
		auto variable = std::make_shared<Variable>(name, VariableAttribute::Const, value);
		_variables.emplace(variable->GetHashCode(), variable);
	};

	// print
	_Emplace("print", 1, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space>) -> std::shared_ptr<Value> {
		System::Echo(ValueTool::ToString(args[0].get()));
		return ValueNull::DEFAULT_VALUE;
	});
}

