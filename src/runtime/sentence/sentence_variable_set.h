/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceExpression;

class SentenceVariableSet : public Sentence {
	MAKE_NON_COPYABLE(SentenceVariableSet);
public:
	SentenceVariableSet(const std::string& name, std::unique_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::unique_ptr<SentenceExpression> _expression{nullptr};
};

} // namespace peak