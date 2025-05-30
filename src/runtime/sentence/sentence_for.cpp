#include "sentence_for.h"
#include "runtime/value/value_tool.h"
#include "sentence_expression.h"

using namespace peak;

SentenceFor::SentenceFor(std::shared_ptr<Sentence> sentence0,
						 std::shared_ptr<SentenceExpression> expression0,
						 std::shared_ptr<SentenceExpression> expression1,
						 std::shared_ptr<Sentence> content)
	: _sentence0(sentence0), _expression0(expression0), _expression1(expression1), _content(content) {
}

ExecuteResult SentenceFor::Execute(std::shared_ptr<Space> space) {
	auto tempSpace = std::make_shared<Space>(SpaceType::Loop, space);
	if (_sentence0) {
		if (!IsSuccess(_sentence0->Execute(tempSpace))) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::For, "The 'sentence0' of 'for' execute failed!");
			return ExecuteResult::Failed;
		}
	}

	auto contentSpace = std::make_shared<Space>(SpaceType::Loop, tempSpace);
	while (true) {
		if (_expression0) {
			if (!IsSuccess(_expression0->Execute(tempSpace))) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::For, "The condition0-expression of 'for' execute failed!");
				return ExecuteResult::Failed;
			}
			if (!ValueTool::ToLogic(_expression0->GetValue().get())) {
				break;
			}
		}
		if (_content) {
			contentSpace->Clear();
			auto ret = _content->Execute(contentSpace);
			if (!IsSuccess(ret)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::For, "The sentence of 'for' execute failed!");
				return ExecuteResult::Failed;
			}
			if (ret == ExecuteResult::Break) {
				break;
			}
			if (ret == ExecuteResult::Return) {
				SetReturnValue(std::static_pointer_cast<SentenceReturn>(_content)->GetReturnValue());
				tempSpace->Clear();
				return ExecuteResult::Return;
			}
		}
		if (_expression1) {
			if (!IsSuccess(_expression1->Execute(tempSpace))) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::For, "The condition1-expression of 'for' execute failed!");
				return ExecuteResult::Failed;
			}
		}
	}
	tempSpace->Clear();
	return ExecuteResult::Successed;
}