#include "value_object.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/module.h"

using namespace peak;

ValueObject::ValueObject(std::shared_ptr<Module> module) {
	_space = module->GetSpace();
}

ValueObject::ValueObject(std::shared_ptr<Space> indexSpace) {
	_space = std::make_shared<Space>(SpaceType::Object, indexSpace);
}

std::shared_ptr<Value> ValueObject::Clone() const {
	auto ret = std::make_shared<ValueObject>();
	ret->_space = _space->CopySpace();
	return ret;
}

std::shared_ptr<Space> ValueObject::GetSpace() const {
	return _space;
}

std::string ValueObject::ToString() const {
	std::string ret = "{ ";
	const auto& variables = _space->GetVariables();
	auto index = 0u;
	for (auto& item : variables) {
		ret += item.second->GetName();
		ret += "=";
		ret += item.second->GetValue()->ToRawString();
		if (index < variables.size() - 1) {
			ret += ", ";
		}
		index += 1;
	}
	ret += " }";
	return ret;
}

std::string ValueObject::ToRawString() const {
	return ToString();
}

bool ValueObject::UpdateValue(const std::string& name, std::shared_ptr<Value> value) {
	auto variable = _space->FindVariable(name);
	if (!variable) {
		return false;
	}
	variable->SetValue(value);
	return true;
}
