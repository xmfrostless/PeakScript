/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceLoopControlContinue : public Sentence {
	MAKE_NON_COPYABLE(SentenceLoopControlContinue);
public:
	SentenceLoopControlContinue() = default;
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
};
class SentenceLoopControlBreak : public Sentence {
	MAKE_NON_COPYABLE(SentenceLoopControlBreak);
public:
	SentenceLoopControlBreak() = default;
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
};

} // namespace peak