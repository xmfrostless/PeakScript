/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak {


class SentenceObjectDefine : public Sentence {
public:
	SentenceObjectDefine(const std::string& name, const std::string& parentName, std::list<std::shared_ptr<Sentence>>& sentenceList);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::string _parentName;
	std::list<std::shared_ptr<Sentence>> _sentenceList;
};


} // namespace peak