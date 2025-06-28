#include "sentence_expression_double.h"
#include "runtime/value/value_calculate.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"
#include "sentence_expression_variable.h"

using namespace peak;

SentenceExpressionDouble::SentenceExpressionDouble(std::unique_ptr<SentenceExpression> variableExpresison, IValueCalculate* calculate, bool last)
	: _variableExpresison(std::move(variableExpresison)), _calculate(calculate), _bLast(last) {
}

ExecuteResult SentenceExpressionDouble::Execute(std::shared_ptr<Space> space) {
	if (_variableExpresison->GetExpressionType() != ExpressionType::Variable) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionDouble, "The expression isn't a type of variable!");
		return ExecuteResult::Failed;
	}
	if (!IsSuccess(_variableExpresison->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionDouble, "The expression execute failed!");
		return ExecuteResult::Failed;
	}
	auto variable = static_cast<SentenceExpressionVariable*>(_variableExpresison.get())->GetVariable();
	auto value = _variableExpresison->GetValue();
	if (!ValueTool::IsNumber(value.get())) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionDouble, "The value of variable isn't a type of 'number'!");
		return ExecuteResult::Failed;
	}
	ValueNumber vn(1);
	auto ret = _calculate->Calculate(value.get(), &vn);
	if (!ret) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionDouble, "The calculate is failed!");
		return ExecuteResult::Failed;
	}
	if (!variable->SetValue(ret)) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionDouble, "The variable set value failed!");
		return ExecuteResult::Failed;
	}
	SetValue(_bLast ? value : ret);
	return ExecuteResult::Successed;
}