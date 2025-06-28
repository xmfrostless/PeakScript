/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak { 

class IValueCalculate;

class SentenceExpressionMath : public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionMath);
public:
	SentenceExpressionMath(std::unique_ptr<SentenceExpression> left, std::unique_ptr<SentenceExpression> right, IValueCalculate* calculate);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::Math; }

protected:
	std::unique_ptr<SentenceExpression> _left{nullptr};
	std::unique_ptr<SentenceExpression> _right{nullptr};
	IValueCalculate* _calculate{nullptr};
};

} // namespace peak