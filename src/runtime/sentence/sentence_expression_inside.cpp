#include "sentence_expression_inside.h"
#include "runtime/value/value_tool.h"
#include "sentence_expression_variable.h"
#include "sentence_expression_function_call.h"

using namespace peak;

SentenceExpressionInside::SentenceExpressionInside(std::unique_ptr<SentenceExpression> header, std::vector<std::unique_ptr<SentenceExpression>> insides)
	: _header(std::move(header)), _insides(std::move(insides)) {
}
ExecuteResult SentenceExpressionInside::Execute(std::shared_ptr<Space> space) {
	if (!Sentence::IsSuccess(_header->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Inside, "The header expression execute failed!");
		return ExecuteResult::Failed;
	}
	auto headerValue = _header->GetValue();
	if (!ValueTool::IsObject(headerValue.get())) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Inside, "The header expression isn't a object!");
		return ExecuteResult::Failed;
	}
	auto tempValue = headerValue;
	for (auto& expression : _insides) {
		if (!ValueTool::IsObject(tempValue.get())) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Inside, "The expression isn't a object!");
			return ExecuteResult::Failed;
		}
		auto objSpace = std::static_pointer_cast<ValueObject>(tempValue)->GetSpace();
		auto expType = expression->GetExpressionType();
		auto executeRet = ExecuteResult::Failed;
		if (expType == ExpressionType::Function) {
			executeRet = static_cast<SentenceExpressionFunctionCall*>(expression.get())->ExecuteFromInside(objSpace, space);
		} else {
			executeRet = expression->Execute(objSpace);
		}
		if (!Sentence::IsSuccess(executeRet)) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Inside, "The inside expression execute failed!");
			return ExecuteResult::Failed;
		}
		tempValue = expression->GetValue();
	}
	SetValue(tempValue);
	return ExecuteResult::Successed;
}