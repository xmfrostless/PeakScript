/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak {

class SentenceExpression;
class SentenceLoop: public SentenceReturn {
	MAKE_NON_COPYABLE(SentenceLoop);
public:
	SentenceLoop(const std::string& indexParam, std::unique_ptr<SentenceExpression> condition, std::unique_ptr<Sentence> sentence);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _indexParam;
	std::size_t _indexParamHashCode { 0 };
	std::unique_ptr<SentenceExpression> _condition { nullptr };
	std::unique_ptr<Sentence> _sentence { nullptr };
};

} // namespace peak