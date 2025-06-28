#include "sentence_expression_function_call.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"

using namespace peak;

SentenceExpressionFunctionCall::SentenceExpressionFunctionCall(const std::string& name, std::vector<std::unique_ptr<SentenceExpression>> args)
	: _name(name), _args(std::move(args)) {
}

ExecuteResult SentenceExpressionFunctionCall::ExecuteFromInside(std::shared_ptr<Space> objSpace, std::shared_ptr<Space> space) {
	auto variable = objSpace->FindVariable(_name);
	if (!variable) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "Can't found function \"" + _name + "\"!");
		return ExecuteResult::Failed;
	}
	auto value = variable->GetValue();
	if (!ValueTool::IsFunction(value.get())) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "Can't found function \"" + _name + "\"!");
		return ExecuteResult::Failed;
	}
	std::vector<std::shared_ptr<Value>> args;
	for (auto& expression : _args) {
		if (!expression || !IsSuccess(expression->Execute(space))) {
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "The parameters of function \"" + _name + "\" execute failed!");
			return ExecuteResult::Failed;
		}
		auto arg = expression->GetValue();
		args.emplace_back(arg);
	}
	auto result = std::static_pointer_cast<ValueFunction>(value)->Call(args, objSpace);
	if (!result) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionCall, "The function \"" + _name + "\" execute failed!");
		return ExecuteResult::Failed;
	}
	SetValue(result);
	return ExecuteResult::Successed;
}
ExecuteResult SentenceExpressionFunctionCall::Execute(std::shared_ptr<Space> space) {
	return ExecuteFromInside(space, space);
}
const std::string& SentenceExpressionFunctionCall::GetFunctionName() const {
	return _name;
}
