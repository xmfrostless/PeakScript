/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"

namespace peak { 


class SentenceExpression;

class SentenceReturn : public Sentence {
public:
	SentenceReturn() = default;
	SentenceReturn(std::shared_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

	std::shared_ptr<Value> GetReturnValue() const;

protected:
	void SetReturnValue(std::shared_ptr<Value> value);

private:
	std::shared_ptr<SentenceExpression> _returnExpression{nullptr};
	std::shared_ptr<Value> _returnValue{nullptr};
};

} // namespace peak