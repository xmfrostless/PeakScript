/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 

class SentenceTryCatchFinally : public Sentence {
public:
	SentenceTryCatchFinally(std::shared_ptr<Sentence> senTry, std::shared_ptr<Sentence> senCatch, std::shared_ptr<Sentence> senFinally);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<Sentence> _senTry{nullptr};
	std::shared_ptr<Sentence> _senCatch{nullptr};
	std::shared_ptr<Sentence> _senFinally{nullptr};
};

} // namespace peak