#include "sentence_condition.h"
#include "runtime/value/value_tool.h"
#include "sentence_expression.h"

using namespace peak;

SentenceCondition::SentenceCondition(std::unique_ptr<SentenceExpression> expression, std::unique_ptr<Sentence> sentenceTrue, std::unique_ptr<Sentence> sentenceFalse)
	: _expression(std::move(expression)), _sentenceTrue(std::move(sentenceTrue)), _sentenceFalse(std::move(sentenceFalse)) {
}

ExecuteResult SentenceCondition::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_expression->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Condition, "The condition expression execute failed!");
		return ExecuteResult::Failed;
	}
	bool bTrue = ValueTool::ToLogic(_expression->GetValue().get());
	auto& tempSentence = bTrue ? _sentenceTrue : _sentenceFalse;
	if (tempSentence) {
		auto tempSpace = std::make_shared<Space>(SpaceType::Condition, space);
		auto executeRet = tempSentence->Execute(tempSpace);
		if (!IsSuccess(executeRet)) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Condition, bTrue ? "The 'true' code sentence execute failed!" : "The 'false' code sentence execute failed!");
			return ExecuteResult::Failed;
		}
		if (executeRet == ExecuteResult::Return) {
			SetReturnValue(static_cast<SentenceReturn*>(tempSentence.get())->GetReturnValue());
			return ExecuteResult::Return;
		}
		if (executeRet == ExecuteResult::Continue) {
			return ExecuteResult::Continue;
		}
		if (executeRet == ExecuteResult::Break) {
			return ExecuteResult::Break;
		}
		tempSpace->Clear();
	}

	return ExecuteResult::Successed;
}
