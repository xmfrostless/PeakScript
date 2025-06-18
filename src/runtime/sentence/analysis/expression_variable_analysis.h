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
public:
	ExpressionVariableAnalysisName(const std::string& name);
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space);
	void SetName(const std::string& name);

private:
	std::string _name;
};

class ExpressionVariableAnalysisArrayItem: public IExpressionVariableAnalysis {
public:
	ExpressionVariableAnalysisArrayItem(std::shared_ptr<SentenceExpression> valueExpression, std::vector<std::shared_ptr<SentenceExpression>> indexExpressionVec);
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<SentenceExpression> _valueExpression { nullptr };
	std::vector<std::shared_ptr<SentenceExpression>> _indexExpressionVec { nullptr };
};

class ExpressionVariableAnalysisInside: public IExpressionVariableAnalysis {
public:
	ExpressionVariableAnalysisInside(std::shared_ptr<SentenceExpression> header, std::vector<std::shared_ptr<SentenceExpression>> insides);
	virtual std::shared_ptr<Variable> Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<SentenceExpression> _header { nullptr };
	std::vector<std::shared_ptr<SentenceExpression>> _insides { nullptr };
};

} // namespace peak