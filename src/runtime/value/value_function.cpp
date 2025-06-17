#include "value_function.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "value_tool.h"

using namespace peak;

ValueFunction::ValueFunction(std::size_t paramSize, FunctionType func)
	: _params(paramSize), _function(func) {
	for (auto i = 0u; i < paramSize; ++i) {
		_params[i] = std::string("%") + std::to_string(i);
	}
}
ValueFunction::ValueFunction(const std::vector<std::string>& params, FunctionType func)
	: _params(params), _function(func) {
}

std::shared_ptr<Value> ValueFunction::Call(const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space> space) {
	if (args.size() != _params.size()) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "The number of parameters for the function is incorrect");
		return nullptr;
	}

	auto tempSpace = std::make_shared<Space>(SpaceType::Function, space);
	for (std::size_t i = 0; i < args.size(); ++i) {
		auto tempVariable = std::make_shared<Variable>(_params[i], VariableAttribute::None);
		if (!tempSpace->AddVariable(tempVariable)) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "The function params name is exist!");
			return nullptr;
		}
		tempVariable->SetValue(args[i]);
	}
	auto result = _function(args, tempSpace);
	tempSpace->Clear();
	return result;
}

std::string ValueFunction::ToString() const {
	std::string ret = "function (";
	for (auto i = 0u; i < _params.size(); ++i) {
		ret += _params[i];
		if (i < _params.size() - 1) {
			ret += ", ";
		}
	}
	ret += ")";
	return ret;
}

std::string ValueFunction::ToRawString() const {
	return ToString();
}

std::shared_ptr<Value> ValueFunction::Clone() const {
	return std::make_unique<ValueFunction>(_params, _function);
}

std::size_t ValueFunction::GetParamSize() const {
	return _params.size();
}
