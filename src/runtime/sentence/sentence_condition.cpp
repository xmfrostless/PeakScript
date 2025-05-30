#include "sentence_condition.h"
#include "runtime/value/value_tool.h"
#include "sentence_expression.h"

using namespace peak;

SentenceCondition::SentenceCondition(std::shared_ptr<SentenceExpression> expression, std::shared_ptr<Sentence> sentenceTrue, std::shared_ptr<Sentence> sentenceFalse)
	: _expression(expression), _sentenceTrue(sentenceTrue), _sentenceFalse(sentenceFalse) {
}

ExecuteResult SentenceCondition::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_expression->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Condition, "The condition expression execute failed!");
		return ExecuteResult::Failed;
	}
	bool bTrue = ValueTool::ToLogic(_expression->GetValue().get());
	auto tempSentence = bTrue ? _sentenceTrue : _sentenceFalse;
	if (tempSentence) {
		auto tempSpace = std::make_shared<Space>(SpaceType::Condition, space);
		auto executeRet = tempSentence->Execute(tempSpace);
		if (!IsSuccess(executeRet)) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Condition, bTrue ? "The 'true' code sentence execute failed!" : "The 'false' code sentence execute failed!");
			return ExecuteResult::Failed;
		}
		if (executeRet == ExecuteResult::Return) {
			SetReturnValue(std::static_pointer_cast<SentenceReturn>(tempSentence)->GetReturnValue());
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
