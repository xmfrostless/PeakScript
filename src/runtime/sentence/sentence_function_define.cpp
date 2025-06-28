#include "sentence_function_define.h"
#include "runtime/sentence/sentence_return.h"
#include "runtime/value/value_function.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"

using namespace peak;

SentenceFunctionDefine::SentenceFunctionDefine(const std::string& name, const std::vector<std::string>& params, std::unique_ptr<Sentence> content)
	: _name(name), _params(params), _content(std::move(content)) {
}
ExecuteResult SentenceFunctionDefine::Execute(std::shared_ptr<Space> space) {
	auto func = [this](const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space> space) -> std::shared_ptr<Value> {
		auto result = _content->Execute(space);
		if (!IsSuccess(result)) {
			return nullptr;
		}
		if (result == ExecuteResult::Return) {
			return static_cast<SentenceReturn*>(_content.get())->GetReturnValue();
		}
		return std::make_shared<ValueNull>();
	};

	auto variable = space->FindVariableFromTop(_name);
	if (variable) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::FunctionDefine, "The \"" + _name + "\" is exist!");
		return ExecuteResult::Failed;
	} else {
		variable = std::make_shared<Variable>(_name, VariableAttribute::None);
		auto value = std::make_shared<ValueFunction>(_params, func);
		variable->SetValue(value);
		space->AddVariable(variable);
	}

	return ExecuteResult::Successed;
}