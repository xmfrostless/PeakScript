#include "sentence_expression_not.h"
#include "runtime/value/value_tool.h"

using namespace peak;

SentenceExpressionNot::SentenceExpressionNot(std::unique_ptr<SentenceExpression> expression)
	: _expression(std::move(expression)) {
}
ExecuteResult SentenceExpressionNot::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_expression->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ExpressionNot, "The [not-expression] execute failed!");
		return ExecuteResult::Failed;
	}

	auto ret = ValueTool::ToLogic(_expression->GetValue().get());
	SetValue(std::make_shared<ValueBool>(!ret));

	return ExecuteResult::Successed;
}
