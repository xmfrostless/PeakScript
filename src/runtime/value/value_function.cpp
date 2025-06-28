#include "value_function.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "value_tool.h"

using namespace peak;

ValueFunction::ValueFunction(std::size_t paramSize, FunctionType func)
	: _params(paramSize), _function(func) {
	for (auto i = 0u; i < paramSize; ++i) {
		std::string key = std::string("%") + std::to_string(i);
		_params[i] = std::make_tuple(key, HashFunction::String(key));
	}
}
ValueFunction::ValueFunction(const std::vector<std::string>& params, FunctionType func)
	: _params(params.size()), _function(func) {
	std::transform(params.begin(), params.end(), _params.begin(), [](const std::string& str) {
		return std::make_tuple(str, HashFunction::String(str));
	});
}

std::shared_ptr<Value> ValueFunction::Call(const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space> space) {
	if (args.size() != _params.size()) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "The number of parameters for the function is incorrect");
		return nullptr;
	}

	auto tempSpace = std::make_shared<Space>(SpaceType::Function, space);
	for (std::size_t i = 0; i < args.size(); ++i) {
		auto tempVariable = std::make_shared<Variable>(std::get<0>(_params[i]), std::get<1>(_params[i]), VariableAttribute::None);
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
		ret += std::get<0>(_params[i]);
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
	auto value = std::make_unique<ValueFunction>();
	value->_function = _function;
	value->_params = _params;
	return value;
}

std::size_t ValueFunction::GetParamSize() const {
	return _params.size();
}
