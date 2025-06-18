/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak { 

class SentenceExpression;
class SentenceFor : public SentenceReturn {
public:
	SentenceFor(std::shared_ptr<Sentence> sentence0,
				std::shared_ptr<SentenceExpression> expression0,
				std::shared_ptr<SentenceExpression> expression1,
				std::shared_ptr<Sentence> content);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::shared_ptr<Sentence> _sentence0{nullptr};
	std::shared_ptr<SentenceExpression> _expression0{nullptr};
	std::shared_ptr<SentenceExpression> _expression1{nullptr};
	std::shared_ptr<Sentence> _content{nullptr};
};

} // namespace peak