#include "sentence_loop.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"
#include "sentence_expression.h"

using namespace peak;

SentenceLoop::SentenceLoop(const std::string& indexParam, std::unique_ptr<SentenceExpression> condition, std::unique_ptr<Sentence> sentence)
	: _indexParam(indexParam), _condition(std::move(condition)), _sentence(std::move(sentence)) {
}
ExecuteResult SentenceLoop::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_condition->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Loop, "The condition execute failed!");
		return ExecuteResult::Failed;
	}
	auto value = _condition->GetValue();
	if (!ValueTool::IsNumber(value.get())) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Loop, "The value of condition isn't a number!");
		return ExecuteResult::Failed;
	}
	auto count = std::max(0, static_cast<int>(std::static_pointer_cast<ValueNumber>(value)->GetValue()));
	auto tempSpace = std::make_shared<Space>(SpaceType::Loop, space);
	if (_indexParam.empty()) {
		for (int i = 0; i < count; ++i) {
			tempSpace->Clear();
			auto ret = _sentence->Execute(tempSpace);
			if (!IsSuccess(ret)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Loop, "The sentence execute failed!");
				return ExecuteResult::Failed;
			}
			if (ret == ExecuteResult::Break) {
				break;
			}
		}
	} else {
		tempSpace->Clear();
		auto indexVariable = std::make_shared<Variable>(_indexParam, VariableAttribute::None);
		if (!tempSpace->AddVariable(indexVariable)) {
			ErrorLogger::LogRuntimeError(_indexParam);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Loop, "The variable \"" + _indexParam + "\" is exist!");
			return ExecuteResult::Failed;
		}
		for (int i = 0; i < count; ++i) {
			if (!indexVariable->SetValue(std::make_shared<ValueNumber>(i))) {
				return ExecuteResult::Failed;
			}
			auto ret = _sentence->Execute(tempSpace);
			if (!IsSuccess(ret)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Loop, "The sentence execute failed!");
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
		}
	}
	tempSpace->Clear();
	return ExecuteResult::Successed;
}