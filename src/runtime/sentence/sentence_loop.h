/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak {

class SentenceExpression;
class SentenceLoop: public SentenceReturn {
public:
	SentenceLoop(const std::string& indexParam, std::shared_ptr<SentenceExpression> condition, std::shared_ptr<Sentence> sentence);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _indexParam;
	std::shared_ptr<SentenceExpression> _condition { nullptr };
	std::shared_ptr<Sentence> _sentence { nullptr };
};

} // namespace peak