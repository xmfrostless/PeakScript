/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak {


class Variable;

class SentenceFunctionDefine: public Sentence {
	MAKE_NON_COPYABLE(SentenceFunctionDefine);
public:
	SentenceFunctionDefine(const std::string& name, const std::vector<std::string>& params, std::unique_ptr<Sentence> content);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::size_t _hashCode { 0 };
	std::vector<std::string> _params;
	std::unique_ptr<Sentence> _content { nullptr };
};

} // namespace peak