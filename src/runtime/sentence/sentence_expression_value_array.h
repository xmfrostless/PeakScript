/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak { 

class Value;

class SentenceExpressionValueArray : public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionValueArray);
public:
	SentenceExpressionValueArray(std::vector<std::unique_ptr<SentenceExpression>> valueExpressionArray);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::vector<std::unique_ptr<SentenceExpression>> _expressionArray;
};

} // namespace peak