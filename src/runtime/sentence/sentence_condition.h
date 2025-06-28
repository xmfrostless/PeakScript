/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak { 

class SentenceExpression;
class SentenceCondition : public SentenceReturn {
	MAKE_NON_COPYABLE(SentenceCondition);
public:
	SentenceCondition(std::unique_ptr<SentenceExpression> expression, std::unique_ptr<Sentence> sentenceTrue, std::unique_ptr<Sentence> sentenceFalse);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<SentenceExpression> _expression{nullptr};
	std::unique_ptr<Sentence> _sentenceTrue{nullptr};
	std::unique_ptr<Sentence> _sentenceFalse{nullptr};
};

} // namespace peak