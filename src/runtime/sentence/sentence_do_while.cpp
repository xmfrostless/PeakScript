#include "sentence_do_while.h"
#include "runtime/value/value_tool.h"
#include "sentence_expression.h"

using namespace peak;

SentenceDoWhile::SentenceDoWhile(std::unique_ptr<SentenceExpression> expression, std::unique_ptr<Sentence> sentence)
	: _expression(std::move(expression)), _sentence(std::move(sentence)) {
}
ExecuteResult SentenceDoWhile::Execute(std::shared_ptr<Space> space) {
	auto tempSpace = std::make_shared<Space>(SpaceType::Loop, space);
	while (true) {
		tempSpace->Clear();
		auto ret = _sentence->Execute(tempSpace);

		if (!IsSuccess(ret)) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::DoWhile, "The sentence execute failed!");
			return ExecuteResult::Failed;
		}

		if (ret == ExecuteResult::Break) {
			break;
		}
		if (ret == ExecuteResult::Return) {
			SetReturnValue(static_cast<SentenceReturn*>(_sentence.get())->GetReturnValue());
			tempSpace->Clear();
			return ExecuteResult::Return;
		}

		if (!IsSuccess(_expression->Execute(space))) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::DoWhile, "The condition expression execute failed!");
			return ExecuteResult::Failed;
		}
		if (!ValueTool::ToLogic(_expression->GetValue().get())) {
			break;
		}
	}
	tempSpace->Clear();
	return ExecuteResult::Successed;
}