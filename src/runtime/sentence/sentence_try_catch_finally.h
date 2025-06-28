/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceTryCatchFinally : public Sentence {
	MAKE_NON_COPYABLE(SentenceTryCatchFinally);
public:
	SentenceTryCatchFinally(std::unique_ptr<Sentence> senTry, std::unique_ptr<Sentence> senCatch, std::unique_ptr<Sentence> senFinally);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<Sentence> _senTry{nullptr};
	std::unique_ptr<Sentence> _senCatch{nullptr};
	std::unique_ptr<Sentence> _senFinally{nullptr};
};

} // namespace peak