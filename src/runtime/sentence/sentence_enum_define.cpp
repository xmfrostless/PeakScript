#include "sentence_enum_define.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"

using namespace peak;

SentenceEnumDefine::SentenceEnumDefine(const std::string& name, const std::vector<std::pair<std::string, std::shared_ptr<ValueNumber>>>& valueList)
	: _name(name) {
	_hashCode = HashFunction::String(_name);
	_valueList.resize(valueList.size());
	std::transform(valueList.begin(), valueList.end(), _valueList.begin(), [](const std::pair<std::string, std::shared_ptr<ValueNumber>>& item) {
		return std::make_tuple(item.first, HashFunction::String(item.first), item.second);
	});
}

ExecuteResult SentenceEnumDefine::Execute(std::shared_ptr<Space> space) {
	auto valueObject = std::make_shared<ValueObject>();
	auto variable = std::make_shared<Variable>(_name, _hashCode, VariableAttribute::Const, valueObject);
	if (!space->AddVariable(variable)) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::EnumDefine, "The enum name \"" + _name + "\" is exist!");
		return ExecuteResult::Failed;
	}
	auto valueSpace = valueObject->GetSpace();
	for (auto& tu : _valueList) {
		auto valueVariable = std::make_shared<Variable>(std::get<0>(tu), std::get<1>(tu), VariableAttribute::Const, std::get<2>(tu));
		if (!valueSpace->AddVariable(valueVariable)) {
			ErrorLogger::LogRuntimeError(std::get<0>(tu));
			ErrorLogger::LogRuntimeError(_name);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::EnumDefine, "The item name \"" + _name + "\" is exist!");
			return ExecuteResult::Failed;
		}
	}
	return ExecuteResult::Successed;
}
