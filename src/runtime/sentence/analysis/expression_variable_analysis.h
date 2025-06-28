/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "runtime/sentence/sentence_expression.h"

namespace peak {


class Variable;

class IExpressionVariableAnalysis {
public:
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space) = 0;
	virtual ~IExpressionVariableAnalysis() = default;
};

class ExpressionVariableAnalysisName: public IExpressionVariableAnalysis {
	MAKE_NON_COPYABLE(ExpressionVariableAnalysisName);
public:
	ExpressionVariableAnalysisName(const std::string& name);
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::size_t _hashCode { 0 };
};

class ExpressionVariableAnalysisArrayItem: public IExpressionVariableAnalysis {
	MAKE_NON_COPYABLE(ExpressionVariableAnalysisArrayItem);
public:
	ExpressionVariableAnalysisArrayItem(std::unique_ptr<SentenceExpression> valueExpression, std::vector<std::unique_ptr<SentenceExpression>> indexExpressionVec);
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<SentenceExpression> _valueExpression { nullptr };
	std::vector<std::unique_ptr<SentenceExpression>> _indexExpressionVec;
};

class ExpressionVariableAnalysisInside: public IExpressionVariableAnalysis {
	MAKE_NON_COPYABLE(ExpressionVariableAnalysisInside);
public:
	ExpressionVariableAnalysisInside(std::unique_ptr<SentenceExpression> header, std::vector<std::unique_ptr<SentenceExpression>> insides);
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<SentenceExpression> _header { nullptr };
	std::vector<std::unique_ptr<SentenceExpression>> _insides;
};

} // namespace peak