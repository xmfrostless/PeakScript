/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "runtime/variable.h"
#include "sentence.h"

namespace peak { 

class SentenceExpression;

class SentenceVariableDefine : public Sentence {
	MAKE_NON_COPYABLE(SentenceVariableDefine);
public:
	SentenceVariableDefine(const std::string& name, VariableAttribute attribute, std::unique_ptr<SentenceExpression> expression = nullptr);
	virtual ExecuteResult Execute(std::shared_ptr<Space> space);

private:
	std::string _name;
	VariableAttribute _attribute{VariableAttribute::None};
	std::shared_ptr<Variable> _variable{nullptr};
	std::unique_ptr<SentenceExpression> _expression{nullptr};
};

} // namespace peak