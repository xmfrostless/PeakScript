#include "variable.h"
#include "value/value_null.h"

using namespace peak;

Variable::Variable(const std::string& name, VariableAttribute attribute, std::shared_ptr<Value> value)
	: _name(name), _attribute(attribute) {
	_value = value ? value : ValueNull::DEFAULT_VALUE;
	_hashCode = HashFunction::String(_name);
}

Variable::Variable(const std::string& name, std::size_t hashCode, VariableAttribute attribute, std::shared_ptr<Value> value)
	: _name(name), _hashCode(hashCode), _attribute(attribute) {
	_value = value ? value : ValueNull::DEFAULT_VALUE;
}

VariableAttribute Variable::GetAttribute() const {
	return _attribute;
}

const std::string& Variable::GetName() const {
	return _name;
}

std::size_t Variable::GetHashCode() const {
	return _hashCode;
}

bool Variable::SetValue(std::shared_ptr<Value> value) {
	switch (_attribute) {
	case VariableAttribute::None:
		_value = value;
		return true;
	case VariableAttribute::Const:
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Variable, "Can't set value, The variable \"" + _name + "\" is 'const'!");
		return false;
	}
	return true;
}
std::shared_ptr<Value> Variable::GetValue() const {
	return _value;
}

std::shared_ptr<Variable> Variable::Clone() const {
	return std::make_shared<Variable>(_name, _hashCode, _attribute, _value->Clone());
}
