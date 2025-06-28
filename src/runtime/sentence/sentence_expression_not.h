/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak {


class SentenceExpressionNot : public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionNot);
public:
	SentenceExpressionNot(std::unique_ptr<SentenceExpression> expression);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::Not; }
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<SentenceExpression> _expression{nullptr};
};


} // namespace peak