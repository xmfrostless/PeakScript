#include "space.h"
#include "builtin/builtin_function.h"
#include "module.h"
#include "module_pool.h"
#include "value/value.h"
#include "variable.h"
#include "value/value_tool.h"

using namespace peak;

Space::Space(SpaceType spaceType)
	: _spaceType(spaceType) {
}
Space::Space(SpaceType spaceType, std::shared_ptr<Space> outSpace)
	: _spaceType(spaceType), _outSpace(outSpace) {
}

std::shared_ptr<Space> Space::CopySpace() const {
	auto space = std::make_shared<Space>(_spaceType);
	space->SetParent(_parent ? _parent->CopySpace() : nullptr);
	space->_usingSpace = _usingSpace;

	for (auto& pair : _variables) {
		space->_variables.emplace(pair.first, pair.second->Clone());
	}
	return space;
}

void Space::Clear() {
	_variables.clear();
	_usingSpace.clear();
}

void Space::SetParent(std::shared_ptr<Space> parent) {
	this->_parent = parent;
}

void Space::AddUsingSpace(std::shared_ptr<Space> space) {
	if (space) {
		_usingSpace.emplace_back(space);
	}
}

bool Space::AddVariable(std::shared_ptr<Variable> value) {
	if (!value) {
		return false;
	}
	auto findIte = _variables.find(value->GetName());
	if (findIte != _variables.end()) {
		ErrorLogger::LogRuntimeError(value->GetName());
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Space, "The variable \"" + value->GetName() + "\" is exist!");
		return false;
	}
	_variables.emplace(value->GetName(), value);
	return true;
}

std::shared_ptr<Variable> Space::FindVariableFromTop(const std::string& name) const {
	auto ite = _variables.find(name);
	if (ite != _variables.end()) {
		return ite->second;
	}
	return nullptr;
}

std::shared_ptr<Variable> Space::FindVariable(const std::string& name) const {
	auto ite = _variables.find(name);
	if (ite != _variables.end()) {
		return ite->second;
	}
	if (_parent) {
		auto ret = _parent->FindVariable(name);
		if (ret) {
			return ret;
		}
	}
	if (_outSpace) {
		auto ret = _outSpace->FindVariable(name);
		if (ret) {
			return ret;
		}
	}
	for (auto space : _usingSpace) {
		auto find = space->FindVariable(name);
		if (find) {
			return find;
		}
	}
	return BuiltInFunction::GetInstance()->FindVariable(name);
}

std::shared_ptr<Value> Space::FindVariableValue(const std::string& name) const {
	auto va = FindVariable(name);
	if (!va) {
		return nullptr;
	}
	return va->GetValue();
}

std::shared_ptr<ValueArray> Space::FindVariableValueAsArray(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsArray(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueArray>(value);
}
std::shared_ptr<ValueBool> Space::FindVariableValueAsBool(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsBool(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueBool>(value);
}
std::shared_ptr<ValueFunction> Space::FindVariableValueAsFunction(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsFunction(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueFunction>(value);
}
std::shared_ptr<ValueNull> Space::FindVariableValueAsNull(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsNull(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueNull>(value);
}
std::shared_ptr<ValueNumber> Space::FindVariableValueAsNumber(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsNumber(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueNumber>(value);
}
std::shared_ptr<ValueObject> Space::FindVariableValueAsObject(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsObject(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueObject>(value);
}
std::shared_ptr<ValueString> Space::FindVariableValueAsString(const std::string& name) const {
	auto value = FindVariableValue(name);
	if (!value || !ValueTool::IsString(value.get())) {
		return nullptr;
	}
	return std::static_pointer_cast<ValueString>(value);
}

SpaceType Space::GetSpaceType() const {
	return _spaceType;
}

std::unordered_map<std::string, std::shared_ptr<Variable>>& Space::GetVariables() {
	return _variables;
}
