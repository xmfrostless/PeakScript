/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak {

class SentenceExpressionNew: public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionNew);
public:
	SentenceExpressionNew(const std::string& name);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::New; }
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::size_t _hashCode { 0 };
};


} // namespace peak