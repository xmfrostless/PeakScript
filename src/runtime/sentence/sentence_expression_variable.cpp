#include "sentence_expression_variable.h"
#include "runtime/variable.h"
#include "analysis/expression_variable_analysis.h"

using namespace peak;

SentenceExpressionVariable::SentenceExpressionVariable(std::unique_ptr<IExpressionVariableAnalysis> variableAnalysis)
	: _analysis(std::move(variableAnalysis)) {
}

ExecuteResult SentenceExpressionVariable::Execute(std::shared_ptr<Space> space) {
	_variable = _analysis->Execute(space);
	if (!_variable) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionVariable, "The variable-expression execute failed!");
		return ExecuteResult::Failed;
	}
	SetValue(_variable->GetValue());
	return ExecuteResult::Successed;
}

std::shared_ptr<Variable> SentenceExpressionVariable::GetVariable() const {
	return _variable;
}
