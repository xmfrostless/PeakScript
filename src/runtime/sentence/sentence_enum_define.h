/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak {


class ValueNumber;

class SentenceEnumDefine: public Sentence {
	MAKE_NON_COPYABLE(SentenceEnumDefine);
public:
	SentenceEnumDefine(const std::string& name, const std::vector<std::pair<std::string, std::shared_ptr<ValueNumber>>>& valueList);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	std::size_t _hashCode { 0 };
	std::vector<std::tuple<std::string, std::size_t, std::shared_ptr<ValueNumber>>> _valueList;
};

} // namespace peak