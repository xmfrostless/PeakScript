/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "sentence_expression_math.h"

namespace peak { 


class IValueCalculate;

class SentenceExpressionSelfAssign : public SentenceExpressionMath {
	MAKE_NON_COPYABLE(SentenceExpressionSelfAssign);
public:
	using SentenceExpressionMath::SentenceExpressionMath;
	SentenceExpressionSelfAssign() = delete;
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);
	virtual ExpressionType GetExpressionType() const { return ExpressionType::SelfAssign; }
};

} // namespace peak