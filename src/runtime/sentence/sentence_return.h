/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence.h"
#include "sentence_expression.h"

namespace peak { 

class SentenceReturn : public Sentence {
	MAKE_NON_COPYABLE(SentenceReturn);
public:
	SentenceReturn() = default;
	SentenceReturn(std::unique_ptr<SentenceExpression> expression);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

	std::shared_ptr<Value> GetReturnValue() const;

protected:
	void SetReturnValue(std::shared_ptr<Value> value);

private:
	std::unique_ptr<SentenceExpression> _returnExpression{nullptr};
	std::shared_ptr<Value> _returnValue{nullptr};
};

} // namespace peak