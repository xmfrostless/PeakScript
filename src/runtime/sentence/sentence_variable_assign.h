/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceExpression;

class SentenceVariableAssign : public Sentence {
	MAKE_NON_COPYABLE(SentenceVariableAssign);
public:
	SentenceVariableAssign(std::unique_ptr<SentenceExpression> variableExpression, std::unique_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<SentenceExpression> _variableExpression{nullptr};
	std::unique_ptr<SentenceExpression> _expression{nullptr};
};

} // namespace peak