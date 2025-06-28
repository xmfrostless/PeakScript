#include "value_array.h"
#include "runtime/variable.h"

using namespace peak;

ValueArray::ValueArray() {
}
ValueArray::ValueArray(const std::vector<std::shared_ptr<Variable>>& value)
	: _value(value) {
}
std::string ValueArray::ToString() const {
	std::string ret = "[";
	auto arrSize = _value.size();
	for (auto i = 0u; i < arrSize; ++i) {
		ret += _value[i]->GetValue()->ToRawString();
		if (i != arrSize - 1) {
			ret += ", ";
		}
	}
	ret += "]";
	return ret;
}
std::string ValueArray::ToRawString() const {
	return ToString();
}
std::shared_ptr<Value> ValueArray::Clone() const {
	auto valueArray = std::make_shared<ValueArray>();
	for (auto item : _value) {
		auto variable = std::make_shared<Variable>(item->GetName(), item->GetHashCode(), item->GetAttribute());
		if (item->GetValue()) {
			variable->SetValue(item->GetValue()->Clone());
		}
		valueArray->EmplaceBack(variable);
	}
	return valueArray;
}

std::vector<std::shared_ptr<Variable>>& ValueArray::GetArray() {
	return _value;
}

void ValueArray::Reserve(std::size_t size) {
	_value.reserve(size);
}
void ValueArray::EmplaceBack(std::shared_ptr<Variable> variable) {
	_value.emplace_back(variable);
}
void ValueArray::EmplaceBack(std::shared_ptr<Value> value) {
	auto variable = std::make_shared<Variable>("", 0u, VariableAttribute::None, value);
	_value.emplace_back(variable);
}