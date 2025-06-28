/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak { 


class SentenceExpressionVariable;
class IValueCalculate;

class SentenceExpressionDouble : public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionDouble);
public:
	SentenceExpressionDouble(std::unique_ptr<SentenceExpression> variableExpresison, IValueCalculate* calculate, bool last);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

	virtual ExpressionType GetExpressionType() const { return ExpressionType::DoubleSymbol; }

private:
	std::unique_ptr<SentenceExpression> _variableExpresison{nullptr};
	IValueCalculate* _calculate{nullptr};
	bool _bLast{false};
};

} // namespace peak