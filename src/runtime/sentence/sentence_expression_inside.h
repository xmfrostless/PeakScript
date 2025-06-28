/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak {


class SentenceExpressionInside : public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionInside);
public:
	SentenceExpressionInside(std::unique_ptr<SentenceExpression> header, std::vector<std::unique_ptr<SentenceExpression>> insides);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::Inside; }

private:
	std::unique_ptr<SentenceExpression> _header{nullptr};
	std::vector<std::unique_ptr<SentenceExpression>> _insides;
};


} // namespace peak