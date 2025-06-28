#include "sentence_return.h"
#include "runtime/value/value_tool.h"

using namespace peak;

SentenceReturn::SentenceReturn(std::unique_ptr<SentenceExpression> expression)
	: _returnExpression(std::move(expression)) {
}
ExecuteResult SentenceReturn::Execute(std::shared_ptr<Space> space) {
	if (_returnExpression) {
		if (!IsSuccess(_returnExpression->Execute(space))) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Return, "The expression execute failed!");
			return ExecuteResult::Failed;
		}
		SetReturnValue(_returnExpression->GetValue());
	} else {
		SetReturnValue(ValueNull::DEFAULT_VALUE);
	}
	return ExecuteResult::Return;
}

std::shared_ptr<Value> SentenceReturn::GetReturnValue() const {
	return _returnValue;
}

void SentenceReturn::SetReturnValue(std::shared_ptr<Value> value) {
	_returnValue = value;
}
