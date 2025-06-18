/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak {


class SentenceExpressionInside : public SentenceExpression {
public:
	SentenceExpressionInside(std::shared_ptr<SentenceExpression> header, std::vector<std::shared_ptr<SentenceExpression>> insides);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::Inside; }

private:
	std::shared_ptr<SentenceExpression> _header{nullptr};
	std::vector<std::shared_ptr<SentenceExpression>> _insides{nullptr};
};


} // namespace peak