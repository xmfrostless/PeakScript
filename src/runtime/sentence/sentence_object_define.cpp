#include "sentence_object_define.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"

using namespace peak;

SentenceObjectDefine::SentenceObjectDefine(const std::string& name, const std::string& parentName, std::vector<std::unique_ptr<Sentence>> sentenceList)
	: _name(name), _parentName(parentName), _sentenceList(std::move(sentenceList)) {
	_hashCode = HashFunction::String(_name);
	_parentHashCode = HashFunction::String(_parentName);
}

ExecuteResult SentenceObjectDefine::Execute(std::shared_ptr<Space> space) {
	std::shared_ptr<ValueObject> parentObject { nullptr };
	if (!_parentName.empty()) {
		auto parentVariable = space->FindVariable(_parentHashCode);
		if (!parentVariable) {
			ErrorLogger::LogRuntimeError(_parentName);
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ObjectDefine, "The extends object \"" + _parentName + "\" of \"" + _name + "\" not found!");
			return ExecuteResult::Failed;
		}
		auto parentValue = parentVariable->GetValue();
		if (!ValueTool::IsObject(parentValue.get())) {
			ErrorLogger::LogRuntimeError(_parentName);
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ObjectDefine, "The variable \"" + _parentName + "\" isn't a object!");
			return ExecuteResult::Failed;
		}
		parentObject = std::static_pointer_cast<ValueObject>(parentValue);
	}
	auto valueObject = std::make_shared<ValueObject>(space, parentObject);
	auto objectSpace = valueObject->GetSpace();
	for (auto& sentence : _sentenceList) {
		if (!IsSuccess(sentence->Execute(objectSpace))) {
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ObjectDefine, "The object build failed!");
			return ExecuteResult::Failed;
		}
	}

	if (!space->AddVariable(std::make_shared<Variable>(_name, _hashCode, VariableAttribute::None, valueObject))) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::ObjectDefine, "The object \"" + _name + "\" is exist!");
		return ExecuteResult::Failed;
	}

	return ExecuteResult::Successed;
}
