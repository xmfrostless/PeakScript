#include "sentence_expression_value_array.h"
#include "runtime/value/value_array.h"

using namespace peak;

SentenceExpressionValueArray::SentenceExpressionValueArray(std::vector<std::unique_ptr<SentenceExpression>> valueExpressionArray)
	: _expressionArray(std::move(valueExpressionArray)) {
}
ExecuteResult SentenceExpressionValueArray::Execute(std::shared_ptr<Space> space) {
	auto valueArray = std::make_shared<ValueArray>();
	if (!_expressionArray.empty()) {
		valueArray->Reserve(_expressionArray.size());
		for (auto i = 0u; i < _expressionArray.size(); ++i) {
			auto& expression = _expressionArray[i];
			if (!IsSuccess(expression->Execute(space))) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Array, "The array execute failed!");
				return ExecuteResult::Failed;
			}
			auto value = expression->GetValue();
			valueArray->EmplaceBack(value);
		}
	}
	SetValue(valueArray);
	return ExecuteResult::Successed;
}