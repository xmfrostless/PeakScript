/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceExpression;
class SentenceEcho : public Sentence {
	MAKE_NON_COPYABLE(SentenceEcho);
public:
	SentenceEcho(std::unique_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
private:
	std::unique_ptr<SentenceExpression> _expression{nullptr};
};

} // namespace peak