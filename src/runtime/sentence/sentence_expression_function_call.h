/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak {


class SentenceExpressionFunctionCall: public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionFunctionCall);
public:
	SentenceExpressionFunctionCall(const std::string& name, std::vector<std::unique_ptr<SentenceExpression>> args);

	ExecuteResult ExecuteFromInside(std::shared_ptr<Space> objSpace, std::shared_ptr<Space> space);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

	virtual ExpressionType GetExpressionType() const { return ExpressionType::Function; }

	const std::string& GetFunctionName() const;

private:
	std::string _name;
	std::size_t _hashCode { 0 };
	std::vector<std::unique_ptr<SentenceExpression>> _args;
};

} // namespace peak