#include "sentence_foreach.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"
#include "sentence_expression.h"

using namespace peak;

SentenceForeach::SentenceForeach(const std::string& name, const std::string& indexParam, std::unique_ptr<SentenceExpression> expression, std::unique_ptr<Sentence> sentence)
	: _name(name), _indexParam(indexParam), _expression(std::move(expression)), _sentence(std::move(sentence)) {
}
ExecuteResult SentenceForeach::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_expression->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Foreach, "The expression execute failed!");
		return ExecuteResult::Failed;
	}
	if (!ValueTool::IsArray(_expression->GetValue().get())) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Foreach, "The expression isn't a array!");
		return ExecuteResult::Failed;
	}
	auto& arr = std::static_pointer_cast<ValueArray>(_expression->GetValue())->GetArray();
	if (arr.empty()) {
		return ExecuteResult::Successed;
	}

	auto tempSpace = std::make_shared<Space>(SpaceType::Loop, space);
	auto itemVariable = std::make_shared<Variable>(_name, VariableAttribute::None);
	if (!tempSpace->AddVariable(itemVariable)) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Foreach, "The variable \"" + _name + "\" execute failed!");
		return ExecuteResult::Failed;
	}
	auto contentSpace = std::make_shared<Space>(SpaceType::Loop, tempSpace);

	if (_indexParam.empty()) {
		for (auto item : arr) {
			itemVariable->SetValue(item->GetValue());
			contentSpace->Clear();
			auto ret = _sentence->Execute(contentSpace);
			if (!IsSuccess(ret)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Foreach, "The sentence execute failed!");
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
	} else {
		auto indexVariable = std::make_shared<Variable>(_indexParam, VariableAttribute::None);
		if (!_indexParam.empty()) {
			if (!tempSpace->AddVariable(indexVariable)) {
				ErrorLogger::LogRuntimeError(_indexParam);
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Loop, "The variable \"" + _indexParam + "\" is exist!");
				return ExecuteResult::Failed;
			}
		}
		for (auto i = 0u; i < arr.size(); ++i) {
			indexVariable->SetValue(std::make_shared<ValueNumber>(i));
			itemVariable->SetValue(arr[i]->GetValue());
			contentSpace->Clear();
			auto ret = _sentence->Execute(contentSpace);
			if (!IsSuccess(ret)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Foreach, "The sentence execute failed!");
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