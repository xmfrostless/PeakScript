/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak {

class SentenceExpression;
class SentenceDoWhile : public SentenceReturn {
	MAKE_NON_COPYABLE(SentenceDoWhile);
public:
	SentenceDoWhile(std::unique_ptr<SentenceExpression> expression, std::unique_ptr<Sentence> sentence);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<SentenceExpression> _expression{nullptr};
	std::unique_ptr<Sentence> _sentence{nullptr};
};

} // namespace peak