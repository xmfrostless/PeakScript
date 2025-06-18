/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak { 

class SentenceExpression;
class SentenceCondition : public SentenceReturn {
public:
	SentenceCondition(std::shared_ptr<SentenceExpression> expression, std::shared_ptr<Sentence> sentenceTrue, std::shared_ptr<Sentence> sentenceFalse);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<SentenceExpression> _expression{nullptr};
	std::shared_ptr<Sentence> _sentenceTrue{nullptr};
	std::shared_ptr<Sentence> _sentenceFalse{nullptr};
};

} // namespace peak