/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak {


class ValueNumber;
	
class SentenceEnumDefine : public Sentence {
public:
	SentenceEnumDefine(const std::string& name, const std::list<std::pair<std::string, std::shared_ptr<ValueNumber>>>& valueList);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::list<std::pair<std::string, std::shared_ptr<ValueNumber>>> _valueList;
};

} // namespace peak