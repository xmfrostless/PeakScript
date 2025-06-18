/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceExpression;

class SentenceVariableSet : public Sentence {
public:
	SentenceVariableSet(const std::string& name, std::shared_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::shared_ptr<SentenceExpression> _expression{nullptr};
};

} // namespace peak