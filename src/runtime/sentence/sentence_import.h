/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak {


class SentenceImport : public Sentence {
public:
	SentenceImport(const std::string& moduleName, const std::string& alias);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _moduleName;
	std::string _alias;
};


} // namespace peak