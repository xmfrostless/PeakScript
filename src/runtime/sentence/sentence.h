/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "runtime/space.h"

namespace peak {

class Value;

enum class ExecuteResult {
	Successed,
	Failed,
	Return,
	Continue,
	Break,
};
class Sentence {
	MAKE_NON_COPYABLE(Sentence);
public:
	Sentence() = default;
	virtual ~Sentence() = default;
	virtual ExecuteResult Execute(std::shared_ptr<Space> space) = 0;

	static bool IsSuccess(ExecuteResult value);
};

} // namespace peak