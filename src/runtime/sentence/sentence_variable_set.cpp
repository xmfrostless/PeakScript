#include "sentence_variable_set.h"
#include "runtime/variable.h"
#include "sentence_expression.h"

using namespace peak;

SentenceVariableSet::SentenceVariableSet(const std::string& name, std::unique_ptr<SentenceExpression> expression)
	: _name(name), _expression(std::move(expression)) {
	_hashCode = HashFunction::String(_name);
}

ExecuteResult SentenceVariableSet::Execute(std::shared_ptr<Space> space) {
	auto variable = space->FindVariable(_hashCode);
	if (!variable) {
		variable = std::make_shared<Variable>(_name, _hashCode, VariableAttribute::None);
		space->AddVariable(variable);
	}
	if (_expression) {
		if (!IsSuccess(_expression->Execute(space))) {
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableSet, "The \"" + _name + "\" expression execute failed!");
			return ExecuteResult::Failed;
		}
		if (!variable->SetValue(_expression->GetValue())) {
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableSet, "The \"" + _name + "\" expression execute failed!");
			return ExecuteResult::Failed;
		}
	}
	return ExecuteResult::Successed;
}
