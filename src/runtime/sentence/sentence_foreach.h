/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_return.h"

namespace peak { 

class SentenceExpression;

class SentenceForeach : public SentenceReturn {
public:
	SentenceForeach(const std::string& name, const std::string& indexParam, std::shared_ptr<SentenceExpression> expression, std::shared_ptr<Sentence> sentence);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::string _indexParam;
	std::shared_ptr<SentenceExpression> _expression{nullptr};
	std::shared_ptr<Sentence> _sentence{nullptr};
};

} // namespace peak