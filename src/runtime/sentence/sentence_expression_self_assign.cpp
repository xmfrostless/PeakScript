#include "sentence_expression_self_assign.h"
#include "runtime/value/value_calculate.h"
#include "runtime/variable.h"
#include "sentence_expression_variable.h"

using namespace peak;
ExecuteResult SentenceExpressionSelfAssign::Execute(std::shared_ptr<Space> space) {
	if (_left->GetExpressionType() != ExpressionType::Variable) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::SelfAssign, "The expression target isn't a variable!");
		return ExecuteResult::Failed;
	}

	if (!IsSuccess(SentenceExpressionMath::Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::SelfAssign, "The [self-assign-expression] execute failed!");
		return ExecuteResult::Failed;
	}

	auto temp = static_cast<SentenceExpressionVariable*>(_left.get());
	auto variable = temp->GetVariable();
	if (!variable) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::SelfAssign, "The [self-assign-expression] execute failed!");
		return ExecuteResult::Failed;
	}

	if (!variable->SetValue(GetValue())) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::SelfAssign, "The [self-assign-expression] execute failed!");
		return ExecuteResult::Failed;
	}

	return ExecuteResult::Successed;
}