/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak {


class SentenceExpressionNot : public SentenceExpression {
public:
	SentenceExpressionNot(std::shared_ptr<SentenceExpression> expression);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::Not; }
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<SentenceExpression> _expression{nullptr};
};


} // namespace peak