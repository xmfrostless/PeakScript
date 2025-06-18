#include "value_string.h"

using namespace peak;

ValueString::ValueString(const std::string& value)
	: _value(value) {
}
std::string& ValueString::GetValue() {
	return _value;
}
std::string ValueString::ToString() const {
	return _value;
}
std::string ValueString::ToRawString() const {
	std::string ret;
	ret.reserve(_value.size() + 2);
	ret.append("\"");
	ret.append(_value);
	ret.append("\"");
	return ret;
}
std::shared_ptr<Value> ValueString::Clone() const {
	return std::make_shared<ValueString>(_value);
}
