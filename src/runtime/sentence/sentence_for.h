/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak { 

class SentenceExpression;
class SentenceFor : public SentenceReturn {
	MAKE_NON_COPYABLE(SentenceFor);
public:
	SentenceFor(std::unique_ptr<Sentence> sentence0,
				std::unique_ptr<SentenceExpression> expression0,
				std::unique_ptr<SentenceExpression> expression1,
				std::unique_ptr<Sentence> content);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::unique_ptr<Sentence> _sentence0{nullptr};
	std::unique_ptr<SentenceExpression> _expression0{nullptr};
	std::unique_ptr<SentenceExpression> _expression1{nullptr};
	std::unique_ptr<Sentence> _content{nullptr};
};

} // namespace peak