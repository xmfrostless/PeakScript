/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceExpression;

class SentenceVariableAssign : public Sentence {
public:
	SentenceVariableAssign(std::shared_ptr<SentenceExpression> variableExpression, std::shared_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<SentenceExpression> _variableExpression{nullptr};
	std::shared_ptr<SentenceExpression> _expression{nullptr};
};

} // namespace peak