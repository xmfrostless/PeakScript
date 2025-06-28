/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak { 

class SentenceBlock : public SentenceReturn {
	MAKE_NON_COPYABLE(SentenceBlock);
public:
	SentenceBlock() = default;
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
	void Push(std::unique_ptr<Sentence> sentence);

private:
	std::vector<std::unique_ptr<Sentence>> _list;
};

} // namespace peak