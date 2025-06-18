/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak { 

class Value;

class SentenceExpressionValueArray : public SentenceExpression {
public:
	SentenceExpressionValueArray(const std::vector<std::shared_ptr<SentenceExpression>>& valueExpressionArray);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::vector<std::shared_ptr<SentenceExpression>> _expressionArray;
};

} // namespace peak