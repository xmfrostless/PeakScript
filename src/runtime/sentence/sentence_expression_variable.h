/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression.h"

namespace peak { 

class Variable;
class IExpressionVariableAnalysis;

class SentenceExpressionVariable : public SentenceExpression {
	MAKE_NON_COPYABLE(SentenceExpressionVariable);
public:
	SentenceExpressionVariable(std::unique_ptr<IExpressionVariableAnalysis> variableAnalysis);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::Variable; }

	std::shared_ptr<Variable> GetVariable() const;

private:
	std::unique_ptr<IExpressionVariableAnalysis> _analysis{nullptr};
	std::shared_ptr<Variable> _variable{nullptr};
};

} // namespace peak