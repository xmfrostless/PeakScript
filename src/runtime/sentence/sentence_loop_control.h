/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceLoopControlContinue : public Sentence {
public:
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
};
class SentenceLoopControlBreak : public Sentence {
public:
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
};

} // namespace peak